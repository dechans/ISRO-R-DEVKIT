#pragma once

#include "IObject.h"

#include <GFX3DFunction/GFontTexture.h>

class CIGIDObject : public CIObject {
public:
    /// \brief Get the assigned name of this object
    const std::n_wstring &GetName() const;

private:
    char pad_00DC[52]; //0x00DC
    std::n_wstring m_name; //0x0110
    CGFontTexture fonttexture_playername; //0x012C
    char pad_0194[28]; //0x0194
    CGFontTexture fonttexture_title; //0x01B0
    char pad_0218[60]; //0x0218
};
