#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/ComponentClassIds.h"
#include "Bang/ComponentMacros.h"
#include "Bang/Light.h"
#include "Bang/MetaNode.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "Bang/TextureCubeMap.h"

namespace Bang
{
class Camera;
class Framebuffer;
class GameObject;
class ICloneable;
class Material;
class ShaderProgram;

class PointLight : public Light
{
    COMPONENT_WITH_FAST_DYNAMIC_CAST(PointLight)

public:
    void SetRange(float range);

    float GetRange() const;

    // Light
    TextureCubeMap *GetShadowMapTexture() const override;

    // Serializable
    void Reflect() override;

protected:
    float m_range = 1.0f;
    RH<Material> m_shadowMapMaterial;
    Framebuffer *m_shadowMapFramebuffer = nullptr;

    PointLight();
    virtual ~PointLight() override;

    float GetLightZFar() const;

    // Light
    void RenderShadowMaps_(GameObject *go) override;
    Array<Matrix4> GetWorldToShadowMapMatrices() const;
    virtual void SetUniformsBeforeApplyingLight(
        ShaderProgram *sp) const override;

    // Renderer
    AARect GetRenderRect(Camera *cam) const override;
};
}

#endif  // POINTLIGHT_H
