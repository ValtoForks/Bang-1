#ifndef BANGFAB_H
#define BANGFAB_H

#include "Bang/Asset.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

class Prefab : public Asset
{
    ASSET(Prefab)

public:
    GameObject* Instantiate() const;
    GameObject* InstantiateRaw() const;

    void SetGameObject(GameObject *go);

    const String& GetMetaContent() const;

    // Resource
    void Import(const Path& prefabFilepath) override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;

private:
    String m_gameObjectMetaInfoContent = "";

    Prefab();
    Prefab(GameObject *go);
    Prefab(const String &gameObjectMetaInfoContent);
    virtual ~Prefab();
};

NAMESPACE_BANG_END

#endif // BANGFAB_H
