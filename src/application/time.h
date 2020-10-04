#pragma once


class Time
{
public:
    static float NowTime() { return s_NowTime; }
    static float DeltaTime() { return s_DeltaTime; }
    static unsigned int FrameCount() { return s_FrameCount; }

    static void OnFrameBegin();

private:
    static float s_NowTime;
    static float s_DeltaTime;
    static unsigned int s_FrameCount;
};