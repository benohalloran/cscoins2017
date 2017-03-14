#include <string>

using namespace std;


class BaseClient {
    public:
        //virtual void run();
        BaseClient(string hostname="localhost", int port=8989, bool ssl=true) :
            hostname_(hostname), port_(port), ssl_(ssl) {};
    private:
       string hostname_;
       int port_;
       bool ssl_;
/*       int public_key; 
       int private_key;
       string wallet_id;
       string wallet_name; */

};
