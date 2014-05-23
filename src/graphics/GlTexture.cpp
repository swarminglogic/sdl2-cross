#include <graphics/GlTexture.h>

#include <cassert>

#include <graphics/GlState.h>
#include <math/MathUtil.h>


GlTexture::GlTexture()
  : log_("GlTexture"),
    textureId_(0),
    isMaxFiltering_(true)
{
}


GlTexture::~GlTexture()
{
  releaseResources();
}


void GlTexture::releaseResources()
{
  if (textureId_) {
    glDeleteTextures(1, &textureId_);
    textureId_ = 0;
  }
}

void GlTexture::setId(GLuint textureId)
{
  releaseResources();
  textureId_ = textureId;
}


GLuint GlTexture::glBind() const
{
  assert(textureId_ && "OpenGL texture id not set!");
  GlState::bindTexture(GL_TEXTURE_2D, textureId_);
  return textureId_;
}


GLuint GlTexture::getTextureId() const
{return textureId_;}


// bool GlTexture::isMaxFiltering() const
// {return isMaxFiltering_;}

// void GlTexture::setIsMaxFiltering(bool isMaxFilteringSet)
// {isMaxFiltering_ = isMaxFilteringSet;}
