// SDL image resource.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// Modified by Anders Bach Nielsen <abachn@daimi.au.dk> - 21. Nov 2007
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Resources/SDLImage.h>
#include <Resources/Exceptions.h>
#include <Resources/File.h>
#include <Utils/Convert.h>

//@todo make a SDL_image in Meta to suppot different platforms
#include <SDL_image.h>

namespace OpenEngine {
namespace Resources {

using OpenEngine::Utils::Convert;

bool SDLImage::sdlloaded = false;

SDLImagePlugin::SDLImagePlugin() {
    this->AddExtension("bmp");
    this->AddExtension("gif");
    this->AddExtension("jpeg");
    this->AddExtension("jpg");
    this->AddExtension("lbm");
    this->AddExtension("pcx");
    this->AddExtension("png");
    this->AddExtension("pnm");
    this->AddExtension("tiff");
    this->AddExtension("tif");
    this->AddExtension("tga");
    this->AddExtension("xcf");
    this->AddExtension("xpm");
    this->AddExtension("xv");
}

ITextureResourcePtr SDLImagePlugin::CreateResource(string file) {
    return ITextureResourcePtr(new SDLImage(file));
}

SDLImage::SDLImage(string filename)
    : ITextureResource(), 
      filename(filename) {
}

SDLImage::~SDLImage() {
    this->Unload();
}

void SDLImage::Load() {
    if (this->data) return;

    SDL_Surface* image = IMG_Load(filename.c_str());
    if (!image)
        throw ResourceException("Error loading SDLImage data in: " + filename + ". Description: " + SDL_GetError());

    unsigned int depth = image->format->BitsPerPixel;
    if (depth != 32 && depth != 24) {
        string msg = "Unsupported color depth: ";
        msg += Convert::ToString(depth) + " in file: " + filename;
        throw ResourceException(msg);
    }

    SDL_PixelFormat format;
    switch(depth) {
    case 32: 
        format.palette = 0; format.colorkey = 0; format.alpha = 0;
        format.BitsPerPixel = 32; format.BytesPerPixel = 4;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        format.Rmask = 0xFF000000; format.Rshift = 0; format.Rloss = 0;
        format.Gmask = 0x00FF0000; format.Gshift = 8; format.Gloss = 0;
        format.Bmask = 0x0000FF00; format.Bshift = 16; format.Bloss = 0;
        format.Amask = 0x000000FF; format.Ashift = 24; format.Aloss = 0;
#else
        format.Rmask = 0x000000FF; format.Rshift = 24; format.Rloss = 0;
        format.Gmask = 0x0000FF00; format.Gshift = 16; format.Gloss = 0;
        format.Bmask = 0x00FF0000; format.Bshift = 8; format.Bloss = 0;
        format.Amask = 0xFF000000; format.Ashift = 0; format.Aloss = 0;
#endif
        break;
    case 24:
        format.palette = 0; format.colorkey = 0; format.alpha = 0;
        format.BitsPerPixel = 24; format.BytesPerPixel = 3;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        format.Rmask = 0xFF000000; format.Rshift = 0; format.Rloss = 0;
        format.Gmask = 0x00FF0000; format.Gshift = 8; format.Gloss = 0;
        format.Bmask = 0x0000FF00; format.Bshift = 16; format.Bloss = 0;
#else
        format.Rmask = 0x000000FF; format.Rshift = 24; format.Rloss = 0;
        format.Gmask = 0x0000FF00; format.Gshift = 16; format.Gloss = 0;
        format.Bmask = 0x00FF0000; format.Bshift = 8; format.Bloss = 0;
#endif
        break;
    default:
        throw Exception("unsupported color depth, in file: " + filename);
    }

    SDL_LockSurface(image);
    SDL_Surface* converted = SDL_ConvertSurface(image, &format, SDL_SWSURFACE);
    if (converted == NULL)
        throw ResourceException("Error converting SDLImage data in: " +
                                filename);
    SDL_LockSurface(converted);
    SDL_FreeSurface(image);

    this->width = converted->w;
    this->height = converted->h;
    depth = converted->format->BitsPerPixel;
    this->channels = (depth/8);

    unsigned int lineWidth = GetWidth() * this->channels;
    unsigned long size = lineWidth * GetHeight();
    this->data = new unsigned char[size];
    memcpy(this->data, converted->pixels, size);
    SDL_FreeSurface(converted);
    
    //flip vertecally
    ReverseVertecally();

    // Set format
    switch(this->channels){
    case 1:
        this->format = LUMINANCE;
        break;
    case 3:
        this->format = RGB;
        break;
    case 4:
        this->format = RGBA;
        break;
    default:
        throw Exception("unknown color format");
    }
}

void SDLImage::ReverseVertecally() {
    unsigned int lineWidth = GetWidth() * this->channels;
    unsigned long size = lineWidth * GetHeight();
    unsigned char* tempArr = new unsigned char[size];

    for (unsigned int i=0, j=size-lineWidth; i < size;
	 i+=lineWidth, j-=lineWidth) {
	memcpy(&tempArr[j], &data[i], lineWidth);
    }
    delete[] data;
    data = tempArr;
}

} //NS Resources
} //NS OpenEngine
