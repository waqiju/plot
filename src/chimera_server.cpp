#pragma once
#include <string>
#include "chimera_server.h"
#include "asset_loader/asset_loader.h"
#include "command/command.h"
#include "common/common.h"
#include "entity/ec.h"
#include "application/application.h"
#include "window.h"


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
    builder.SetMaxMessageSize(100 * 1024 * 1024);
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
	if (request->name() == "load_prefab")
	{
		// load prefab 参数过长，不全部打印
		int prefabCount = 0, objectCount = 0;
		for (auto p : request->parameters())
		{
			++prefabCount;
			objectCount += p.prefab().world_object_pool_size();
		}
		std::cout << "Receive command[load_prefab], Prefab[" << prefabCount << "], Object[" << objectCount << "]\n";
	}
	else
	{
		request->PrintDebugString();
	}
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

void ChimeraServerImpl::ExecuteFile(std::string path)
{
    std::string content;
    FileHelper::Read(path, content);
    pb::CommandList commandList;
	commandList.ParseFromString(content);
	for (auto& request : commandList.command_list())
	{
		ExecuteCommand(request);
	}
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

        World::ActiveWorld()->ClearCache();
	}
	else if (request.name() == "destroy_entity")
	{
		auto parameter = request.parameters(0);
		auto command = DestroyEntityCommand(parameter.p_int());
		command.Execute();

        World::ActiveWorld()->ClearCache();
	}
    else if (request.name() == "reset_plot")
    {
        auto command = ResetPlotCommand();
        command.Execute();

        World::ActiveWorld()->ClearCache();
    }
	else if (request.name() == "focus_plot")
	{
        auto parameter0 = request.parameters(0);
        auto parameter1 = request.parameters(1);
		auto command = FocusPlotCommand(parameter0.p_float(), parameter1.p_float());
		command.Execute();
	}
    else if (request.name() == "tree")
    {
		int id = 0;
		if (request.parameters_size() > 0)
		{
			id = request.parameters(0).p_int();
		}
		auto command = TreeCommand(id);
		std::string content = command.Execute();
		// TODO 返回结果
		std::cout << content << std::endl;
    }
    else if (request.name() == "set_window_title")
    {
        auto parameter0 = request.parameters(0);
		Application::MainWindow()->SetWindowTitle(parameter0.p_string());
    }
	else
	{
		std::cout << "Unsupported cammand[" << request.name() << "]\n";
	}
}
