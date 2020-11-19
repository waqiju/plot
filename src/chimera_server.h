#pragma once
#include <string>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include "pb/chimera.grpc.pb.h"
#include <mutex>
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using pb::EchoRequest;
using pb::EchoReply;
using pb::CommandRequest;
using pb::CommandReply;
using pb::ChimeraRoost;


class ChimeraServerImpl final : public ChimeraRoost::Service {
public:
	static void RunServer();
    static ChimeraServerImpl service;
    static std::mutex CallLock;

    Status Echo(ServerContext* context, const EchoRequest* request, EchoReply* reply) override;
    Status Call(ServerContext* context, const CommandRequest* request, CommandReply* reply) override;

	void ConsumeCommand();

private:
	void ExecuteCommand(const CommandRequest& request);

	std::vector<CommandRequest> m_CommandList;
};