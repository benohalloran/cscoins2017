#include "solver.hpp"
#include <uWS/uWS.h>
#include <iostream>
#include <cstdio>
#include <string.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "coinslib/baseclient.hpp"
#include "coinslib/commands.hpp"

using namespace std;
using namespace rapidjson;

int main(int argc, char** argv)
{
    cout << "Starting up client..." << endl;
    string hostname = "ec2-54-186-71-144.us-west-2.compute.amazonaws.com";
    int    port     = 8989;
    BaseClient client = BaseClient(hostname, port, true);
    cout << client << endl;
    GetCurrentChallenge command1 = GetCurrentChallenge();
    cout << command1 << endl;
    GetChallengeSolution command2 = GetChallengeSolution(100);
    cout << command2 << endl;
    GetCentralAuthorityServerInformation command3 = GetCentralAuthorityServerInformation();
    cout << command3 << endl;

    cout << "Shutting down client..." << std::endl;
}
