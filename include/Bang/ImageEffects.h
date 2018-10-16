#ifndef IMAGEEFFECTS_H
#define IMAGEEFFECTS_H

#include "Bang/BangDefines.h"
#include "Bang/Image.h"
#include "Bang/ImageIO.h"

NAMESPACE_BANG_BEGIN

class ImageEffects
{
public:

    // Zero  Color (0,0,0,0): background
    // Other Color:           foreground
    static void SignedDistanceField(const Imageb &inputImageBW,
                                    Imageb *distanceFieldOutputImage,
                                    int distanceRadius);

    // Zero  Color (0,0,0,0): background
    // Other Color:           foreground
    static void Outline(const Imageb &inputImageBW,
                        Imageb *outlineOutputImageBW);

    static bool Albedo(const Imageb &inputImage1,
                     const Imageb &inputImage2,
                     Imageb *diffOutputImage,
                     bool diffAlpha = false);

    ImageEffects() = delete;
};

NAMESPACE_BANG_END

#endif // DISTANCEFIELDER_H
