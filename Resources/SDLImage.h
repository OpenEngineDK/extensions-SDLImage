// SDL image resource.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
// Modified by Anders Bach Nielsen <abachn@daimi.au.dk> - 21. Nov 2007
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------

#ifndef _SDL_IMAGE_H_
#define _SDL_IMAGE_H_

#include <Resources/Texture2D.h>
#include <Resources/IResourcePlugin.h>
#include <string>
#include <iostream>
#include <fstream>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Resources {

using namespace std;

class IArchiveWriter;
class IArchiveReader;


/**
 * SDL image resource.
 *
 * @class SDLImage SDLImage.h Resources/SDLImage.h
 */
class SDLImage : public Texture2D<unsigned char> {
private:
    string filename;            //!< file name

public:

    /**
     * Constructor
     *
     * @param file tga file to load.
     */

    //    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<ITextureResource>(*this);
        ar & filename;
    }

    SDLImage() : Texture2D<unsigned char>() {}
    SDLImage(string file);
    ~SDLImage();

    static bool sdlloaded;
    static void InitSDL();

    // resource methods
    void Load();

    // texture resource methods
    virtual void ReverseVertecally();

    void Serialize(Resources::IArchiveWriter& );
    void Deserialize(Resources::IArchiveReader& );
};

/**
 * SDL image texture resource plug-in.
 *
 * @class SDLImagePlugin SDLImage.h Resources/SDLImage.h
 */
class SDLImagePlugin : public IResourcePlugin<ITexture2D> {
public:
    SDLImagePlugin();
    ITexture2DPtr CreateResource(string file);
};

/**
 * SDL image texture resource plug-in.
 *
 * @class UCharSDLImagePlugin SDLImage.h Resources/SDLImage.h
 */
class UCharSDLImagePlugin : public IResourcePlugin<UCharTexture2D> {
public:
    UCharSDLImagePlugin();
    UCharTexture2DPtr CreateResource(string file);
};

} //NS Resources
} //NS OpenEngine


#endif // _SDL_IMAGE_H_
