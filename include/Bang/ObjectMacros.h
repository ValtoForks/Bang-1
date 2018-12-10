#ifndef OBJECTMACROS_H
#define OBJECTMACROS_H

#include "Bang/ObjectClassIds.h"

namespace Bang
{
#define OBJECT_WITH_FAST_DYNAMIC_CAST_EXPLICIT(CLASS, CIDBegin, CIDEnd) \
    SET_CLASS_ID(CLASS, CIDBegin, CIDEnd)

#define OBJECT_WITH_FAST_DYNAMIC_CAST(CLASS) \
    OBJECT_WITH_FAST_DYNAMIC_CAST_EXPLICIT(  \
        CLASS, CLASS##CIDBegin, CLASS##CIDEnd)
}

#endif  // OBJECTMACROS_H
