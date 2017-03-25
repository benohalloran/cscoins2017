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
    string hostname = "cscoins.2017.csgames.org/client";
    int    port     = 8989;
    bool ssl = true;
    MinerClient client = MinerClient(hostname, port, ssl);
    cout << client << endl;
    client.initWallet("test");
    bool check = client.testWalletSig();
    client.connect();
}
