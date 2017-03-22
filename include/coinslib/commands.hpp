#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

class BaseCommand {
    public:
        BaseCommand(string command_name, vector<pair<string, string>> args) :
            command_name_(command_name), args_(args) {}

        friend ostream& operator<<(ostream& os, const BaseCommand& bc); 

        virtual string serialize() const; 
    private:
        string command_name_;
        vector<pair<string, string>> args_;
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
                {{"start", to_string(start)}, {"count", to_string(count)}}) {}
};

class CreateTransaction : public BaseCommand {
    public:
        CreateTransaction(string source, string recipient, double amount, string signature) : BaseCommand("create_transaction", 
                {{"source", source}, {"recipient", recipient}, {"amount", to_string(amount)}, {"signature", signature}}) {}
};

class Submission : public BaseCommand {
    public:
        Submission(string wallet_id, string nonce) : BaseCommand("submission",
                {{"wallet_id", wallet_id}, {"nonce", nonce}}) {}
};


class GetCentralAuthorityServerInformation : public BaseCommand {
     public:
         GetCentralAuthorityServerInformation() : BaseCommand("ca_server_info", {}) {}
};

class Close : public BaseCommand {
     public:
         Close() : BaseCommand("close", {}) {}
};

