#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <uWS/uWS.h>
#include <memory>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/sha.h>
#include "coinslib/clients.hpp"
#include "coinslib/commands.hpp"
#include "solver.hpp"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include <cassert>
#include <regex>
#define ASSERT assert

using std::unique_ptr;
using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using BIO_FILE_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;

using namespace std;
using namespace rapidjson;


BaseClient::BaseClient(string hostname, int port, bool ssl) {
   this->hostname_ = hostname;
    this->port_ = port;
    this->ssl_ = ssl;
    this->hub = new uWS::Hub();
    hub->onError([](void *user) {
        switch ((long) user) {
        case 1:
            std::cout << "Client emitted error on invalid URI" << std::endl;
            break;
        case 2:
            std::cout << "Client emitted error on resolve failure" << std::endl;
            break;
        case 3:
            std::cout << "Client emitted error on connection timeout (non-SSL)" << std::endl;
            break;
        case 5:
            std::cout << "Client emitted error on connection timeout (SSL)" << std::endl;
            break;
        case 6:
            std::cout << "Client emitted error on HTTP response without upgrade (non-SSL)" << std::endl;
            break;
        case 7:
            std::cout << "Client emitted error on HTTP response without upgrade (SSL)" << std::endl;
            break;
        default:
            std::cout << "FAILURE: " << user << " should not emit error!" << std::endl;
            exit(-1);
        }
    });


    hub->onMessage([](uWS::WebSocket<uWS::CLIENT> ws, char *message, size_t length, uWS::OpCode opCode) {
        uWS::OpCode opcode = uWS::OpCode::TEXT;
        std::cout << std::string(message, length) << std::endl;


 /*     solution s = solve(message);
        auto submission = Submission(to_string(s.id), to_string(s.nonce));
        std::cout << "id: " << s.id << " nonce: " << s.nonce << " hash: " << s.hash << std::endl;
        std::cout << submission << std::endl;
        string command = submission.serialize();

        const char *payload = command.c_str();
        int payloadlength = strlen(payload);

        ws.send(payload, payloadlength, opcode); */

        ws.close(1000);
    });

    hub->onDisconnection([](uWS::WebSocket<uWS::CLIENT> ws, int code, char *message, size_t length) {
        std::cout << "Client got disconnected with data: " << ws.getUserData() << ", code: " << code << ", message: <" << std::string(message, length) << ">" << std::endl;
    }); 



} 

void BaseClient::connect() {
    std::stringstream sstream;
    sstream << (this->ssl_ ? "wss://" : "ws://") << this->hostname_ << ":" << this->port_;
    this->hub->connect(sstream.str(), (void *) 5);
    this->hub->run();
}

std::ostream& operator<<(std::ostream& os, const BaseClient& bc)  {  
    os << "BaseClient hostname: " << bc.hostname_ << " port: " << bc.port_;  
    return os;  
}


void MinerClient::connect() {
    std::stringstream sstream;
    sstream << (this->ssl_ ? "wss://" : "ws://") << this->hostname_ << ":" << this->port_ << "/client";
    this->hub->connect(sstream.str(), (void *) 5);
    this->hub->run();
}

