#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Bang/Light.h"
#include "Bang/Matrix4.h"
#include "Bang/Texture2D.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Framebuffer;

class DirectionalLight : public Light
{
    COMPONENT(DirectionalLight)

public:
    void SetShadowDistance(float shadowDistance);
    float GetShadowDistance() const;

    // Light
    Texture2D* GetShadowMapTexture() const override;

    // Component
    void OnRender(RenderPass rp) override;

    // Light
    void SetUniformsBeforeApplyingLight(ShaderProgram *sp) const override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Framebuffer *m_shadowMapFramebuffer = nullptr;
    Matrix4 m_lastUsedShadowMapViewProj = Matrix4::Identity;
    float m_shadowDistance = 100.0f;

    DirectionalLight();
    virtual ~DirectionalLight();

    // Light
    void RenderShadowMaps_() override;

    void GetWorldToShadowMapMatrices(Scene *scene,
                                     Matrix4 *viewMatrix,
                                     Matrix4 *projMatrix) const;
    Matrix4 GetShadowMapMatrix(Scene *scene) const;
    Matrix4 GetLightToWorldMatrix() const;
    AABox GetShadowMapOrthoBox(Scene *scene) const;
};

NAMESPACE_BANG_END

#endif // DIRECTIONALLIGHT_H
