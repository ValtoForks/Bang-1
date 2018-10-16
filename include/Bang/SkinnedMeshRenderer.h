#ifndef SKINNEDMESHRENDERER_H
#define SKINNEDMESHRENDERER_H

#include <functional>
#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/ComponentClassIds.h"
#include "Bang/ComponentMacros.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsName.h"
#include "Bang/Map.h"
#include "Bang/MeshRenderer.h"
#include "Bang/MetaNode.h"
#include "Bang/ObjectGatherer.h"
#include "Bang/Set.h"
#include "Bang/String.h"
#include "Bang/UMap.h"

NAMESPACE_BANG_BEGIN

FORWARD   class GameObject;
FORWARD   class ICloneable;
FORWARD   class IEventsName;
FORWARD   class Model;
FORWARD   class ShaderProgram;
FORWARD   template <class ObjectType, bool RECURSIVE> class ObjectGatherer;
FORWARD_T class IEventsObjectGatherer;

class SkinnedMeshRenderer : public MeshRenderer,
                            public EventListener<IEventsName>,
                            public EventListener< IEventsObjectGatherer<GameObject> >
{
    COMPONENT_WITH_FAST_DYNAMIC_CAST(SkinnedMeshRenderer)

public:
	SkinnedMeshRenderer();
	virtual ~SkinnedMeshRenderer();

    // MeshRenderer
    void OnRender() override;
    virtual void Bind() override;
    virtual void SetUniformsOnBind(ShaderProgram *sp) override;
    Matrix4 GetModelMatrixUniform() const override;

    void SetRootBoneGameObjectName(const String &rootBoneGameObjectName);

    Model *GetActiveModel() const;
    GameObject *GetRootBoneGameObject() const;
    const String& GetRootBoneGameObjectName() const;
    GameObject *GetBoneGameObject(const String &boneName) const;
    Matrix4 GetBoneSpaceToRootSpaceMatrix(const String &boneName) const;
    Matrix4 GetBoneTransformMatrixFor(
                GameObject *boneGameObject,
                const Matrix4 &transform,
                UMap<GameObject*, Matrix4> *boneTransformInRootSpaceCache) const;
    const Matrix4& GetInitialTransformMatrixFor(const String &boneName) const;
    const Map<String, Matrix4> &GetInitialTransforms() const;
    const Set<String> &GetBonesNames() const;

    void SetBoneUniforms(ShaderProgram *sp);
    void UpdateBonesMatricesFromTransformMatrices();
    void UpdateTransformMatricesFromInitialBonePosition();
    void SetSkinnedMeshRendererCurrentBoneMatrices(
                                    const Map<String, Matrix4> &boneMatrices);
    void SetSkinnedMeshRendererCurrentBoneMatrices(
                                    const Array<Matrix4> &boneMatrices);

    void RetrieveBonesBindPoseFromCurrentHierarchy();

    // ObjectGatherer
    virtual void OnObjectGathered(GameObject *go) override;
    virtual void OnObjectUnGathered(GameObject *previousGameObject,
                                    GameObject *go) override;

    // IEventsName
    virtual void OnNameChanged(GameObject *go,
                               const String &oldName,
                               const String &newName) override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;

private:
    Set<String> m_bonesNames;
    Map<String, Matrix4> m_initialTransforms;
    Map<String, Matrix4> m_boneSpaceToRootSpaceMatrices;
    Array<Matrix4> m_bonesTransformsMatricesArrayUniform;

    ObjectGatherer<GameObject, true> *m_gameObjectGatherer = nullptr;

    String m_rootBoneGameObjectName = "";
    mutable GameObject *p_rootBoneGameObject = nullptr;
    mutable Map<String, GameObject*> m_boneNameToBoneGameObject;


};

NAMESPACE_BANG_END

#endif // SKINNEDMESHRENDERER_H

