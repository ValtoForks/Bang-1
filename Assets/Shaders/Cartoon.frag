#include "ScreenPass.frag"

void main()
{
    vec4 outputColor = B_SampleColor();

    float originalValue = length(outputColor);
    float toonValue = round(originalValue * 8) / 8.0;
    outputColor = normalize(outputColor) * toonValue;

    float valueFactor = 3.0;
    float contrast = 1.3;
    outputColor = pow(outputColor * valueFactor, vec4(contrast)) / valueFactor;

    B_GIn_Color = outputColor;
}
