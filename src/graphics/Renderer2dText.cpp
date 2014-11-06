#include <graphics/Renderer2dText.h>

#include <graphics/GlCheck.h>
#include <graphics/GlUtil.h>
#include <graphics/ShaderResourceManager.h>
#include <math/Size.h>
#include <util/CharMap.h>
#include <util/TextBoxText.h>
#include <util/assert.h>


Renderer2dText::Renderer2dText()
  : log_("Renderer2dText"),
    zoomFactor_(1u),
    isDirty_(true)
{
  log_.d("Initializing");

  // assert(charmap_ && surface_ && textBoxText_);
  // assert(viewport_ != Size(0, 0));
  shader_ = ShaderResourceManager::instance().loadShader(
    {{ShaderProgram::VERTEX,   "render2dtext.vert"},
     {ShaderProgram::FRAGMENT, "render2dtext.frag"}});
  assert(shader_);
  updateShader();

  vertexBuffer_  = GlUtil::allocateVertexBuffer(2*4*sizeof(GLfloat));
}


Renderer2dText::~Renderer2dText()
{
}



void Renderer2dText::render(float time) const
{
  (void)time;
  assert(charmap_ && charmapImage_ && textBoxText_);
  assert(!vertices_.empty());

  CHECKGL();
  GlState::useProgram(shader_->getGlId());
  CHECKGL();

  // Uniform Tex1
  GlState::activeTexture(GL_TEXTURE0);
  charmapImage_->glBind();
  glUniform1i(paramId_Tex1_, 0);

  // Uniform Viewport
  glUniform2iv(paramId_Viewport_, 1, viewport_.getData());

  // Uniform quadVertices
  glEnableVertexAttribArray(0);
  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glUniform2fv(paramId_charBoxDimensions_, 1, charTextDim_.getData());

  // Render instanced quads in batches of 512
  int remaining = static_cast<int>(textData_.size());
  int rendered = 0;
  while (remaining > 0) {
    int toRender = std::min(remaining, 64);
      // Transfer data to uniform.
    glUniform4fv(paramId_offset_, toRender, &textData_[rendered][0]);

    // render toRender quads, with information
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, toRender);

    // Increment, decrement counters
    rendered += toRender;
    remaining -= toRender;
  }

  assert(rendered == static_cast<int>(textData_.size()));
  glDisableVertexAttribArray(0);
}


void Renderer2dText::update(float time)
{
  (void)time;
  if (shader_->isModified())
    updateShader();
  if (isDirty_)
    prepareTextData();
}



void Renderer2dText::handleResize(int width, int height)
{
  if (viewport_.w() == width && viewport_.h() == height)
    return;

  viewport_ = Size(width, height);
  if (!vertices_.empty())
    updateQuad();
}


void Renderer2dText::setTextDataObjects(const TextBoxText& textBoxText,
                                        const CharMap& charmap,
                                        SurfaceShPtr& charmapImage)
{
  charmap_.reset(new CharMap(charmap));
  textBoxText_.reset(new TextBoxText(textBoxText));
  charmapImage_ = charmapImage;
}


void Renderer2dText::setText(const std::string& text)
{
  assert(textBoxText_);
  textBoxText_->setText(text);
  isDirty_ = true;
}


void Renderer2dText::setPosition(Point position)
{
  assert(textBoxText_);
  position_ = position;
  isDirty_ = true;
}


void Renderer2dText::setZoomFactor(unsigned char zoomFactor)
{
  assert(textBoxText_);
  zoomFactor_ = zoomFactor;
  isDirty_ = true;
}


void Renderer2dText::setBoxWidth(unsigned int boxWidth)
{
  assert(textBoxText_);
  textBoxText_->setBoxWidth(boxWidth);
  isDirty_ = true;
}


void Renderer2dText::postConfigureInitialize()
{
  assert(charmap_ && charmapImage_ && textBoxText_);
  assert(viewport_ != Size(0, 0));
  updateQuad();
}


