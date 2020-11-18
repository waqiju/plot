/*
 * World中对象的公共基类
 */
#pragma once
#include <string>


class WorldObject
{
public:
    int id = 0;
    std::string name = "Unnamed";
    int GetInstanceID();
    int GetHashCode();
	virtual std::string ToString();

private:
    static int s_InstanceCount;
};