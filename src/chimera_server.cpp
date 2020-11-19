#pragma once
#include <string>
#include "chimera_server.h"
#include "asset_loader/asset_loader.h"
#include "command/command.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using pb::EchoRequest;
using pb::EchoReply;
using pb::CommandRequest;
using pb::CommandReply;
using pb::ChimeraRoost;

ChimeraServerImpl ChimeraServerImpl::service;
std::unique_ptr<Server> server;
ServerBuilder builder;

std::mutex ChimeraServerImpl::CallLock;


void ChimeraServerImpl::RunServer()
{
    std::string serverAddress("0.0.0.0:2200");

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    //ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
	server = builder.BuildAndStart();
    std::cout << "Server listening on " << serverAddress << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    // server->Wait();
}

Status ChimeraServerImpl::Echo(ServerContext* context, const EchoRequest* request, EchoReply* reply)
{
    request->PrintDebugString();
    reply->set_text(request->text());
    return Status::OK;
}

Status ChimeraServerImpl::Call(ServerContext* context, const CommandRequest* request, CommandReply* reply)
{
	request->PrintDebugString();
	std::lock_guard<std::mutex> scopedLock(CallLock);
	m_CommandList.push_back(*request);

    return Status::OK;
}

void ChimeraServerImpl::ConsumeCommand()
{
    std::lock_guard<std::mutex> scopedLock(CallLock);
	if (m_CommandList.size() <= 0)
		return;

	for (CommandRequest& request : m_CommandList)
	{
		try
		{
			ExecuteCommand(request);
		}
		catch (std::exception& e)
		{
			std::cout << "Execute command["<< request.name() <<"] exception!\n" << e.what() << std::endl;
		}
	}
	m_CommandList.clear();
}

void ChimeraServerImpl::ExecuteCommand(const CommandRequest& request)
{
	if (request.name() == "load_prefab")
	{
		for (auto parameter : request.parameters())
		{
			auto loader = PrefabLoader(&parameter.prefab());
			loader.Load();
		}
	}
	else if (request.name() == "destroy_entity")
	{
		auto parameter = request.parameters(0);
		auto command = DestroyEntityCommand(parameter.p_int());
		command.Execute();
	}
    else if (request.name() == "reset_plot")
    {
        auto command = ResetPlotCommand();
        command.Execute();
    }
	else if (request.name() == "focus_plot")
	{
        auto parameter0 = request.parameters(0);
        auto parameter1 = request.parameters(1);
		auto command = FocusPlotCommand(parameter0.p_float(), parameter1.p_float());
		command.Execute();
	}
	else
	{
		std::cout << "Unsupported cammand[" << request.name() << "]\n";
	}
}
