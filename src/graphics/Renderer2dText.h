#ifndef GRAPHICS_RENDERER2DTEXT_H
#define GRAPHICS_RENDERER2DTEXT_H

#include <glm/glm.hpp>
#include <graphics/RendererInterface.h>
#include <graphics/SDL_opengl.h>
#include <graphics/ShaderProgram.h>
#include <graphics/Surface.h>
#include <math/Point.h>
#include <util/Log.h>


class CharMap;
class TextBoxText;
class AssetImage;

/**
 * Renderer2dText class.
 *
 * @author SwarmingLogic
 */
class Renderer2dText : public RendererInterface
{
public:
  Renderer2dText();
  virtual ~Renderer2dText();

  virtual void render(float time) const;
  virtual void update(float time) override;
  virtual void handleResize(int width, int height);
  virtual bool handleEvent(const SDL_Event&) override { return 0; }

  // Textbox initialization
  void setTextDataObjects(const TextBoxText& textBoxText,
                          const CharMap& charmap,
                          SurfaceShPtr& charmapImage);

  // Text visualization control
  void setText(const std::string& text);
  void setPosition(Point position);
  void setZoomFactor(unsigned char zoomFactor);
  void setBoxWidth(unsigned int boxWidth);

  void postConfigureInitialize();

private:
  void updateShader();
  void updateQuad();
  void prepareTextData();

  Log log_;

  // Environment
  Size viewport_;

  // Shader stuff
  ShaderProgramShPtr shader_;
  GLuint vertexBuffer_;
  std::vector<GLfloat> vertices_;
  unsigned char zoomFactor_;
  GLint paramId_Tex1_;
  GLint paramId_Viewport_;
  GLint paramId_charBoxDimensions_;
  GLint paramId_offset_;

  // Text data
  std::vector<glm::vec4> textData_;
  Pointf charTextDim_;
  bool isDirty_;
  Point position_;

  // Data Objects
  std::unique_ptr<CharMap> charmap_;
  std::unique_ptr<TextBoxText> textBoxText_;
  SurfaceShPtr charmapImage_;


  // NonCopyable
  Renderer2dText(const Renderer2dText& c);
  Renderer2dText& operator=(const Renderer2dText& c);
};

#endif
