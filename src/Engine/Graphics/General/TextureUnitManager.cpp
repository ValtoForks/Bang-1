#include "Bang/TextureUnitManager.h"

#include "Bang/GL.h"
#include "Bang/Texture.h"
#include "Bang/GEngine.h"

USING_NAMESPACE_BANG

TextureUnitManager::TextureUnitManager()
{
    m_numTextureUnits = GL::GetInteger(GL::MaxTextureImageUnits);
    // m_numTextureUnits = 8; // Uncomment to stress test with min tex units
}

TextureUnitManager::TexUnit
TextureUnitManager::BindTextureToUnit(Texture *texture)
{
    TextureUnitManager *tm = TextureUnitManager::GetActive();

    TexUnit texUnit;
    const GLId texId = texture->GetGLId();
    const bool texIsAlreadyInUnit = (tm->m_textureIdToBoundUnit.count(texId) == 1);
    if (texIsAlreadyInUnit)
    {
        // Texture was already bound to a unit. Return the unit it was bound to.
        TexUnit prevTexUnit = tm->m_textureIdToBoundUnit[texId];
        texUnit = prevTexUnit;
    }
    else
    {
        // We will have to bind the texture to some unit.
        const TexUnit freeUnit = tm->MakeRoomAndGetAFreeTextureUnit();

        // Bind to texture unit
        GL::ActiveTexture(GL_TEXTURE0 + freeUnit);
        texture->Bind();

        // GL_TEXTURE0 is the void unit, so that subsequent binds for some other
        // reason dont change the current active texture unit.
        // In other words, without this line, the previous unit is bound, and
        // any texture bind for some other reason will overwrite the unit, and
        // we do not want it
        GL::ActiveTexture(GL_TEXTURE0);

        // Update number of times used
        tm->UpdateStructuresForUsedTexture(texture, freeUnit);

        texUnit = freeUnit;
    }

    tm->m_timestampTexIdUsed[texId] = tm->m_timestampCounter;
    ++tm->m_timestampCounter;

    return texUnit;
}

TextureUnitManager::TexUnit TextureUnitManager::MakeRoomAndGetAFreeTextureUnit()
{
    TexUnit freeUnit = -1;
    uint currentNumBoundTextures = m_textureIdToBoundUnit.size();
    if (currentNumBoundTextures < m_numTextureUnits)
    {
        // If there is enough space, allocate to free unit
        // +1 to avoid using unit GL_TEXTURE0
        freeUnit = currentNumBoundTextures + 1;
    }
    else
    {
        // Otherwise, we will have to make some room.
        // Unallocate one unit so that we can put the new texture there
        ASSERT(m_timestampTexIdUsed.size() >= 1);

        // Find the oldest used texture id
        GLId oldestUsedTextureId        = m_timestampTexIdUsed.begin()->first;
        uint oldestUsedTextureTimestamp = m_timestampTexIdUsed.begin()->second;
        for (const auto &pair : m_timestampTexIdUsed)
        {
            const GLId texId = pair.first;
            const uint texUseTimestamp = pair.second;
            if (texUseTimestamp < oldestUsedTextureTimestamp)
            {
                oldestUsedTextureTimestamp = texUseTimestamp;
                oldestUsedTextureId = texId;
            }
        }

        ASSERT(m_textureIdToBoundUnit.count(oldestUsedTextureId) == 1);

        // Get the unit we are going to replace
        freeUnit = m_textureIdToBoundUnit[oldestUsedTextureId];

        // UnTrack texture
        Texture *oldestUsedTexture = m_textureIdToTexture[oldestUsedTextureId];
        ASSERT(oldestUsedTexture);
        UnTrackTexture(oldestUsedTexture);
    }
    return freeUnit;
}

void TextureUnitManager::UpdateStructuresForUsedTexture(Texture *texture,
                                                        uint usedUnit)
{
    const GLId texId = texture->GetGLId();

    m_textureIdToTexture[texId] = texture;
    m_textureIdToBoundUnit[texId] = usedUnit;

    texture->EventEmitter<IDestroyListener>::RegisterListener(this);
}


int TextureUnitManager::GetMaxTextureUnits()
{
    return TextureUnitManager::GetActive()->m_numTextureUnits;
}

void TextureUnitManager::UnBindAllTexturesFromAllUnits()
{
    const int MaxTexUnits = TextureUnitManager::GetMaxTextureUnits();
    for (int unit = 0; unit < MaxTexUnits; ++unit)
    {
        GL::ActiveTexture(GL_TEXTURE0 + unit);
        GL::UnBind(GL::BindTarget::Texture1D);
        GL::UnBind(GL::BindTarget::Texture2D);
        GL::UnBind(GL::BindTarget::Texture3D);
        GL::UnBind(GL::BindTarget::TextureCubeMap);
    }
}

GLId TextureUnitManager::GetBoundTextureToUnit(GL::TextureTarget texTarget,
                                               GL::Enum textureUnit)
{
    ASSERT(textureUnit >= GL_TEXTURE0);

    GL::ActiveTexture(textureUnit);
    GLId texId = GL::GetInteger(SCAST<GL::Enum>(texTarget));
    return texId;
}

void TextureUnitManager::PrintTextureUnits()
{
    Debug_Log("===============================");
    const int NumTextureUnits = GL::GetInteger(GL::MaxTextureImageUnits);
    for (int i = 0; i < NumTextureUnits; ++i)
    {
        Debug_Log("Texture unit " << i << " ---:");
        GL::ActiveTexture(GL_TEXTURE0 + i);
        GLId tex1DId = GL::GetInteger(GL::Enum::TextureBinding1D);
        GLId tex2DId = GL::GetInteger(GL::Enum::TextureBinding2D);
        GLId tex3DId = GL::GetInteger(GL::Enum::TextureBinding3D);
        GLId texCMId = GL::GetInteger(GL::Enum::TextureBindingCubeMap);
        Debug_Log("  Texture_1D:      " << tex1DId);
        Debug_Log("  Texture_2D:      " << tex2DId);
        Debug_Log("  Texture_3D:      " << tex3DId);
        Debug_Log("  Texture_CubeMap: " << texCMId);

        uint boundToThisUnit = 0;
        if (tex1DId > 0) { ++boundToThisUnit; }
        if (tex2DId > 0) { ++boundToThisUnit; }
        if (tex3DId > 0) { ++boundToThisUnit; }
        if (texCMId > 0) { ++boundToThisUnit; }
        if (boundToThisUnit > 1)
        {
            Debug_Error("More than one texture bound to the same unit !!!!");
        }
        Debug_Log("-----------------------------");
    }
    Debug_Log("===============================");
}

TextureUnitManager *TextureUnitManager::GetActive()
{
    return GEngine::GetActive()->GetTextureUnitManager();
}

void TextureUnitManager::UnTrackTexture(Texture *texture)
{
    const GLId texId = texture->GetGLId();
    m_timestampTexIdUsed.erase(texId);
    m_textureIdToTexture.erase(texId);
    m_textureIdToBoundUnit.erase(texId);
    texture->EventEmitter<IDestroyListener>::UnRegisterListener(this);
}

void TextureUnitManager::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    Texture *tex = DCAST<Texture*>(object);
    UnTrackTexture(tex);
}
