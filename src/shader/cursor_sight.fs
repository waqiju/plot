#version 330 core

in vec2 viewportPosition;
in float direction;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform vec4 ColorTint;
uniform float ScreenWidth;
uniform float ScreenHeight;


void main(){


    vec2 screenPos = vec2((viewportPosition.x + 1) * 0.5 * ScreenWidth, (viewportPosition.y + 1) * 0.5 * ScreenHeight);

    // 方格 Clip
    // screenPos.xy = floor(screenPos.xy / 4) * 0.5;
    // float checker = fract(screenPos.x + screenPos.y);
    // if (checker > 0)
    //     discard;

    /* 
        如果采用方格 Clip，在上下左右移动的时候，线段会有闪烁的效果。
        这对于使用造成了干扰。
        所以分垂直和竖直两个方向分别 Clip。
    */
    // 横向
    if (direction >= 0 && int(screenPos.x) % 5 > 1)
        discard;
    // 竖向
    if (direction < 0 && int(screenPos.y) % 5 > 1)
        discard;

    color = ColorTint;
	
}