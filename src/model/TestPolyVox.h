#ifndef MODEL_TESTPOLYVOX_H
#define MODEL_TESTPOLYVOX_H

#include <model/PolyVox.h>
#include <cxxtest/TestSuite.h>

/**
 * Test for the PolyVox class.
 *
 * @author SwarmingLogic
 */
class TestPolyVox : public CxxTest::TestSuite
{
 public:

  void testLibraryBasics()
  {
    PolyVox::SimpleVolume<uint8_t> volData(
        PolyVox::Region(PolyVox::Vector3DInt32(0u, 0u, 0u),
                        PolyVox::Vector3DInt32(63u, 63u, 63u)));
    volData.setVoxelAt(0, 0, 0, 124u);
    TS_ASSERT_EQUALS(124u, volData.getVoxelAt(0, 0, 0));
  }

 private:
};

#endif  // MODEL_TESTPOLYVOX_H
