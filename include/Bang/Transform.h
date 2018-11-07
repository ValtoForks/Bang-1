#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/Component.h"
#include "Bang/ComponentClassIds.h"
#include "Bang/ComponentMacros.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEventsChildren.h"
#include "Bang/IEventsTransform.h"
#include "Bang/IInvalidatable.h"
#include "Bang/Matrix4.h"
#include "Bang/MetaNode.h"
#include "Bang/String.h"

namespace Bang
{
class GameObject;
class ICloneable;

class IInvalidatableTransformWorld
    : public IInvalidatable<IInvalidatableTransformWorld>
{
public:
    void OnInvalidated() override
    {
        OnInvalidatedWorld();
    }
    virtual void OnInvalidatedWorld() = 0;
};
class IInvalidatableTransformLocal
    : public IInvalidatable<IInvalidatableTransformLocal>
{
public:
    void OnInvalidated() override
    {
        OnInvalidatedLocal();
    }
    virtual void OnInvalidatedLocal() = 0;
};

class Transform : public Component,
                  public IInvalidatableTransformWorld,
                  public IInvalidatableTransformLocal,
                  public EventListener<IEventsTransform>,
                  public EventListener<IEventsChildren>,
                  public EventEmitter<IEventsTransform>
{
    COMPONENT_WITH_FAST_DYNAMIC_CAST(Transform)

public:
    void LookAt(const Vector3 &target, const Vector3 &up = Vector3::Up());
    void LookAt(Transform *targetTransform, const Vector3 &up = Vector3::Up());
    void LookAt(GameObject *target, const Vector3 &up = Vector3::Up());
    void LookInDirection(const Vector3 &dir, const Vector3 &up = Vector3::Up());

    void FillFromMatrix(const Matrix4 &transformMatrix);

    void SetLocalPosition(const Vector3 &p);
    void SetPosition(const Vector3 &p);
    void TranslateLocal(const Vector3 &translation);
    void Translate(const Vector3 &translation);

    void SetLocalRotation(const Quaternion &r);
    void SetLocalEuler(const Vector3 &degreesEuler);
    void SetLocalEuler(float x, float y, float z);

    void SetRotation(const Quaternion &r);
    void SetEuler(const Vector3 &degreesEuler);
    void SetEuler(float x, float y, float z);

    void RotateLocal(const Quaternion &r);
    void RotateLocalEuler(const Vector3 &degreesEuler);
    void Rotate(const Quaternion &r);
    void RotateEuler(const Vector3 &degreesEuler);

    void SetScale(float s);
    void SetScale(const Vector3 &v);
    void SetLocalScale(float s);
    void SetLocalScale(const Vector3 &s);

    Vector3 TransformPoint(const Vector3 &point) const;
    Vector3 InverseTransformPoint(const Vector3 &point) const;
    Vector3 TransformDirection(const Vector3 &dir) const;
    Vector3 InverseTransformDirection(const Vector3 &dir) const;
    Vector3 TransformVector(const Vector3 &vector) const;
    Vector3 InverseTransformVector(const Vector3 &vector) const;

    Vector3 FromLocalToWorldPoint(const Vector3 &point) const;
    Vector3 FromLocalToWorldVector(const Vector3 &vector) const;
    Vector3 FromLocalToWorldDirection(const Vector3 &dir) const;
    Vector3 FromWorldToLocalPoint(const Vector3 &point) const;
    Vector3 FromWorldToLocalVector(const Vector3 &vector) const;
    Vector3 FromWorldToLocalDirection(const Vector3 &dir) const;

    virtual const Matrix4 &GetLocalToParentMatrix() const;
    virtual const Matrix4 &GetLocalToParentMatrixInv() const;
    virtual const Matrix4 &GetLocalToWorldMatrix() const;
    virtual const Matrix4 &GetLocalToWorldMatrixInv() const;

    Vector3 GetForward() const;
    Vector3 GetBack() const;
    Vector3 GetRight() const;
    Vector3 GetLeft() const;
    Vector3 GetUp() const;
    Vector3 GetDown() const;

    const Vector3 &GetLocalPosition() const;
    Vector3 GetPosition() const;
    const Quaternion &GetLocalRotation() const;
    Quaternion GetRotation() const;
    const Vector3 &GetLocalEuler() const;
    Vector3 GetEuler() const;
    const Vector3 &GetLocalScale() const;
    Vector3 GetScale() const;

    // IEventsChildren
    void OnParentChanged(GameObject *oldParent, GameObject *newParent) override;

    // IEventsTransform
    void OnTransformChanged() override;
    void OnParentTransformChanged() override;
    void OnChildrenTransformChanged() override;

    // Serializable
    virtual void Reflect() override;

    virtual void InvalidateTransform();

protected:
    mutable Matrix4 m_localToWorldMatrix;
    mutable Matrix4 m_localToWorldMatrixInv;
    mutable Matrix4 m_localToParentMatrix;
    mutable Matrix4 m_localToParentMatrixInv;

    Transform();
    virtual ~Transform() override;

    // IInvalidatable
    void OnInvalidatedWorld() override;
    void OnInvalidatedLocal() override;
    virtual void OnTransformInvalidated();

    void RecalculateParentMatricesIfNeeded() const;
    void RecalculateWorldMatricesIfNeeded() const;
    virtual void CalculateLocalToParentMatrix() const;
    virtual void CalculateLocalToWorldMatrix() const;

    virtual bool CanBeRepeatedInGameObject() const override;

private:
    Vector3 m_localPosition = Vector3::Zero();
    Quaternion m_localRotation = Quaternion::Identity();
    Vector3 m_localEulerAnglesDegreesHint = Vector3::Zero();
    Vector3 m_localScale = Vector3::One();

    void PropagateParentTransformChangedEventToChildren() const;
    void PropagateChildrenTransformChangedEventToParent() const;
};
}  // namespace Bang

#endif  // TRANSFORM_H
