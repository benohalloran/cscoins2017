#include <string>
#include <iostream>
#include "coinslib/clients.hpp"

std::ostream& operator<<(std::ostream& os, const BaseClient& bc)  {  
    os << "BaseClient hostname: " << bc.hostname_ << " port: " << bc.port_;  
    return os;  
}  

/*
    uWS::Hub h;
    h.onError([](void *user) {
        switch ((long) user) {
        case 1:
            std::cout << "Client emitted error on invalid URI" << std::endl;
            break;
        case 2:
            std::cout << "Client emitted error on resolve failure" << std::endl;
            break;
        case 3:
            std::cout << "Client emitted error on connection timeout (non-SSL)" << std::endl;
            break;
        case 5:
            std::cout << "Client emitted error on connection timeout (SSL)" << std::endl;
            break;
        case 6:
            std::cout << "Client emitted error on HTTP response without upgrade (non-SSL)" << std::endl;
            break;
        case 7:
            std::cout << "Client emitted error on HTTP response without upgrade (SSL)" << std::endl;
            break;
        default:
            std::cout << "FAILURE: " << user << " should not emit error!" << std::endl;
            exit(-1);
        }
    });


    h.onConnection([](uWS::WebSocket<uWS::CLIENT> ws, uWS::HttpRequest req) {
        uWS::OpCode opcode = uWS::OpCode::TEXT;
        StringBuffer s;
        Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("command");
        writer.String("get_current_challenge");
        writer.Key("args");
        writer.StartArray();
        writer.EndArray();
        writer.EndObject();
        const char *payload = s.GetString();
        int payloadlength = strlen(s.GetString());
        switch ((long) ws.getUserData()) {
        case 4:
            std::cout << "Client established a remote connection over non-SSL" << std::endl;
            ws.close(1000);
            break;
        case 5:
            std::cout << "Client established a remote connection over SSL" << std::endl;
            std::cout << "sending payload: " << s.GetString() << std::endl;
            ws.send(payload, payloadlength, opcode);
            //ws.close(1000);
            break;
        default:
            std::cout << "FAILURE: " << ws.getUserData() << " should not connect!" << std::endl;
            exit(-1);
        }
    });

    // {'command': 'submission', 'args': {'challenge_id': challenge_id, 'nonce': nonce, 'hash': hash, 'signature': signature, 'wallet_id': self.wallet_id}}
    h.onMessage([](uWS::WebSocket<uWS::CLIENT> ws, char *message, size_t length, uWS::OpCode opCode) {
        uWS::OpCode opcode = uWS::OpCode::TEXT;
        StringBuffer s;
        Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("command");
        writer.String("submission");
        writer.Key("args");
        writer.StartObject();
        writer.Key("challenge_id");
        writer.String();
        writer.Key("nonce");
        writer.String();
        writer.Key("hash");
        writer.String();

        writer.Key("signature");
        writer.Key("wallet_id");
        
        writer.EndObject();
        writer.EndObject();



        std::cout << std::string(message, length) << std::endl; 
        solution s = solve(message);
        std::cout << "id: " << s.id << " nonce: " << s.nonce << " hash: " << s.hash << std::endl;
        //ws.send(payload, payloadlength, opcode);

        //ws.close(1000);
    });

    h.onDisconnection([](uWS::WebSocket<uWS::CLIENT> ws, int code, char *message, size_t length) {
        std::cout << "Client got disconnected with data: " << ws.getUserData() << ", code: " << code << ", message: <" << std::string(message, length) << ">" << std::endl;
    });

    h.connect("wss://ec2-54-186-71-144.us-west-2.compute.amazonaws.com:8989/client", (void *) 5);
    h.run();

    */
