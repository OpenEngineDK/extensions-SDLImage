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

namespace OpenEngine {
namespace Resources {

using OpenEngine::Utils::Convert;

bool SDLImage::sdlloaded = false;

SDLImagePlugin::SDLImagePlugin() {
    this->AddExtension("png");
    //TODO: add: BMP, GIF, JPEG, LBM, PCX, PNG, PNM, TGA, TIFF, XCF, XPM, XV
}

ITextureResourcePtr SDLImagePlugin::CreateResource(string file) {
    return ITextureResourcePtr(new SDLImage(file));
}

SDLImage::SDLImage(string filename)
    : loaded(false),
      filename(filename),
      data(NULL) {
    width = height = depth = id = 0;
}

SDLImage::~SDLImage() {
    Unload();
}

void SDLImage::Load() {
    if (loaded) return;

    SDL_Surface *tmp = NULL;
    if ((tmp= IMG_Load(filename.c_str())) == NULL)
      throw ResourceException("Error loading SDLImage data in: " + filename);
    image = SDL_DisplayFormat(tmp); //TODO free in destructor
    //måske:    Surf_Return = SDL_DisplayFormatAlpha(Surf_Temp);
    //eller noget med: SDL_SetAlpha(srcimg, SDL_SRCALPHA, alpha);
    SDL_FreeSurface(tmp);    

    loaded = true;
}

void SDLImage::Unload() {
    if (loaded) {
        SDL_FreeSurface(image);
        loaded = false;
    }
}

int SDLImage::GetID(){
    return id;
}
void SDLImage::SetID(int id){
    this->id = id;
}	
int SDLImage::GetWidth(){
    return image->w;
}
int SDLImage::GetHeight(){
    return image->h;
}
int SDLImage::GetDepth(){
    return image->format->BitsPerPixel;
}
unsigned char* SDLImage::GetData(){
  return (unsigned char*)image->pixels;
}

} //NS Resources
} //NS OpenEngine
