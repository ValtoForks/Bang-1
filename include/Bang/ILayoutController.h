#ifndef ILAYOUTCONTROLLER_H
#define ILAYOUTCONTROLLER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsChildren.h"
#include "Bang/IEventsTransform.h"
#include "Bang/IInvalidatable.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class IEventsChildren;
FORWARD class IEventsTransform;

class ILayoutController : public IInvalidatable<ILayoutController>,
                          public EventListener<IEventsChildren>,
                          public EventListener<IEventsTransform>
{
public:
    virtual bool IsSelfController() const;

    // IInvalidatable
    virtual void Invalidate() override;
    virtual void OnInvalidated() override;

    // IEventsChildren
    void OnChildAdded(GameObject *addedChild, GameObject *parent) override;
    void OnChildRemoved(GameObject *removedChild, GameObject *parent) override;
    void OnParentChanged(GameObject *oldParent, GameObject *newParent) override;

    // IEventsTransform
    virtual void OnTransformChanged() override;
    virtual void OnParentTransformChanged() override;

protected:
    ILayoutController();
    virtual ~ILayoutController();

private:
    virtual void ApplyLayout(Axis axis) = 0;
    void _ApplyLayout(Axis axis);

    friend class UILayoutManager;
};

NAMESPACE_BANG_END

#endif // ILAYOUTCONTROLLER_H
