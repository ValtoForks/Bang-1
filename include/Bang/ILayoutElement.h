#ifndef ILAYOUTELEMENT_H
#define ILAYOUTELEMENT_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/IInvalidatable.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/Vector.tcc"
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

class ILayoutElement : public IInvalidatable<ILayoutElement>
{
public:

    //IInvalidatable
    virtual void Invalidate() override;
    virtual void OnInvalidated() override;

    void SetLayoutPriority(int layoutPriority);
    int GetLayoutPriority() const;

    Vector2i GetMinSize()       const;
    Vector2i GetPreferredSize() const;
    Vector2  GetFlexibleSize()  const;

    Vector2 GetSize(LayoutSizeType sizeType) const;

protected:
    ILayoutElement();
    virtual ~ILayoutElement();

    void SetCalculatedLayout(Axis axis,
                             int min,
                             int preferred  = -1,
                             float flexible = -1.0f);

private:
    int m_layoutPriority = 0;
    mutable Vector2i m_calculatedMinSize       = -Vector2i::One;
    mutable Vector2i m_calculatedPreferredSize = -Vector2i::One;
    mutable Vector2  m_calculatedFlexibleSize  = -Vector2::One;

    virtual void CalculateLayout(Axis axis) = 0;
    void _CalculateLayout(Axis axis);

    friend class UILayoutManager;
};

NAMESPACE_BANG_END

#endif // ILAYOUTELEMENT_H
