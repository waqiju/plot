import grpc
from pb import chimera_pb2_grpc
from pb import chimera_pb2


server_address = '127.0.0.1:2200'
channel = grpc.insecure_channel(server_address)
stub = chimera_pb2_grpc.ChimeraRoostStub(channel)


def test():
    request = chimera_pb2.EchoRequest(text='Hello')
    response = stub.Echo(request, timeout=10)
    print(response)


if __name__ == '__main__':
    test()
