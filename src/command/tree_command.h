/*
 * 打印出 Entity 的树结构
 */
#pragma once
#include <string>


class TreeCommand
{
public:
    TreeCommand(int id=0);
    std::string Execute();
private:
    int m_Id;
};
