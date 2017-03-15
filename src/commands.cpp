#include <iostream>
#include <string>
#include <vector>
#include <tuple>
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
        writer.Key(get<0>(arg).c_str());
        writer.String(get<1>(arg).c_str());
    }
    writer.EndObject();
    writer.EndObject();
    return string(s.GetString());
}

std::ostream& operator<<(std::ostream& os, const BaseCommand& bc)  {
    os << bc.serialize();
    return os;  
} 

