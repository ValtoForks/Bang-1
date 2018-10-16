#ifndef LIGHT_H
#define LIGHT_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/Component.h"
#include "Bang/ComponentClassIds.h"
#include "Bang/ComponentMacros.h"
#include "Bang/MetaNode.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class GameObject;
FORWARD class GBuffer;
FORWARD class ICloneable;
FORWARD class Renderer;
FORWARD class ShaderProgram;
FORWARD class Texture;

class Light : public Component
{
    COMPONENT_WITH_FAST_DYNAMIC_CAST_ABSTRACT(Light)

public:
    enum class ShadowType { NONE = 0, HARD, SOFT };

    void SetColor(const Color &color);
    void SetIntensity(float intensity);
    void SetShadowBias(float shadowBias);
    void SetShadowType(ShadowType shadowType);
    void SetShadowMapSize(const Vector2i &shadowMapSize);

    Color GetColor() const;
    float GetIntensity() const;
    float GetShadowBias() const;
    ShadowType GetShadowType() const;
    const Vector2i& GetShadowMapSize() const;
    virtual Texture *GetShadowMapTexture() const;

    void RenderShadowMaps(GameObject *go);

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;

protected:
    Light();
    virtual ~Light();

    void SetLightScreenPassShaderProgram(ShaderProgram* sp);
    Array<Renderer*> GetShadowCastersIn(GameObject *go) const;
    virtual void SetUniformsBeforeApplyingLight(ShaderProgram* sp) const;

private:
    float m_intensity = 1.0f;
    Color m_color = Color::White;

    float m_shadowBias = 0.003f;
    Vector2i m_shadowMapSize = Vector2i(256);
    ShadowType m_shadowType = ShadowType::HARD;

    RH<ShaderProgram> p_lightScreenPassShaderProgram;

    void ApplyLight(Camera *camera, const AARect &renderRect) const;
    virtual AARect GetRenderRect(Camera *camera) const;
    virtual void RenderShadowMaps_(GameObject *go) = 0;

    friend class GEngine;
};

NAMESPACE_BANG_END

#endif // LIGHT_H