void Renderer2dText::updateShader()
{
  shader_->compile();
  paramId_charBoxDimensions_ =
    glGetUniformLocation(shader_->getGlId(), "charBoxDimensions");
  paramId_Viewport_ = glGetUniformLocation(shader_->getGlId(), "Viewport");
  paramId_offset_   = glGetUniformLocation(shader_->getGlId(), "offset");
  paramId_Tex1_     = glGetUniformLocation(shader_->getGlId(), "Tex1");
}

void Renderer2dText::updateQuad()
{
  int cw = charmap_->getCharSize().w();
  int ch = charmap_->getCharSize().h();

  const GLfloat x = static_cast<float>(cw * 2 * zoomFactor_);
  const GLfloat y = static_cast<float>(ch * 2 * zoomFactor_);

  const float xoff = (0.375f/static_cast<float>(zoomFactor_ + 1) +
                      static_cast<float>(position_.x() - viewport_.w()));
  const float yoff = (0.375f/static_cast<float>(zoomFactor_ + 1) +
                      static_cast<float>(viewport_.h() - position_.y()));

  const GLfloat A[] = {0 + xoff, -y + yoff};
  const GLfloat B[] = {x + xoff, -y + yoff};
  const GLfloat C[] = {0 + xoff,  0 + yoff};
  const GLfloat D[] = {x + xoff,  0 + yoff};

  vertices_ = { A[0], A[1],
                B[0], B[1],
                C[0], C[1],
                D[0], D[1] };

  log_.e() << "TextBoxTextRender::prepareVertices" << Log::end;
  log_.w() << "zoomFactor_: " << static_cast<int>(zoomFactor_) << Log::end;
  log_.w() << "cw:ch: " << cw << " : " << ch << Log::end;
  log_.w() << "x:y: " << x << " : " << y << Log::end;
  log_.w() << "xoff:yoff: " << xoff << " : " << yoff << Log::end;
  log_.w() << "vertices: " << vertices_ << Log::end;

  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  GlUtil::fillVertexBuffer(vertices_);
}


void Renderer2dText::prepareTextData()
{
  // Uniform Charbox dimensions
  const int charWidth     = charmap_->getCharSize().w();
  const float charWidthf  =
      static_cast<float>(charWidth) /
      static_cast<float>(charmapImage_->getWidth());
  const int charHeight    = charmap_->getCharSize().h();
  const float charHeightf =
      static_cast<float>(charHeight) /
      static_cast<float>(charmapImage_->getHeight());

  charTextDim_ = Pointf(charmapImage_->glTexCoordX(charWidthf),
                        charmapImage_->glTexCoordY(charHeightf));

  textData_.clear();
  textData_.resize(textBoxText_->getNCharacters());

  size_t charIndex = 0;
  const uint8_t cHeight = textBoxText_->getHeight();
  const std::array<uint8_t, 256>& cWidths = textBoxText_->getWidths();
  const std::vector<std::string>& lines = textBoxText_->getFormatted();
  float xpos = 0.0f;
  float hpos = 0.0f;
  for (auto const & line : lines) {
    for (const auto& ch : line) {
      // TODO swarminglogic, 2013-10-02:
      // The variant should be provided by TextBoxText
      // TextBoxTextChar 'getChar', 'getVariant'
      const Point charIndexOffset =
        charmap_->getCharIndexOffset(CharMap::C_GOLDEN, ch);
      const float tpos =
          static_cast<float>(charIndexOffset.x()) * charTextDim_.x();
      const float upos =
          static_cast<float>(charIndexOffset.y()) * charTextDim_.y();
      textData_[charIndex++] = glm::vec4(xpos, hpos, tpos, upos);
      xpos += static_cast<float>(cWidths[ch] * 2 * zoomFactor_);
    }
    // 3: line spacing
    hpos -= static_cast<float>(cHeight) * 2 * zoomFactor_;
    xpos = 0;
  }

  isDirty_ = false;
}
