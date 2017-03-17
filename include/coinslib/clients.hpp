#include <string>
#include <iostream>
#include <uWS/uWS.h>
#include <openssl/rsa.h>

using namespace std;

class BaseClient {
    public:
        //virtual void run();
        BaseClient(string hostname="localhost", int port=8989, bool ssl=true);
        
        virtual void connect();

        friend ostream& operator<<(ostream& os, const BaseClient& bc); 
    protected:
       string hostname_;
       int port_;
       bool ssl_;
       uWS::Hub *hub;
};

class MinerClient : public BaseClient {
    public:
        MinerClient(string hostname, int port, bool ssl); 
        void connect();
        string signMessage(string name);
       /* string signMessage(string message);
        string getCurrentChallenge();
        string getChallengeSolution(int challenge_id);
        string registerWallet(string name);
        void   initWallet();
        void   loadKeys(string public_key_path, string private_key_path);
        void   exportKeys(string public_key_path, string private_key_path);
        string submission(string wallet_id, string nonce);
        string getCAInfo();  
        string getTransactions(int start, int count);
        string createTransaction(string recipient, double amount);*/
        void initWallet(string name);
    private:
        string wallet_name;
        string wallet_id;
        string public_key;
        string private_key;
};
