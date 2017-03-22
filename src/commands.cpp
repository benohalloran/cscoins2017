#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "rapidjson/prettywriter.h"
#include "coinslib/commands.hpp"

using namespace std;
using namespace rapidjson;

string BaseCommand::serialize()
const {
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("command");
    writer.String(this->command_name_.c_str());
    writer.Key("args");
    writer.StartObject();
    for(auto &arg : this->args_) {
        writer.Key((arg.first).c_str());
        writer.String((arg.second).c_str());
    }
    writer.EndObject();
    writer.EndObject();
    return string(s.GetString());
}

std::ostream& operator<<(std::ostream& os, const BaseCommand& bc)  {
    os << bc.serialize();
    return os;  
} 

