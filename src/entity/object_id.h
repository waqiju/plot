#pragma once
#include <map>


class WorldObject;

class ObjectID 
{
public:
    // 自动号段
    static const int AutoSectionBegin;
    static const int AutoSectionEnd;
    // 用户号段
    static const int UserSectionBegin;
    static const int UserSectionEnd;
    // 系统号段
    static const int SystemSectionBegin;
    static const int SystemSectionEnd;
    static const int WorldRoot;
    static const int PlotRoot;
    static const int PlotMainPanelRoot;
	static const int WorldRootTransform;
	static const int PlotRootTransform;

    static ObjectID NextAutoID(WorldObject* object);
    static WorldObject* Find(int id);

private:
    static int s_NextAutoID;
    static std::map<int, WorldObject*> s_IdToObject;

// non static
public:
    ObjectID() {}
    // ObjectID(int id);
    // ObjectID(ObjectID& other);
    void Unregister();
    // 转int
    operator int() { return m_ID; }
    void operator=(int id);
    void operator=(ObjectID& other);

private:
    int m_ID = 0;
};


