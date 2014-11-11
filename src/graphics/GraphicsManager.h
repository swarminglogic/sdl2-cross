#ifndef GRAPHICS_GRAPHICSMANAGER_H
#define GRAPHICS_GRAPHICSMANAGER_H

#include <graphics/SDL_opengl.h>
#include <math/Size.h>
#include <util/Log.h>
#include <util/SDL.h>


class ViewConfig;
class PreferenceManager;

/**
 * GraphicsManager class.
 *
 * @author SwarmingLogic
 */
class GraphicsManager
{
 public:
  GraphicsManager(const PreferenceManager& preferences);
  virtual ~GraphicsManager();

  void swapBuffers();

  Size getScreenSize() const;
  void setScreenSize(Size windowSize);

 private:
  void initalizeOpenGL(const ViewConfig& viewConfig);

  // Debug function for logging graphics driver information
  void logAcquiredGlVersion(const std::string& glName) const;
  void logGraphicsDriverInfo() const;
  void logOpenGLContextInfo() const;
  void logStaticOpenGLInfo() const;

  Log log_;
  sdl::WindowPtr window_;
  Size screenSize_;
  sdl::GLContextPtr context_;
  bool isFullScreen_;
  bool isVSync_;
  bool isOpenGlDebugEnabled_;

  GLuint vertexArrayObject_;

  // NonCopyable
  GraphicsManager(const GraphicsManager& c);
  GraphicsManager& operator=(const GraphicsManager& c);
};

#endif  // GRAPHICS_GRAPHICSMANAGER_H
