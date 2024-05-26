#pragma once
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include "pb/chimera.grpc.pb.h"
#include <mutex>
#include <vector>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using pb::EchoRequest;
using pb::EchoReply;
using pb::CommandRequest;
using pb::CommandReply;
using pb::ChimeraRoost;


class ChimeraClient {
public:
    static void InitializeClient(std::string ip="localhost", int port=2201);
    static void Poll();
    static std::thread* PollThread;

    static ChimeraClient* client;
    static std::mutex CallLock;

    ChimeraClient(std::shared_ptr<Channel> channel)
      : m_Stub(ChimeraRoost::NewStub(channel)) {}

    void SubmitCommand(const CommandRequest& request);

private:
    std::unique_ptr<ChimeraRoost::Stub> m_Stub;
	std::vector<CommandRequest> m_CommandList;
};