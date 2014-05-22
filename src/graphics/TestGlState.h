#ifndef GRAPHICS_TESTGLSTATE_H
#define GRAPHICS_TESTGLSTATE_H

#include <graphics/GlState.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the GlState class.
 *
 * Only doing test-cases that don't require a OpenGL context.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestGlState : public CxxTest::TestSuite
{
public:
  void testGlStatics()
  {
    // GlState::Capabaility
    TS_ASSERT_EQUALS(GlState::toGLenum(GlState::BLEND), GL_BLEND);
    TS_ASSERT_EQUALS(GlState::toGLenum(GlState::CULL_FACE), GL_CULL_FACE);
    TS_ASSERT_EQUALS(GlState::toGLenum(GlState::DEPTH_TEST), GL_DEPTH_TEST);

    // GlState::BufferTarget
    TS_ASSERT_EQUALS(GlState::toGLenum(GlState::ARRAY_BUFFER),
                     GL_ARRAY_BUFFER);
    TS_ASSERT_EQUALS(GlState::toGLenum(GlState::ELEMENT_ARRAY_BUFFER),
                     GL_ELEMENT_ARRAY_BUFFER);
  }


  void testGlState()
  {
    GlState::Capability cap = GlState::BLEND;
    // Checking that default is false
    TS_ASSERT(!GlState::isEnabled(cap));
    // Checking that setting it false is redundant.
    TS_ASSERT(!GlState::disable(cap));
    // Checking that setting it true is not redundant
    TS_ASSERT(GlState::enable(cap));
    TS_ASSERT(GlState::isEnabled(cap));
    // Checking that setting it true is now redundant.
    TS_ASSERT(!GlState::enable(cap));
    TS_ASSERT(GlState::isEnabled(cap));

    // // Same for depth test
    cap = GlState::DEPTH_TEST;
    TS_ASSERT(!GlState::isEnabled(cap));
    TS_ASSERT(!GlState::disable(cap));
    TS_ASSERT(GlState::enable(cap));
    TS_ASSERT(GlState::isEnabled(cap));
    TS_ASSERT(!GlState::enable(cap));
    TS_ASSERT(GlState::isEnabled(cap));

    // Viewport
    // Checking that default is false
    TS_ASSERT_EQUALS(GlState::getViewport(), Rect(0,0,0,0));
    TS_ASSERT(!GlState::viewport(Rect(0,0,0,0)));
    TS_ASSERT(GlState::viewport(Rect(0,0,800,600)));
    TS_ASSERT_EQUALS(GlState::getViewport(), Rect(0,0,800,600));
    TS_ASSERT(!GlState::viewport(Rect(0,0,800,600)));

    // Active texture
    TS_ASSERT_EQUALS(GlState::getActiveTexture(), GL_TEXTURE0);
    TS_ASSERT(!GlState::activeTexture(GL_TEXTURE0));
    TS_ASSERT(GlState::activeTexture(GL_TEXTURE1));
    TS_ASSERT_EQUALS(GlState::getActiveTexture(), GL_TEXTURE1);
    TS_ASSERT(!GlState::activeTexture(GL_TEXTURE1));

    // Program
    TS_ASSERT_EQUALS(GlState::getProgram(), 0);
    TS_ASSERT(!GlState::useProgram(0));
    TS_ASSERT(GlState::useProgram(42));
    TS_ASSERT_EQUALS(GlState::getProgram(), 42);
    TS_ASSERT(!GlState::useProgram(42));
  }

private:
};

#endif
