#include "Bang/Light.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/AARect.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

Light::Light() { }
Light::~Light() { }

void Light::SetColor(const Color &color) { m_color = color; }
void Light::SetIntensity(float intensity) { m_intensity = intensity; }
void Light::SetShadowBias(float shadowBias) { m_shadowBias = shadowBias; }
void Light::SetShadowType(ShadowType shadowType) { m_shadowType = shadowType; }
void Light::SetShadowMapSize(const Vector2i &shadowMapSize)
{
    m_shadowMapSize = shadowMapSize;
}

Color Light::GetColor() const { return m_color; }
float Light::GetIntensity() const { return m_intensity; }
float Light::GetShadowBias() const { return m_shadowBias; }
Light::ShadowType Light::GetShadowType() const { return m_shadowType; }
const Vector2i &Light::GetShadowMapSize() const { return m_shadowMapSize; }
Texture *Light::GetShadowMapTexture() const { return nullptr; }

void Light::RenderShadowMaps()
{
    if (GetShadowType() != ShadowType::NONE)
    {
        RenderShadowMaps_();
    }
}

void Light::ApplyLight(Camera *camera, const AARect &renderRect) const
{
    p_lightMaterial.Get()->Bind();
    SetUniformsBeforeApplyingLight(p_lightMaterial.Get()->GetShaderProgram());

    // Intersect with light rect to draw exactly what we need
    GBuffer *gbuffer = camera->GetGBuffer();
    AARect improvedRenderRect = AARect::Intersection(GetRenderRect(camera),
                                                 renderRect);
    gbuffer->ApplyPass(p_lightMaterial.Get()->GetShaderProgram(),
                       true, improvedRenderRect);
    p_lightMaterial.Get()->UnBind();
}

void Light::SetUniformsBeforeApplyingLight(ShaderProgram* sp) const
{
    ASSERT(GL::IsBound(sp))

    Transform *tr = GetGameObject()->GetTransform();
    sp->Set("B_LightShadowType",    SCAST<int>( GetShadowType() ), false);
    sp->Set("B_LightShadowBias",    GetShadowBias(),               false);
    sp->Set("B_LightIntensity",     GetIntensity(),                false);
    sp->Set("B_LightColor",         GetColor(),                    false);
    sp->Set("B_LightForwardWorld",  tr->GetForward(),              false);
    sp->Set("B_LightPositionWorld", tr->GetPosition(),             false);
    sp->Set("B_LightShadowMap",     GetShadowMapTexture(),         false);
    sp->Set("B_LightShadowMapSoft", GetShadowMapTexture(),         false);
}

void Light::SetLightMaterial(Material *lightMat)
{
    p_lightMaterial.Set(lightMat);
}

AARect Light::GetRenderRect(Camera *camera) const
{
    return AARect::NDCRect;
}

void Light::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Light *l = Cast<Light*>(clone);
    l->SetIntensity(GetIntensity());
    l->SetColor(GetColor());
    l->SetShadowBias( GetShadowBias() );
    l->SetShadowType( GetShadowType() );
    l->SetShadowMapSize( GetShadowMapSize() );
}

void Light::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Intensity"))
    { SetIntensity(xmlInfo.Get<float>("Intensity")); }

    if (xmlInfo.Contains("Color"))
    { SetColor(xmlInfo.Get<Color>("Color")); }

    if (xmlInfo.Contains("ShadowBias"))
    { SetShadowBias(xmlInfo.Get<float>("ShadowBias")); }

    if (xmlInfo.Contains("ShadowType"))
    { SetShadowType(xmlInfo.Get<ShadowType>("ShadowType")); }

    if (xmlInfo.Contains("ShadowMapSize"))
    { SetShadowMapSize(xmlInfo.Get<Vector2i>("ShadowMapSize")); }
}

void Light::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("Intensity", GetIntensity());
    xmlInfo->Set("Color", GetColor());
    xmlInfo->Set("ShadowBias", GetShadowBias());
    xmlInfo->Set("ShadowType", GetShadowType());
    xmlInfo->Set("ShadowMapSize", GetShadowMapSize());
}
