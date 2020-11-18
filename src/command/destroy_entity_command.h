#pragma once


class DestroyEntityCommand
{
public:
    DestroyEntityCommand(int objectID);
    void Execute();

private:
    int m_ObjectID;
};
