#ifndef GRAPHICS_RENDERERINTERFACE_H
#define GRAPHICS_RENDERERINTERFACE_H

#include <graphics/GlState.h>
#include <graphics/SDL_opengl.h>
#include <math/Rect.h>
#include <util/SDL_Event_fwd.h>


class ShaderResourceManager;

/**
 * RendererInterface class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class RendererInterface
{
public:
  RendererInterface(){}
  virtual ~RendererInterface(){}

  // Optional overrides
  virtual void initialize() {};
  virtual void finalize() {};
  virtual void handleResize(int width, int height) {
    GlState::viewport(Rect(0, 0, width, height));
  }


  // Pure virtuals
  virtual void update() = 0;
  virtual bool handleEvent(const SDL_Event&) = 0;

private:
  // NonCopyable
  RendererInterface(const RendererInterface& c);
  RendererInterface& operator=(const RendererInterface& c);
};


#endif
