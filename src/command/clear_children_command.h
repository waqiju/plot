#pragma once


class ClearChildrenCommand
{
public:
    ClearChildrenCommand(int objectID);
    void Execute();

private:
    int m_ObjectID;
};
