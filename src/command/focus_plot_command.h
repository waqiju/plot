#pragma once


class FocusPlotCommand
{
public:
    FocusPlotCommand(float begin, float end);
    void Execute();

private:
    float m_Begin, m_End;
};
