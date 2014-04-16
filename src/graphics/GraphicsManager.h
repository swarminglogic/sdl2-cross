#ifndef GRAPHICS_GRAPHICSMANAGER_H
#define GRAPHICS_GRAPHICSMANAGER_H

#include <math/Size.h>
#include <util/Log.h>
#include <util/SDL.h>


class ViewConfig;

/**
 * GraphicsManager class.
 *
 * @author SwarmingLogic
 */
class GraphicsManager
{
public:
  GraphicsManager();
  virtual ~GraphicsManager();

  void swapBuffers();

  Size getScreenSize() const;
  void setScreenSize(Size windowSize);

private:
  void initalizeOpenGL(const ViewConfig& viewConfig);

  // Debug function for logging graphics driver information
  void logGraphicsDriverInfo() const;
  void logOpenGLContextInfo() const;
  void logStaticOpenGLInfo() const;

  Log log_;
  SdlWindowPtr window_;
  Size screenSize_;
  SdlGLContextPtr context_;
  bool isFullScreen_;
  bool isVSync_;
  bool isOpenGlDebugEnabled_;

  // NonCopyable
  GraphicsManager(const GraphicsManager& c);
  GraphicsManager& operator=(const GraphicsManager& c);
};

#endif
