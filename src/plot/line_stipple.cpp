#include "line_stipple.h"
#include "camera_helper.h"
#include "application/application.h"
#include "camera_helper.h"


/*
TODO 虚线的实现方式探讨

目前的实现方式，CPU 端对 Mesh 拆点。模拟在顺着顶点列表行走，把每段按照单像素的大小切分为更小的片段。

更高效的实现一，CPU 端记录顶点的走过距离，传递给 GPU，在片元着色器里根据 (当前像素对应的距离，pattern, 像素大小) 决定是否 visible。
这样不用拆点，顶点更少，GPU端的开销更少。

前两种都是只渲染一遍的实现方式，需要依赖像素大小固定，不具有通用性。当前是固定在原点的xy屏幕，像素大小是固定的。

更高效的实现二，渲染两遍。第一遍按照连续线段渲染，第二遍按照实际像素遍历挨个剔除。剔除算法需要适当的设计，暂时没有去细化思考。
*/

const int kPatternBitCount = 16;
const int kPatternBitStart = kPatternBitCount - 1;


void ApplyLineStipple(const std::vector<Vector3>& source, const std::vector<Color>& sourceColor, std::vector<Vector3>& destination, std::vector<Color>& destinationColor, int pattern)
{
    // 首像素保证可见
    pattern |= 1 << kPatternBitStart;

    auto sourceSize = source.size();
    if (sourceSize <= 1)
    {
        destination = source;
        destinationColor = sourceColor;
        return;
    }

    float currentLineLength = 0;
    destination.clear();
    destinationColor.clear();

    float pixelSize = CameraHelper::OnePixelSizeInWorld(Application::MainCamera(), Application::screenHeight);
    float currentPixelResidual = pixelSize;
    int currentStipplePatternIndex = kPatternBitStart;
    bool currentPixelVisible = (pattern & (1 << currentStipplePatternIndex)) > 0;

    auto mainCamera = Application::MainCamera();
    for (int i = 0; i < sourceSize; i += 2)
    {
        Vector3 lineStart = source[i];
        Vector3 lineEnd = source[i+1];
        Vector3 center = (lineStart + lineEnd) * 0.5f;
        // 简单粗暴的优化，为了防止拆点导致的无限细分虚线消耗性能，剔除掉部分线段
        // 如果虚线改为非拆点方式实现，可以去掉这个优化
        if (!CameraHelper::IsPositionVisible(mainCamera, center))
        {
            continue;
        }

        Color colorStart = sourceColor[i];
        Color colorEnd = sourceColor[i+1];
        currentLineLength += (lineEnd - lineStart).Length();

        // 跨像素，需要拆点
        Vector3 previousVertex = lineStart;
        Color previousColor = colorStart;
        while (currentLineLength >= currentPixelResidual)
        {
            Vector3 middle = Vector3::Lerp(previousVertex, lineEnd, currentPixelResidual / currentLineLength);
            Color middleColor = Color::Lerp(previousColor, colorEnd, currentPixelResidual / currentLineLength);
            if (currentPixelVisible)
            {
                destination.push_back(previousVertex);
                destination.push_back(middle);
                destinationColor.push_back(previousColor);
                destinationColor.push_back(middleColor);
            }
            // 换到下个像素
            currentLineLength -= currentPixelResidual;
            previousVertex = middle;

            currentStipplePatternIndex = (currentStipplePatternIndex - 1 + kPatternBitCount) % kPatternBitCount;
            currentPixelVisible = (pattern & (1 << currentStipplePatternIndex)) > 0;
            currentPixelResidual = pixelSize;
        }
        if (currentLineLength > 0 && currentLineLength < currentPixelResidual)
        {
            if (currentPixelVisible)
            {
                destination.push_back(previousVertex);
                destination.push_back(lineEnd);
                destinationColor.push_back(previousColor);
                destinationColor.push_back(colorEnd);
            }
            currentPixelResidual -= currentLineLength;
            currentLineLength = 0;
        }
    }
}