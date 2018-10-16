#ifndef TEXTUREUNITMANAGER_H
#define TEXTUREUNITMANAGER_H

#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GL.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/Map.h"

NAMESPACE_BANG_BEGIN

FORWARD_T class EventEmitter;
FORWARD   class IEventsDestroy;
FORWARD   class Texture;

class TextureUnitManager : public EventListener<IEventsDestroy>
{
public:
    using TexUnit = uint;
    TextureUnitManager();

    static void BindTextureToUnit(Texture *texture, TexUnit unit);
    static TexUnit BindTextureToUnit(Texture *texture);

    static int GetMaxTextureUnits();
    static int GetNumUsableTextureUnits();
    static void UnBindAllTexturesFromAllUnits();

    static int GetUnitTextureIsBoundTo(Texture *texture);
    static GLId GetBoundTextureToUnit(GL::TextureTarget texTarget,
                                      GL::Enum textureUnit);
    static void PrintTextureUnits();
    static TextureUnitManager* GetActive();


private:
    int m_numMaxTextureUnits    = -1;
    int m_numUsableTextureUnits = -1;
    TexUnit m_voidTexUnit       = -1;

    uint m_timestampCounter = 0;
    std::queue<TexUnit> m_freeUnits;
    std::unordered_map<GLId, uint> m_timestampTexIdUsed;
    std::unordered_map<GLId, Texture*> m_textureIdToTexture;
    std::unordered_map<GLId, TexUnit> m_textureIdToBoundUnit;

    TexUnit MakeRoomAndGetAFreeTextureUnit();
    void UpdateStructuresForUsedTexture(Texture *texture, uint usedUnit);
    void UnTrackTexture(GLId textureId);
    void CheckBindingsValidity() const;

    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_END

#endif // TEXTUREUNITMANAGER_H
