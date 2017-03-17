#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
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
#include <cassert>
#define ASSERT assert

using std::unique_ptr;
using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using BIO_FILE_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;

using namespace std;


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
    //cout << "pub key pem: " << pubkey_pem << endl;
    //cout << "pub key der: " << pubkey_der << endl;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, pubkey_der.c_str(), pubkey_der.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
   
    string wallet_id = string(ss.str());
        
    this->wallet_name = name;
    this->wallet_id = wallet_id;
    this->public_key = pubkey_pem;
    this->private_key = privkey_pem;
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
    RSA_sign(NID_sha256, msg, msg_len, sigret, &siglen, rsa);


    RSA_free(rsa);
    BIO_free(b);
    stringstream ss;
    for(int i = 0; i < siglen; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)sigret[i];
    }

    return string(ss.str());
}


MinerClient::MinerClient(string hostname, int port, bool ssl) {

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


    hub->onConnection([this](uWS::WebSocket<uWS::CLIENT> ws, uWS::HttpRequest req) {
        uWS::OpCode opcode = uWS::OpCode::TEXT;
        string name = this->wallet_name;
        string signature = this->signMessage(this->wallet_id);
        string key = this->public_key;
        RegisterWallet registerWallet = RegisterWallet(name, key, signature);
        std::cout << registerWallet << std::endl;
        string command = registerWallet.serialize();
        
        const char *payload = command.c_str();
        int payloadlength = strlen(payload);

        switch ((long) ws.getUserData()) {
        case 4:
            std::cout << "Client established a remote connection over non-SSL" << std::endl;
            ws.close(1000);
            break;
        case 5:
            std::cout << "Client established a remote connection over SSL" << std::endl;
            std::cout << "sending payload: " << payload << std::endl;
            ws.send(payload, payloadlength, opcode);
            //ws.close(1000);
            break;
        default:
            std::cout << "FAILURE: " << ws.getUserData() << " should not connect!" << std::endl;
            exit(-1);
        }
 });
}

