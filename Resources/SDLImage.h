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

#include <Resources/ITextureResource.h>
#include <Resources/IResourcePlugin.h>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/split_member.hpp>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Resources {

using namespace std;

/**
 * SDL image resource.
 *
 * @class SDLImage SDLImage.h Resources/SDLImage.h
 */
class SDLImage : public ITextureResource {
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

    SDLImage() : ITextureResource() {}
    SDLImage(string file);
    ~SDLImage();

    static bool sdlloaded;
    static void InitSDL();

    // resource methods
    void Load();

    // texture resource methods
    virtual void ReverseVertecally();
};

/**
 * SDL image texture resource plug-in.
 *
 * @class SDLImagePlugin SDLImage.h Resources/SDLImage.h
 */
class SDLImagePlugin : public IResourcePlugin<ITextureResource> {
public:
    SDLImagePlugin();
    ITextureResourcePtr CreateResource(string file);
};

} //NS Resources
} //NS OpenEngine

BOOST_CLASS_EXPORT(OpenEngine::Resources::SDLImage)

#endif // _SDL_IMAGE_H_