void MinerClient::initWallet(string name) {
    this->wallet_name = name;
    
    std::ifstream ifs_public_pem_f("rsa-public.pem");
    std::ifstream ifs_private_pem_f("rsa-private.pem");
    std::ifstream ifs_wallet_id_f("wallet_id.txt");
    std::ifstream ifs_wallet_sig_f("wallet.sig");

    string public_key_f((std::istreambuf_iterator<char>( ifs_public_pem_f )), 
            std::istreambuf_iterator<char>());
    string private_key_f((std::istreambuf_iterator<char>( ifs_private_pem_f )), 
            std::istreambuf_iterator<char>());

    string wallet_sig_f((std::istreambuf_iterator<char>( ifs_wallet_sig_f )), 
            std::istreambuf_iterator<char>());
    string wallet_id_f((std::istreambuf_iterator<char>( ifs_wallet_id_f )), 
            std::istreambuf_iterator<char>());



    if( wallet_id_f != "" && wallet_sig_f != "" && private_key_f != "" && public_key_f != "") {
        this->wallet_id = wallet_id_f;
        this->wallet_sig = wallet_sig_f;
        this->public_key = public_key_f;
        this->private_key = private_key_f;
        return;
    }

    int rc;

    RSA_ptr rsa(RSA_new(), ::RSA_free);
    BN_ptr bn(BN_new(), ::BN_free);

    BIO_FILE_ptr der1(BIO_new_file("rsa-public.der", "w"), ::BIO_free);
    BIO_FILE_ptr der2(BIO_new_file("rsa-private.der", "w"), ::BIO_free);
    BIO_FILE_ptr pem1(BIO_new_file("rsa-public.pem", "w"), ::BIO_free);
    BIO_FILE_ptr pem2(BIO_new_file("rsa-private.pem", "w"), ::BIO_free);

    rc = BN_set_word(bn.get(), RSA_F4);
    ASSERT(rc == 1);

    // Generate key
    rc = RSA_generate_key_ex(rsa.get(), 1024, bn.get(), NULL);
    ASSERT(rc == 1);

    // Convert RSA to PKEY
    EVP_KEY_ptr pkey(EVP_PKEY_new(), ::EVP_PKEY_free);
    rc = EVP_PKEY_set1_RSA(pkey.get(), rsa.get());
    ASSERT(rc == 1);

    // Write public key in ASN.1/DER
    rc = i2d_RSAPublicKey_bio(der1.get(), rsa.get());

    // Write public key in PKCS PEM
    rc = PEM_write_bio_RSAPublicKey(pem1.get(), rsa.get());
    ASSERT(rc == 1);

     // Write private key in ASN.1/DER
    rc = i2d_RSAPrivateKey_bio(der2.get(), rsa.get());
    ASSERT(rc == 1);

     // Write private key in PKCS PEM.
    rc = PEM_write_bio_PrivateKey(pem2.get(), pkey.get(), NULL, NULL, 0, NULL, NULL);
    ASSERT(rc == 1);

    BIO_flush(pem1.get());
    BIO_flush(pem2.get());
    BIO_flush(der1.get());
    BIO_flush(der2.get());


    std::ifstream ifsder("rsa-public.der");
    std::ifstream ifspem_pub("rsa-public.pem");
    std::ifstream ifspem_priv("rsa-private.pem");
    string pubkey_der((std::istreambuf_iterator<char>(ifsder)), 
            std::istreambuf_iterator<char>());
    string pubkey_pem((std::istreambuf_iterator<char>(ifspem_pub)), 
            std::istreambuf_iterator<char>());
    string privkey_pem((std::istreambuf_iterator<char>(ifspem_priv)), 
            std::istreambuf_iterator<char>());
    cout << "pub key pem: " << pubkey_pem << endl;
    cout << "priv key pem: " << privkey_pem << endl;

    const int pycrypto_len = 22;
    const char *pycrypto_tags = "\x30\x81\x9f\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x01\x05\x00\x03\x81\x8d\x00";

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, pycrypto_tags, pycrypto_len);
    SHA256_Update(&sha256, pubkey_der.c_str(), pubkey_der.size());
    SHA256_Final(hash, &sha256);
       
    unsigned char* sigret = nullptr;
    sigret = reinterpret_cast<unsigned char *>( malloc(RSA_size(rsa.get())));
    unsigned int siglen = 0;
    // https://www.openssl.org/docs/man1.1.0/crypto/RSA_sign.html
    RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, sigret, &siglen, rsa.get());


    BIGNUM *hash_bn = BN_new();
    BIGNUM *sig_bn = BN_new();
    char *hash_str;
    char *sig_str;
    BN_bin2bn(hash,  SHA256_DIGEST_LENGTH, hash_bn);
    BN_bin2bn(sigret,  siglen, sig_bn);
    hash_str = BN_bn2hex(hash_bn);
    sig_str = BN_bn2hex(sig_bn);

    string wallet_sig = string(sig_str);
    string wallet_id = string(hash_str);

    transform(wallet_sig.begin(), wallet_sig.end(), wallet_sig.begin(), ::tolower);
    transform(wallet_id.begin(), wallet_id.end(), wallet_id.begin(), ::tolower);

    std::ofstream out("wallet.sig");
    out << wallet_sig;
    out.close();
    cout << "Wallet Sig" << endl;
    cout << wallet_sig << endl;

    std::ofstream out2("wallet_id.txt");
    out2 << wallet_id;
    out2.close();
    cout << "Wallet id" << endl;
    cout << wallet_id << endl;

    this->wallet_id = wallet_id;
    this->public_key = pubkey_pem;
    this->private_key = privkey_pem;
    this->wallet_sig = wallet_sig;

    OPENSSL_free(hash_str);
    OPENSSL_free(sig_str);
}

