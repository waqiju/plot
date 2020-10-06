#pragma once
#include <map>

class Vector3;

class Input
{
public:
    static bool GetKey(int key);
    static bool GetKeyDown(int key);
    static bool GetKeyUp(int key);
    static bool GetMouseButton(int button);
    static bool GetMouseButtonDown(int button);
    static bool GetMouseButtonUp(int button);
    static Vector3 MousePosition();

    // 更新所有 Key 的上一帧 IsDown 状态
    static void OnFrameEnd();

private:
    static std::map<int, bool> s_PreviousKeyDown;
    static std::map<int, bool> s_PreviousMouseButtonDown;
};