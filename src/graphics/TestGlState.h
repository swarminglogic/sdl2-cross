#ifndef GRAPHICS_TESTGLSTATE_H
#define GRAPHICS_TESTGLSTATE_H

#include <graphics/GlState.h>

#include <cxxtest/TestSuite.h>

// TODO swarminglogic, 2014-06-12: Update tester to reflect that GlState calls
// no longer returns boolan values on whether or not they change the state.

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
#ifndef GL_STATE_GUARD_DISABLED
    GlState::Capability cap = GlState::BLEND;
    // Checking that default is false
    TS_ASSERT(!GlState::isEnabled(cap));
    GlState::enable(cap);
    TS_ASSERT(GlState::isEnabled(cap));
    GlState::disable(cap);
    TS_ASSERT(!GlState::isEnabled(cap));

    // // Same for depth test
    cap = GlState::DEPTH_TEST;
    TS_ASSERT(!GlState::isEnabled(cap));
    GlState::disable(cap);
    TS_ASSERT(!GlState::isEnabled(cap));
    GlState::enable(cap);
    TS_ASSERT(GlState::isEnabled(cap));

    // Viewport
    // Checking that default is false
    TS_ASSERT_EQUALS(GlState::getViewport(), Rect(0, 0, 0, 0));
    GlState::viewport(Rect(0, 0, 0, 0));
    TS_ASSERT_EQUALS(GlState::getViewport(), Rect(0, 0, 0, 0));
    GlState::viewport(Rect(0, 0, 800, 600));
    TS_ASSERT_EQUALS(GlState::getViewport(), Rect(0, 0, 800, 600));

    // Active texture
    TS_ASSERT_EQUALS(GlState::getActiveTexture(), GL_TEXTURE0);
    GlState::activeTexture(GL_TEXTURE0);
    TS_ASSERT_EQUALS(GlState::getActiveTexture(), GL_TEXTURE0);
    GlState::activeTexture(GL_TEXTURE1);
    TS_ASSERT_EQUALS(GlState::getActiveTexture(), GL_TEXTURE1);

    // Program
    TS_ASSERT_EQUALS(GlState::getProgram(), 0);
    GlState::useProgram(0);
    TS_ASSERT_EQUALS(GlState::getProgram(), 0);
    GlState::useProgram(42);
    TS_ASSERT_EQUALS(GlState::getProgram(), 42);
    GlState::useProgram(42);
    TS_ASSERT_EQUALS(GlState::getProgram(), 42);
// TODO swarminglogic, 2014-06-13: #else: create gl context and test values?
// #else
#endif
  }

 private:
};

#endif  // GRAPHICS_TESTGLSTATE_H
