#ifndef GRAPHICS_RENDERER2DIMAGE_H
#define GRAPHICS_RENDERER2DIMAGE_H

#include <vector>

#include <graphics/RendererInterface.h>
#include <graphics/SDL_opengl.h>
#include <graphics/ShaderProgram.h>
#include <graphics/Surface.h>
#include <math/Point.h>
#include <util/Log.h>


/**
 * Renderer2dImage class.
 *
 * @author SwarmingLogic
 */
class Renderer2dImage : public RendererInterface
{
 public:
  Renderer2dImage();
  virtual ~Renderer2dImage();

  virtual void update(float time) override;
  virtual bool handleEvent(const SDL_Event&) override;

  virtual void setSurface(SurfaceShPtr surface);
  virtual void render(Point position,
                      float currentTime);

  // // To implement if needed
  // virtual void finalize() override;
  virtual void handleResize(int width, int height) override;

 private:
  void updateShader();
  void updateQuad();
  void updateTexcoords();

  Log log_;

  // Shader Program
  ShaderProgramShPtr shader_;

  // GPU link
  GLuint vertexBuffer_;
  GLuint textureBuffer_;
  std::vector<GLfloat> vertices_;
  std::vector<GLfloat> texcoords_;
  GLint shaderIdViewport_;
  GLint shaderIdTex_;

  // Other local
  Size viewport_;
  // Rectangle on screen to draw on (pixels)
  Rect targetRect_;

  // Surface data
  SurfaceShPtr surface_;

  // NonCopyable
  Renderer2dImage(const Renderer2dImage& c);
  Renderer2dImage& operator=(const Renderer2dImage& c);
};

#endif  // GRAPHICS_RENDERER2DIMAGE_H
