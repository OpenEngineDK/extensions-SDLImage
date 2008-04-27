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

    if ((image= IMG_Load(filename.c_str())) == NULL)
      throw ResourceException("Error loading SDLImage data in: " + filename);

    depth = image->format->BitsPerPixel;
    if (depth != 32 && depth != 24) {
        string msg = "Unsupported color depth: ";
	msg += Convert::int2string(depth) + " in file: " + filename;
	throw ResourceException(msg);
    }

    SDL_PixelFormat fmt;
    switch (depth) {
    case 24:
      {
	  //It's 24 bit, so always convert
	  SDL_PixelFormat format = {NULL, 24, 4, 0, 0, 0, 0, 0, 8, 16, 24,
				    0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000, 0, 255};
	  fmt = format;
      }
      break;
    case 32:
      {
	  // It's 32 bit, so convert only if it's ABGR
	  if (image->format->Rshift > image->format->Bshift) {
	      SDL_PixelFormat format = {NULL, 32, 4, 0, 0, 0, 0, 0, 8, 16, 24,
					0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000, 0, 255};
	      fmt = format;
	  }
      }
      break;
      default:
 	  fmt = *(image->format);
      }

      SDL_Surface *temp = SDL_ConvertSurface(image, &fmt, SDL_SWSURFACE);
      SDL_FreeSurface(image);
      image = temp;

      //flip vertecally
      ReverseVertecally();

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
  logger.info << "depth=" << depth<< " in file: " << filename << logger.end;
  return depth;
}

unsigned char* SDLImage::GetData(){
  return (unsigned char*)image->pixels;
}

} //NS Resources
} //NS OpenEngine
