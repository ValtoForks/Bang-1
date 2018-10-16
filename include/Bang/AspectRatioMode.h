#ifndef ASPECTRATIOMODE
#define ASPECTRATIOMODE

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Math.h"
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

enum class AspectRatioMode { IGNORE, KEEP, KEEP_EXCEEDING};


namespace AspectRatio
{
    Vector2i GetAspectRatioedSize(const Vector2i &currentSize,
                                  const Vector2i &targetSize,
                                  AspectRatioMode aspectRatioMode);
}

NAMESPACE_BANG_END

#endif
