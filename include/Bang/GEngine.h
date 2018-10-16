#ifndef GENGINE_H
#define GENGINE_H

#include <functional>
#include <vector>

#include "Bang/AARect.h"
#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/Flags.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/Light.h"
#include "Bang/Map.tcc"
#include "Bang/MultiObjectGatherer.h"
#include "Bang/ObjectGatherer.tcc"
#include "Bang/ReflectionProbe.h"
#include "Bang/RenderPass.h"
#include "Bang/ResourceHandle.h"
#include "Bang/StackAndValue.h"
#include "Bang/USet.h"

NAMESPACE_BANG_BEGIN

FORWARD   class Camera;
FORWARD   class DebugRenderer;
FORWARD_T class EventEmitter;
FORWARD   class Framebuffer;
FORWARD   class GameObject;
FORWARD   class GBuffer;
FORWARD   class GL;
FORWARD   class IEventsDestroy;
FORWARD   class Light;
FORWARD   class Material;
FORWARD   class Mesh;
FORWARD   class ReflectionProbe;
FORWARD   class RenderFactory;
FORWARD   class Renderer;
FORWARD   class Scene;
FORWARD   class ShaderProgram;
FORWARD   class Texture2D;
FORWARD   class Texture;
FORWARD   class TextureCubeMap;
FORWARD   class TextureUnitManager;

class GEngine : public EventListener<IEventsDestroy>
{
public:
    using RenderRoutine = std::function<void(Renderer*)>;

    GEngine();
    virtual ~GEngine();

    void Init();

    void Render(Scene *scene);
    void Render(Scene *scene, Camera *camera);
    void RenderTexture(Texture2D *texture);
    void RenderTexture(Texture2D *texture, float gammaCorrection);
    void RenderWithPass(GameObject *go,
                        RenderPass renderPass,
                        bool renderChildren = true);
    void RenderViewportRect(ShaderProgram *sp,
                            const AARect &destRectMask = AARect::NDCRect);
    void RenderToGBuffer(GameObject *go, Camera *camera);
    void RenderViewportPlane();

    void ApplyGammaCorrection(GBuffer *gbuffer,
                              float gammaCorrection);

    void SetReplacementMaterial(Material *material);
    void SetRenderRoutine(RenderRoutine renderRoutine);

    void PushActiveRenderingCamera();
    void SetActiveRenderingCamera(Camera *camera);
    void PopActiveRenderingCamera();

    void FillCubeMapFromTextures(TextureCubeMap *texCMToFill,
                                 Texture2D *topTexture,
                                 Texture2D *botTexture,
                                 Texture2D *leftTexture,
                                 Texture2D *rightTexture,
                                 Texture2D *frontTexture,
                                 Texture2D *backTexture,
                                 int mipMapLevel = 0);

    static GBuffer *GetActiveGBuffer();

    DebugRenderer *GetDebugRenderer() const;
    RenderFactory *GetRenderFactory() const;
    Material* GetReplacementMaterial() const;
    static Camera *GetActiveRenderingCamera();
    const Array<ReflectionProbe*>& GetReflectionProbesFor(Scene *scene) const;

    GL *GetGL() const;
    TextureUnitManager *GetTextureUnitManager() const;


    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    static GEngine* GetInstance();

private:
    GL *m_gl = nullptr;
    DebugRenderer *m_debugRenderer = nullptr;
    RenderFactory *m_renderFactory = nullptr;
    TextureUnitManager *m_texUnitManager = nullptr;

    MultiObjectGatherer<ReflectionProbe, true> m_reflProbesCache;
    MultiObjectGatherer<Light, true> m_lightsCache;

    RenderRoutine m_renderRoutine;
    StackAndValue<Camera*> p_renderingCameras;
    USet<Camera*> m_stackedCamerasThatHaveBeenDestroyed;

    RH<ShaderProgram> m_renderSkySP;
    RH<Material> m_replacementMaterial;
    RH<ShaderProgram> m_fillCubeMapFromTexturesSP;
    Framebuffer *m_fillCubeMapFromTexturesFB = nullptr;

    // Forward rendering arrays
    bool m_currentlyForwardRendering = false;
    Array<int> m_currentForwardRenderingLightTypes;
    Array<Color> m_currentForwardRenderingLightColors;
    Array<Vector3> m_currentForwardRenderingLightPositions;
    Array<Vector3> m_currentForwardRenderingLightForwardDirs;
    Array<float> m_currentForwardRenderingLightIntensities;
    Array<float> m_currentForwardRenderingLightRanges;

    RH<Mesh> p_windowPlaneMesh;
    RH<ShaderProgram> p_renderTextureToViewportSP;
    RH<ShaderProgram> p_renderTextureToViewportGammaSP;

    void Render(Renderer *rend);
    void RenderShadowMaps(GameObject *go);
    void RenderTexture_(Texture2D *texture, float gammaCorrection);
    void RenderReflectionProbes(GameObject *go);
    void RenderWithAllPasses(GameObject *go);
    void RenderTransparentPass(GameObject *go);
    void RenderWithPassAndMarkStencilForLights(GameObject *go, RenderPass renderPass);
    bool CanRenderNow(Renderer *rend, RenderPass renderPass) const;

    void ApplyStenciledDeferredLightsToGBuffer(
                                GameObject *lightsContainer,
                                Camera *camera,
                                const AARect &maskRectNDC = AARect::NDCRect);

    void RetrieveForwardRenderingInformation(GameObject *goToRender);
    void PrepareForForwardRendering(Renderer *rend);

    friend class Gizmos;
    friend class Window;
    friend class Renderer;
};

NAMESPACE_BANG_END

#endif // GENGINE_H
