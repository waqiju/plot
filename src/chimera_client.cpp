#pragma once
#include <string>
#include <windows.h>
#include "chimera_client.h"
#include "common/common.h"


std::mutex ChimeraClient::CallLock;
ChimeraClient* ChimeraClient::client = nullptr;
std::thread* ChimeraClient::PollThread = nullptr;


void ChimeraClient::InitializeClient(std::string ip, int port)
{
    if (client != nullptr)
    {
        delete client;
    }
    client = new ChimeraClient(grpc::CreateChannel(
        ip + ":" + std::to_string(port), grpc::InsecureChannelCredentials()));
    PollThread = new std::thread(ChimeraClient::Poll);
}


void ChimeraClient::Poll()
{
    while (true)
    {
        CallLock.lock();
        if (client->m_CommandList.size() <= 0)
        {
            CallLock.unlock();
            Sleep(50);
            continue;
        }

        for (CommandRequest& request : client->m_CommandList)
        {
            try
            {
                ClientContext context;
                CommandReply reply;
                Status status = client->m_Stub->Call(&context, request, &reply);

                if (!status.ok())
                {
                    std::cout << "Client request[" << request.name() << "] exception!\n" << status.error_code() << ": " << status.error_message() << std::endl;
                }
            }
            catch (std::exception& e)
            {
                std::cout << "Client request[" << request.name() << "] exception!\n" << e.what() << std::endl;
            }
        }
        client->m_CommandList.clear();
        CallLock.unlock();
        Sleep(50);
    }
}


void ChimeraClient::SubmitCommand(const CommandRequest& request)
{
    std::lock_guard<std::mutex> scopedLock(CallLock);
    m_CommandList.push_back(request);

}