string MinerClient::signMessage(string message) {
    RSA *rsa = NULL;
    BIO *b = BIO_new(BIO_s_mem());
    BIO_puts(b, this->private_key.c_str());
    rsa = PEM_read_bio_RSAPrivateKey(b, NULL, NULL, NULL);
    if(b == NULL) {
        cout << "null b" << endl;
    }
    RSA_print(b, rsa,1);

    const unsigned char*  msg  = reinterpret_cast<const unsigned char*>(message.c_str());
    unsigned int msg_len = strlen(message.c_str());
    unsigned char* sigret = NULL;
    sigret = (unsigned char *) malloc(RSA_size(rsa));
    unsigned int siglen = 0;
    // https://www.openssl.org/docs/man1.1.0/crypto/RSA_sign.html
    RSA_sign(NID_sha256, msg, msg_len, sigret, &siglen, rsa);


    RSA_free(rsa);
    BIO_free(b);
    stringstream ss;
    for(int i = 0; i < siglen; i++)
    {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(sigret[i]); // if char then wrong << will be called
    }

    return string(ss.str());
}


bool MinerClient::testWalletSig() {


    BIO_FILE_ptr pub_der(BIO_new_file("rsa-public_test.der", "w"), ::BIO_free);

    string pubkey_pem = this->public_key;

    RSA *rsa = nullptr;
    BIO *b = BIO_new(BIO_s_mem());
    BIO_puts(b, pubkey_pem.c_str());
    rsa = PEM_read_bio_RSAPublicKey(b, nullptr, nullptr, nullptr);

    int rc = i2d_RSAPublicKey_bio(pub_der.get(), rsa);
    BIO_flush(pub_der.get());

    std::ifstream ifder("rsa-public_test.der");
    string pubkey_der((std::istreambuf_iterator<char>(ifder)), 
            std::istreambuf_iterator<char>());

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, pubkey_der.c_str(), pubkey_der.size());
    SHA256_Final(hash, &sha256);

    // converting back to see if the sig verify passes
    BIGNUM *input_hash = BN_new();
    BIGNUM *input_sig = BN_new();
    int input_hash_length = BN_hex2bn(&input_hash, this-> wallet_id.c_str());
    int input_sig_length = BN_hex2bn(&input_sig, this->wallet_sig.c_str());
    input_hash_length = (input_hash_length + 1) / 2; // BN_hex2bn() returns number of hex digits
    input_sig_length = (input_sig_length + 1) / 2; // BN_hex2bn() returns number of hex digits
    unsigned char *hash_buffer = (unsigned char*)malloc(input_hash_length);
    unsigned char *sig_buffer = (unsigned char*)malloc(input_sig_length);
    int retval_hash = BN_bn2bin(input_hash, hash_buffer);
    int retval_sig  = BN_bn2bin(input_sig, sig_buffer);

    int check2 = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, sig_buffer, input_sig_length, rsa);

    RSA_free(rsa);

    if(!check2) {
        cout << "verify of signature for wallet_id failed" << endl;
        return false;
    } 
    
    cout << "verify of signature for wallet_id passed" << endl;
    return true;
}


