#include <string>
#include <iostream>
#include <uWS/uWS.h>

using namespace std;

class BaseClient {
    public:
        //virtual void run();
        BaseClient(string hostname="localhost", int port=8989, bool ssl=true);
        
        void connect();
        

        friend ostream& operator<<(ostream& os, const BaseClient& bc); 
    private:
       string hostname_;
       int port_;
       bool ssl_;
       uWS::Hub *hub;
/*     int public_key; 
       int private_key;
       string wallet_id;
       string wallet_name; */

};

class MinerClient : public BaseClient {

};
