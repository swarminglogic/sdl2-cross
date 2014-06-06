#ifndef UTIL_TESTSDL_H
#define UTIL_TESTSDL_H

#include <util/SDL.h>
#include <cxxtest/TestSuite.h>

/**
 * Test for the SDL utility header.
 *
 * @author SwarmingLogic
 */
class TestSDL : public CxxTest::TestSuite
{
public:
  void testAllSDLRAII()
  {
    sdl::WindowPtr window;
    TS_ASSERT(!window);
    window.reset(SDL_CreateWindow("n/a", 0,0, 100, 100, 0));
    TS_ASSERT(window);

    sdl::RendererPtr renderer;
    TS_ASSERT(!renderer);
    renderer.reset(SDL_CreateRenderer(window.get(), -1,
                                      SDL_RENDERER_SOFTWARE));
    TS_ASSERT(renderer);

    sdl::SurfacePtr surface;
    TS_ASSERT(!surface);
    surface.reset(SDL_CreateRGBSurface(0, 50, 50, 32,
                                       0, 0, 0, 0));
    TS_ASSERT(surface);

    sdl::TexturePtr texture;
    TS_ASSERT(!texture);
    texture.reset(SDL_CreateTextureFromSurface(renderer.get(),
                                               surface.get()));
    TS_ASSERT(texture);

    sdl::GLContextPtr glContext;
    TS_ASSERT(!glContext);
    glContext.reset(new SDL_GLContext(SDL_GL_CreateContext(window.get())));
    TS_ASSERT(glContext);

  }

private:
};

#endif
