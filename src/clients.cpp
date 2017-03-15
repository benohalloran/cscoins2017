#include <string>
#include <iostream>
#include <sstream>
#include <uWS/uWS.h>
#include "coinslib/clients.hpp"
#include "coinslib/commands.hpp"
#include "solver.hpp" 

BaseClient::BaseClient(string hostname, int port, bool ssl) {
    this->hostname_ = hostname;
    this->port_ = port;
    this->ssl_ = ssl;
    this->hub = new uWS::Hub();
    hub->onError([](void *user) {
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

    hub->onConnection([](uWS::WebSocket<uWS::CLIENT> ws, uWS::HttpRequest req) {
        uWS::OpCode opcode = uWS::OpCode::TEXT;
        auto getchallenge = GetCurrentChallenge();
        std::cout << getchallenge << std::endl;
        string command = getchallenge.serialize();

        const char *payload = command.c_str();
        int payloadlength = strlen(payload);
        switch ((long) ws.getUserData()) {
        case 4:
            std::cout << "Client established a remote connection over non-SSL" << std::endl;
            ws.close(1000);
            break;
        case 5:
            std::cout << "Client established a remote connection over SSL" << std::endl;
            std::cout << "sending payload: " << payload << std::endl;
            ws.send(payload, payloadlength, opcode);
            //ws.close(1000);
            break;
        default:
            std::cout << "FAILURE: " << ws.getUserData() << " should not connect!" << std::endl;
            exit(-1);
        }
    });

    hub->onMessage([](uWS::WebSocket<uWS::CLIENT> ws, char *message, size_t length, uWS::OpCode opCode) {
        uWS::OpCode opcode = uWS::OpCode::TEXT;
        std::cout << std::string(message, length) << std::endl; 
        solution s = solve(message);
        std::cout << "id: " << s.id << " nonce: " << s.nonce << " hash: " << s.hash << std::endl;
        //ws.send(payload, payloadlength, opcode);

        //ws.close(1000);
    });

    hub->onDisconnection([](uWS::WebSocket<uWS::CLIENT> ws, int code, char *message, size_t length) {
        std::cout << "Client got disconnected with data: " << ws.getUserData() << ", code: " << code << ", message: <" << std::string(message, length) << ">" << std::endl;
    });
} 

void BaseClient::connect() {
    std::stringstream sstream;
    sstream << (this->ssl_ ? "wss://" : "ws://") << this->hostname_ << ":" << this->port_;
    this->hub->connect(sstream.str(), (void *) 5);
    this->hub->run();
}

std::ostream& operator<<(std::ostream& os, const BaseClient& bc)  {  
    os << "BaseClient hostname: " << bc.hostname_ << " port: " << bc.port_;  
    return os;  
}
