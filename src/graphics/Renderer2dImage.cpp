#include <graphics/Renderer2dImage.h>

#include <cassert>

#include <graphics/GlUtil.h>
#include <graphics/ShaderResourceManager.h>


Renderer2dImage::Renderer2dImage()
  : log_("Renderer2dImage"),
    targetRect_(0, 0, 0, 0),
    surface_(nullptr)
{
  log_.d("Initializing");

  shader_ = ShaderResourceManager::instance().loadShader(
    {{ShaderProgram::VERTEX,   "render2dimage.vert"},
     {ShaderProgram::FRAGMENT, "render2dimage.frag"}});

  assert(shader_ && "Failed to create shader program");
  updateShader();

  vertexBuffer_  = GlUtil::allocateVertexBuffer(3*4*sizeof(GLfloat));
  textureBuffer_ = GlUtil::allocateVertexBuffer(2*4*sizeof(GLfloat));
}


Renderer2dImage::~Renderer2dImage()
{
}

void Renderer2dImage::update(float time)
{
  (void)time;
  if (shader_->isModified())
    updateShader();
}


bool Renderer2dImage::handleEvent(const SDL_Event&)
{
  return false;
}



void Renderer2dImage::setSurface(SurfaceShPtr surface)
{
  surface_ = surface;
  targetRect_.setSize(surface->getSize());
  updateQuad();
  updateTexcoords();
}


void Renderer2dImage::handleResize(int width, int height)
{
  if (viewport_.w() == width && viewport_.h() == height)
    return;

  viewport_ = Size(width, height);
  updateQuad();
}


void Renderer2dImage::render(Point position,
                             float)
{
  assert(viewport_ != Size(0, 0));
  if (!surface_) {
    log_.e("Trying to render empty surface");
    return;
  }
  if (position.x() != targetRect_.x() ||
      position.y() != targetRect_.y()) {
    targetRect_.setPosition(position);
    updateQuad();
  }

  GlState::enable(GlState::BLEND);
  GlState::enable(GlState::DEPTH_TEST);

  // Uniform Viewport
  GlState::useProgram(shader_->getGlId());
  glUniform2iv(shaderIdViewport_, 1, viewport_.getData());

  // Uniform Tex1
  GlState::activeTexture(GL_TEXTURE0);
  surface_->glBind();
  glUniform1i(shaderIdTex_, 0);

  // vec3 vpos
  glEnableVertexAttribArray(0);
  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // vec2 tpos
  glEnableVertexAttribArray(1);
  GlState::bindBuffer(GlState::ARRAY_BUFFER, textureBuffer_);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  //  Draw 0 vertices, starting from 0.
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}


void Renderer2dImage::updateShader()
{
  log_.d("Updating shader");
  shader_->compile();
  shaderIdViewport_ = glGetUniformLocation(shader_->getGlId(), "Viewport");
  shaderIdTex_      = glGetUniformLocation(shader_->getGlId(), "Tex1");

  if (shaderIdViewport_ < 0) log_.w("Viewport uniform not found");
  if (shaderIdTex_      < 0) log_.w("Tex1 uniform not found");
}


void Renderer2dImage::updateQuad()
{
  assert(surface_);
  log_.d() << "Updating quad. target: " << targetRect_
           << "   Viewport: " << viewport_ << Log::end;

  // TODO swarminglogic, 2014-06-11: replace w/memembers
  const unsigned char zoomFactor_ = 1;

  const GLfloat x = static_cast<float>(targetRect_.w() * 2 * zoomFactor_);
  const GLfloat y = static_cast<float>(targetRect_.h() * 2 * zoomFactor_);
  const GLfloat z = 0.0f;

  const float xoff =
    0.375f + static_cast<float>(targetRect_.x() - viewport_.w());
  const float yoff =
    0.375f + static_cast<float>(viewport_.h() - targetRect_.y());

  const GLfloat A[] = {0 + xoff, -y + yoff, z};
  const GLfloat B[] = {x + xoff, -y + yoff, z};
  const GLfloat C[] = {0 + xoff,  0 + yoff, z};
  const GLfloat D[] = {x + xoff,  0 + yoff, z};

  vertices_ = { A[0], A[1], A[2],
                B[0], B[1], B[2],
                C[0], C[1], C[2],
                D[0], D[1], D[2] };
  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  GlUtil::fillVertexBuffer(vertices_);
}


void Renderer2dImage::updateTexcoords()
{
  log_.d("Updating texcoords");

  const float xmin = surface_->glTexCoordX(0.0f);
  const float ymin = surface_->glTexCoordY(0.0f);
  const float xmax = surface_->glTexCoordX(1.0f);
  const float ymax = surface_->glTexCoordY(1.0f);
  texcoords_ = {xmin , ymax,
                xmax , ymax,
                xmin , ymin,
                xmax , ymin};

  GlState::bindBuffer(GlState::ARRAY_BUFFER, textureBuffer_);
  GlUtil::fillVertexBuffer(texcoords_);
}
