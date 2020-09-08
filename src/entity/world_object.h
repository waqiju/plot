/*
 * World中对象的公共基类
 */
#pragma once
#include <string>


class WorldObject
{
public:
    std::string name = "Unnamed";
    int GetInstanceID();
    int GetHashCode();
    std::string ToString();

private:
    static int s_InstanceCount;
};