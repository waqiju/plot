#include "object_id.h"
#include <iostream>
#include <cassert>


const int ObjectID::AutoSectionBegin = 300000000;
const int ObjectID::AutoSectionEnd = 399999999;
// 用户号段
const int ObjectID::UserSectionBegin = 1000000;
const int ObjectID::UserSectionEnd = 1999999;
// 系统号段
const int ObjectID::SystemSectionBegin = 1000;
const int ObjectID::SystemSectionEnd = 9999;
const int ObjectID::WorldRoot = 1000;
const int ObjectID::PlotRoot = 1001;
const int ObjectID::PlotMainPanelRoot = 1010;
const int ObjectID::WorldRootTransform = 2000;
const int ObjectID::PlotRootTransform = 2001;


int ObjectID::s_NextAutoID  = ObjectID::AutoSectionBegin;
std::map<int, WorldObject*> ObjectID::s_IdToObject;


ObjectID ObjectID::NextAutoID(WorldObject* object)
{
    ObjectID objectID;
    objectID.m_ID = s_NextAutoID;
    s_IdToObject[s_NextAutoID] = object;

    ++s_NextAutoID;
    return objectID;
}


WorldObject* ObjectID::Find(int id)
{
	auto it = s_IdToObject.find(id);
	if (it == s_IdToObject.end())
		return NULL;
	else
		return it->second;
}


// ObjectID::ObjectID(int id)
// {
//     *this = id;
// }

// ObjectID::ObjectID(ObjectID& other)
// {
//     m_ID = other.m_ID;
//     other.m_ID = 0;
// }

void ObjectID::operator=(int id)
{
    if (id == 0)
        return;

    if (m_ID == 0)
    {
        m_ID = id;
        return;
    }

    if (id == m_ID)
    {
        return;
    }

    // 检查旧的和新的
    auto it = s_IdToObject.find(m_ID);
    if (it == s_IdToObject.end())
    {
        std::cout << "ID [" << m_ID << "] not exist!\n";
        assert(false);
    }
    if (s_IdToObject.find(id) != s_IdToObject.end())
    {
        std::cout << "ID [" << id << "] is duplicate!\n";
        assert(false);
    }
    // 擦除旧的
    WorldObject* object = it->second;
    s_IdToObject.erase(it);

    // 写入新的
    m_ID = id;
    s_IdToObject[m_ID] = object;
}

void ObjectID::operator=(ObjectID& other)
{
    m_ID = other.m_ID;
    other.m_ID = 0;
}

void ObjectID::Unregister()
{
    if (m_ID == 0)
    {
        return;
    }

    auto it = s_IdToObject.find(m_ID);
    if (it == s_IdToObject.end())
    {
        std::cout << "ID [" << m_ID << "] not exist!\n";
        assert(false);
    }
    s_IdToObject.erase(it);
}