#pragma once


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


    int NextAutoID()
    {
        return s_NextAutoID++;
    }

private:
    static int s_NextAutoID;
};