MinerClient::MinerClient(string hostname, int port, bool ssl) {
    init_solver();
   this->hostname_ = hostname;
    this->port_ = port;
    this->ssl_ = ssl;
    this->hub = new uWS::Hub();
    hub->onError([](void *user) {
        switch ((long) user) {
        case 1:
            std::cout << "Client emitted error on invalid URI" << std::endl;
            break;
        case 2:
            std::cout << "Client emitted error on resolve failure" << std::endl;
            break;
        case 3:
            std::cout << "Client emitted error on connection timeout (non-SSL)" << std::endl;
            break;
        case 5:
            std::cout << "Client emitted error on connection timeout (SSL)" << std::endl;
            break;
        case 6:
            std::cout << "Client emitted error on HTTP response without upgrade (non-SSL)" << std::endl;
            break;
        case 7:
            std::cout << "Client emitted error on HTTP response without upgrade (SSL)" << std::endl;
            break;
        default:
            std::cout << "FAILURE: " << user << " should not emit error!" << std::endl;
            exit(-1);
        }
    });


    hub->onMessage([this](uWS::WebSocket<uWS::CLIENT> ws, char *message, size_t length, uWS::OpCode opCode) {
        uWS::OpCode opcode = uWS::OpCode::TEXT;
        std::cout << "incoming from server: " << std::string(message, length) << std::endl;

        Document d;
        d.Parse(string(message, length).c_str()); // only get json
        assert(d.IsObject());

        bool ci  = d.HasMember("challenge_id");
        bool wi = d.HasMember("wallet_id");
        bool ts = d.HasMember("transactions");
        bool err = d.HasMember("error");
        

        if( wi  ) {
            cout << "wallet registered" << endl;
            cout << "getting current challenge" << endl;
            auto getChallenge = GetCurrentChallenge();
            string command = getChallenge.serialize();
            const char *payload = command.c_str();
            int payloadlength = strlen(payload);
            ws.send(payload, payloadlength, opcode); 
            cout << "sent challenge request" << endl;
        } else if ( ci  ) {
            cout << "solving challenge" << endl;
            solution s = solve(string(message, length).c_str());
            if (s.id == -1 && s.nonce == -1lu) {
                return;
            }
            auto submission = Submission(this->wallet_id, to_string(s.nonce));
            std::cout << "id: " << s.id << " nonce: " << s.nonce << " hash: " << s.hash << std::endl;
            std::cout << submission << std::endl;
            string command = submission.serialize();

            const char *payload = command.c_str();
            int payloadlength = strlen(payload);

            ws.send(payload, payloadlength, opcode);
            cout << "sent submission" << endl;
        } else if ( ts ) {
            std::ofstream out("transactions.txt");
            out << string(message, length);
            out.close();
        } else {
            cout << "getting current challenge" << endl;
            auto getChallenge = GetCurrentChallenge();
            string command = getChallenge.serialize();
            const char *payload = command.c_str();
            int payloadlength = strlen(payload);
            ws.send(payload, payloadlength, opcode); 
            cout << "sent challenge request" << endl;

        }

    });

    hub->onDisconnection([](uWS::WebSocket<uWS::CLIENT> ws, int code, char *message, size_t length) {
        std::cout << "Client got disconnected with data: " << ws.getUserData() << ", code: " << code << ", message: <" << std::string(message, length) << ">" << std::endl;
    }); 


    hub->onConnection([this](uWS::WebSocket<uWS::CLIENT> ws, uWS::HttpRequest req) {
        uWS::OpCode opcode = uWS::OpCode::TEXT;
        string name = this->wallet_name;
        string signature = this->wallet_sig;//this->signMessage(this->wallet_id);
        string key = this->public_key;
        RegisterWallet registerWallet = RegisterWallet(name, key, signature);
        string command = registerWallet.serialize();
        
        const char *payload = command.c_str();
        int payloadlength = strlen(payload);

        //string sample_id = "eb023422a6f0582b1655c223d4ca8f13f67f111ad7e09e6b5d1545f33b0c5872";
        GetTransactions trans = GetTransactions(0,100);
        string command2 = trans.serialize();
        const char *payload2 = command2.c_str();
        int payload2length = strlen(payload2);


        switch ((long) ws.getUserData()) {
        case 4:
            std::cout << "Client established a remote connection over non-SSL" << std::endl;
            ws.close(1000);
            break;
        case 5:
            std::cout << "Client established a remote connection over SSL" << std::endl;
            std::cout << "sending payload: " << payload << std::endl;
            ws.send(payload, payloadlength, opcode);
            ws.send(payload2, payload2length, opcode);
            //ws.close(1000);
            break;
        default:
            std::cout << "FAILURE: " << ws.getUserData() << " should not connect!" << std::endl;
            exit(-1);
        }
 });
}

