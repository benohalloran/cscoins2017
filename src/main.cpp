#include "solver.hpp"
#include "uWS/uWS.h"
#include <iostream>
#include <cstdio>
#include <string.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "coinslib/clients.hpp"
#include "coinslib/commands.hpp"

using namespace std;
using namespace rapidjson;

int main(int argc, char** argv)
{
    cout << "Starting up client..." << endl;
    string hostname = "ec2-54-186-71-144.us-west-2.compute.amazonaws.com";//"cscoins.2017.csgames.org";
    int    port     = 8989;
    bool ssl = true;
    MinerClient client (hostname, port, ssl);
    cout << client << endl;
    client.initWallet("test");
    bool check = client.testWalletSig();
    client.connect();
}
