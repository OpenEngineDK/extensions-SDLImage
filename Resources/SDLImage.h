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
    bool loaded;
    int id;                     //!< material identifier
    string filename;            //!< file name
    unsigned char* data;        //!< binary material data
    unsigned int width;                  //!< texture width
    unsigned int height;                 //!< texture height
    unsigned int depth;                  //!< texture depth/bits

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

    SDLImage() : loaded(false),data(NULL) {
        width = height = depth = id = 0;
    };

    static bool sdlloaded;
    static void InitSDL();

    SDLImage(string file);
    ~SDLImage();

    // resource methods
    void Load();
    void Unload();

    // texture resource methods
    int GetID();
    void SetID(int id);   
    unsigned int GetWidth();
    unsigned int GetHeight();
    unsigned int GetDepth();
    unsigned char* GetData();
    ColorFormat GetColorFormat();
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
