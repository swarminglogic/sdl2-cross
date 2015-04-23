#ifndef UTIL_TESTUUID_H
#define UTIL_TESTUUID_H

#include <boost/functional/hash/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <algorithm>
#include <unordered_set>
#include <util/Uuid.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Uuid class.
 *
 * @author SwarmingLogic
 */
class TestUuid : public CxxTest::TestSuite
{
 public:
  void testBasicUuid()
  {
    Uuid id1;
    Uuid id2;
    TS_ASSERT_DIFFERS(id1, id2);
    TS_ASSERT_EQUALS(id1, id1);
  }


  void testManyUnorderedSet()
  {
    std::unordered_set<Uuid, boost::hash<boost::uuids::uuid>> uuids;
    for (size_t i = 0 ; i < N_ ; ++i)
      uuids.emplace(local_uuids_[i]);
    TS_ASSERT_EQUALS(uuids.size(), N_);
  }

  void testManyVectorSort()
  {
    // Using sort and linear traversal for uniqueness
    std::vector<Uuid> uuids = local_uuids_;
    sort(uuids.begin(), uuids.end());
    for (size_t i = 0 ; i < uuids.size()-1 ; ++i) {
      TS_ASSERT_DIFFERS(uuids[i], uuids[i+1]);
    }
  }

  void testManyVectorUnique()
  {
    std::vector<Uuid> uuids = local_uuids_;
    unique(uuids.begin(), uuids.end());
    TS_ASSERT_EQUALS(uuids.size(), N_);
  }


  void testManySet()
  {
    std::set<Uuid> uuids;
    for (size_t i = 0 ; i < N_ ; ++i) {
      Uuid id = local_uuids_[i];
      TS_ASSERT_EQUALS(uuids.find(id), uuids.end());
      uuids.emplace(id);
    }
  }


  void testManySetUnique()
  {
    std::set<Uuid> uuids;
    for (size_t i = 0 ; i < N_ ; ++i)
      uuids.emplace(local_uuids_[i]);
    TS_ASSERT_EQUALS(uuids.size(), N_);
  }

  static const size_t N_ = 1000u;
  static const std::vector<Uuid> local_uuids_;
};

const std::vector<Uuid> TestUuid::local_uuids_(N_);
#endif  // UTIL_TESTUUID_H
