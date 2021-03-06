#include "Bang/FontSheetCreator.h"

#include <SDL_pixels.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>

#include <SDL_ttf.h>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/AspectRatioMode.h"
#include "Bang/GL.h"
#include "Bang/Image.h"
#include "Bang/Math.h"
#include "Bang/Texture2D.h"
#include "Bang/Vector2.h"

using namespace Bang;

bool FontSheetCreator::LoadAtlasTexture(TTF_Font *ttfFont,
                                        Texture2D *atlasTexture,
                                        const String &charsToLoad,
                                        Array<AARecti> *imagesOutputRects,
                                        int extraMargin)
{
    if (!ttfFont)
    {
        return false;
    }

    Array<Image> charImages;
    for (const char c : charsToLoad)
    {
        if (TTF_GlyphIsProvided(ttfFont, c))
        {
            // Create bitmap
            constexpr SDL_Color WhiteColor = {255, 255, 255, 255};
            TTF_SetFontHinting(ttfFont, TTF_HINTING_LIGHT);
            SDL_Surface *charBitmap =
                TTF_RenderGlyph_Blended(ttfFont, c, WhiteColor);
            if (!charBitmap)
            {
                continue;
            }

            SDL_PixelFormat *fmt = charBitmap->format;
            Uint32 *charPixels = SCAST<Uint32 *>(charBitmap->pixels);
            Image charImage(charBitmap->w, charBitmap->h);
            for (int y = 0; y < charBitmap->h; ++y)
            {
                for (int x = 0; x < charBitmap->w; ++x)
                {
                    Uint32 color32 = charPixels[y * charBitmap->w + x];
                    Uint32 alpha = ((color32 & fmt->Amask) >> fmt->Ashift)
                                   << fmt->Aloss;
                    Color pxColor = Color::Zero();
                    pxColor.a = (alpha / 255.0f);
                    charImage.SetPixel(x, y, pxColor);
                }
            }
            // charImage.Export( Path("Char_" + String(int(c)) + "_" +
            // String(charBitmap->w) + ".png") );

            charImages.PushBack(charImage);
            SDL_FreeSurface(charBitmap);
        }
        else
        {
            Image empty;
            empty.Create(1, 1, Color::Zero());
            charImages.PushBack(empty);
        }
    }

    // Resize the atlas to fit only the used area
    Image atlasImage = FontSheetCreator::PackImages(
        charImages, extraMargin, imagesOutputRects);
    // atlasImage.Export(Path("font_" + String(atlasImage.GetWidth()) +
    // ".png"));

    if (atlasTexture)  // Create final atlas texture
    {
        GL::PixelStore(GL::UNPACK_ALIGNMENT, 1);
        atlasTexture->Import(atlasImage);
        atlasTexture->SetWrapMode(GL::WrapMode::CLAMP_TO_EDGE);
        atlasTexture->SetFilterMode(GL::FilterMode::NEAREST);
        atlasTexture->SetAlphaCutoff(0.5f);
        atlasTexture->Bind();
        atlasTexture->GenerateMipMaps();
        atlasTexture->UnBind();
    }

    return true;
}

Image FontSheetCreator::PackImages(const Array<Image> &images,
                                   int margin,
                                   Array<AARecti> *imagesOutputRects,
                                   const Color &bgColor)
{
    int maxImgWidth = 0;
    int maxImgHeight = 0;
    for (const Image &img : images)
    {
        maxImgWidth = Math::Max(maxImgWidth, img.GetWidth());
        maxImgHeight = Math::Max(maxImgHeight, img.GetHeight());
    }

    int imagesPerSide = Math::Sqrt(images.Size()) + 1;
    int totalWidth = (imagesPerSide) * (maxImgWidth + (margin * 2));
    int totalHeight = (imagesPerSide) * (maxImgHeight + (margin * 2));

    Image result;
    result.Create(totalWidth, totalHeight, Color::Zero());

    maxImgHeight = 0;
    int currentRowImages = 0;
    Vector2i penPosTopLeft(0, margin);
    for (const Image &img : images)
    {
        if (penPosTopLeft.x + img.GetWidth() + margin * 2 > totalWidth ||
            currentRowImages >= imagesPerSide)
        {
            penPosTopLeft.x = 0;  // Go to next line
            penPosTopLeft.y += maxImgHeight + margin * 2;
            currentRowImages = 0;
            maxImgHeight = 0;
        }
        ++currentRowImages;
        penPosTopLeft.x += margin;

        AARecti imgRect = AARecti(penPosTopLeft, penPosTopLeft + img.GetSize());
        if (imagesOutputRects)
        {
            imagesOutputRects->PushBack(imgRect);
        }
        result.Copy(img, imgRect);

        maxImgHeight = Math::Max(maxImgHeight, imgRect.GetSize().y);
        penPosTopLeft.x += imgRect.GetWidth() + margin;
    }

    Vector2i minPixel = result.GetSize();
    Vector2i maxPixel = Vector2i::Zero();
    for (int y = 0; y < result.GetHeight(); ++y)
    {
        for (int x = 0; x < result.GetWidth(); ++x)
        {
            if (result.GetPixel(x, y).a > 0)
            {
                Vector2i xy(x, y);
                minPixel = Vector2i::Min(minPixel, xy);
                maxPixel = Vector2i::Max(maxPixel, xy);
            }
        }
    }

    maxPixel += Vector2i::One();
    Vector2i fittedSize = (maxPixel - minPixel);
    Image fittedResult(fittedSize.x, fittedSize.y);
    fittedResult = result.GetSubImage(AARecti(minPixel, maxPixel));
    fittedResult.FillTransparentPixels(bgColor);

    Image fittedResultMargined = fittedResult;
    fittedResultMargined.AddMargins(
        Vector2i(margin), bgColor, AspectRatioMode::IGNORE);
    return fittedResultMargined;
}
