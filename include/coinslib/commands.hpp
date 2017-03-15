#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

class BaseCommand {
    public:
        BaseCommand(string command_name, vector<tuple<string, string>> args) :
            command_name_(command_name), args_(args) {}

        friend ostream& operator<<(ostream& os, const BaseCommand& bc); 
    protected:
        virtual string serialize() const; 
    private:
        string command_name_;
        vector<tuple<string, string>> args_;
};

class GetCurrentChallenge : public BaseCommand {
    public:
        GetCurrentChallenge() : BaseCommand("get_current_challenge", {}) {}
};


class GetChallengeSolution : public BaseCommand {
    public:
        GetChallengeSolution(int challenge_id) : BaseCommand("get_challenge_solution", 
                {{"challenge_id", to_string(challenge_id)}}) {}
};

class RegisterWallet : public BaseCommand {
    public:
        RegisterWallet(string name, string key, string signature) : BaseCommand("register_wallet", 
                {{"name", name}, {"key", key}, {"signature", signature}}) {}
};

class GetTransactions : public BaseCommand {
    public:
        GetTransactions(int start, int count) : BaseCommand("get_transactions", 
                {{"start", start}, {"count", count}}) {}
};

