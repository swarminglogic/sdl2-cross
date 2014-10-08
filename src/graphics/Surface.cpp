#include <graphics/Surface.h>

#include <cassert>
#include <string>

#include <graphics/GlState.h>
#include <graphics/SDL_image.h>
#include <math/MathUtil.h>


Surface::Surface(SDL_Surface* surface)
  : log_("Surface"),
    surface_(surface),
    textureId_(0),
    imageRect_(0, 0, 0, 0),
    isMaxFiltering_(true),
    repeatFactor_(1.0f)
{
}

Surface::Surface()
  : Surface(nullptr)
{
}


Surface::Surface(const AssetImage& imagefile)
  : Surface(nullptr)
{
  loadImage(imagefile);
}


Surface::~Surface()
{
  releaseResources();
}


void Surface::releaseResources()
{
  if (textureId_) {
    glDeleteTextures(1, &textureId_);
    textureId_ = 0;
  }
}


void Surface::loadImage(const AssetImage& imagefile)
{
  const std::string path = imagefile.path();
  log_.d() << "Loading image: " << path << Log::end;

  releaseResources();

  surface_.reset(IMG_Load(path.c_str()));
  if (!surface_) {
    log_.e() << "Failed to load image " << path << Log::end;
    log_.e() << SDL_GetError() << Log::end;
  } else {
    imageRect_ = Rect(0, 0, surface_->w, surface_->h);
  }
}


void Surface::setSurface(SDL_Surface& surface)
{
  surface_.reset(&surface);
  releaseResources();
  imageRect_ = Rect(0, 0, surface_->w, surface_->h);
}


SDL_Surface* Surface::getSurface()
{
  return surface_.get();
}


void Surface::setGlTextureId(GLuint textureId, Size size)
{
  releaseResources();
  textureId_ = textureId;
  imageRect_ = Rect(0, 0, size.w(), size.h());
}



unsigned int Surface::getWidth() const
{
  assert((surface_ || textureId_) && "Neither surface nor textureId set!");
  return imageRect_.w();
}


unsigned int Surface::getHeight() const
{
  assert((surface_ || textureId_) && "Neither surface nor textureId set!");
  return imageRect_.h();
}


Size Surface::getSize() const
{
  return imageRect_.getSize();
}


float Surface::glTexCoordX(float texcoord) const
{
  float surfSpace = (static_cast<float>(imageRect_.x()) + texcoord *
                     static_cast<float>(imageRect_.w())) /
                     static_cast<float>(imageRect_.w());
  return MathUtil::nextPow2TexCoord(surfSpace, getWidth());
}


float Surface::glTexCoordY(float texcoord) const
{
  float surfSpace = (static_cast<float>(imageRect_.y()) + texcoord *
                     static_cast<float>(imageRect_.h())) /
                     static_cast<float>(imageRect_.h());
  return MathUtil::nextPow2TexCoord(surfSpace, getHeight());
}


void Surface::prepareForGl()
{
  if (textureId_) {return;}
  assert(surface_ && "Surface not set!");

  const int width  = MathUtil::nextPow2(surface_->w);
  const int height = MathUtil::nextPow2(surface_->h);

  int bpp = 0;
  Uint32 Rmask, Gmask, Bmask, Amask = 0;
  SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp,
                             &Rmask, &Gmask, &Bmask, &Amask);
  sdl::SurfacePtr tempSurface(SDL_CreateRGBSurface(0, width, height,
                                                   bpp,
                                                   Rmask, Gmask, Bmask, Amask));

  SDL_SetSurfaceAlphaMod(surface_.get(), 0xFF);
  SDL_SetSurfaceBlendMode(surface_.get(), SDL_BLENDMODE_NONE);

  SDL_BlitSurface(surface_.get(), NULL, tempSurface.get(), nullptr);

  glGenTextures(1, &textureId_);
  GlState::bindTexture(GL_TEXTURE_2D, textureId_);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
               width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, tempSurface->pixels);

  if (isMaxFiltering_) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

#ifndef USE_OPENGLES
    float maxAnistropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnistropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    maxAnistropy);
#endif
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  log_.d() << "Preparing surface (" << textureId_ << ") for GL" << Log::end;
}


GLuint Surface::glBind() const
{
  assert(textureId_ != 0);

  assert(textureId_ && "OpenGL texture id not set!");

  GlState::bindTexture(GL_TEXTURE_2D, textureId_);
  return textureId_;
}


GLuint Surface::getTextureId() const
{
  return textureId_;
}


bool Surface::isMaxFiltering() const
{
  return isMaxFiltering_;
}


void Surface::setIsMaxFiltering(bool isMaxFilteringSet)
{
  isMaxFiltering_ = isMaxFilteringSet;
}


float Surface::getRepeatFactorHint() const
{
  return repeatFactor_;
}


void Surface::setRepeatFactorHint(float repeatFactor)
{
  repeatFactor_ = repeatFactor;
}
