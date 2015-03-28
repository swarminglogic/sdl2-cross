#include <util/WordGeneratorFactory.h>

#include <boost/preprocessor.hpp>

#include <util/Assert.h>
#include <util/WordGenerator.h>


// Note: Extending this range increases compilation time!

#define RANGE ((1)(2)(3)(4)(5)(6))
#define MACRO(r, p)                   \
    if (BOOST_PP_SEQ_ELEM(0, p) == N) \
      local_ref.reset(                \
        new WordGenerator             \
           <BOOST_PP_TUPLE_REM_CTOR(1, BOOST_PP_SEQ_TO_TUPLE(p))>(N_init));

WordGeneratorPtr WordGeneratorFactory::create(int N, int N_init)
{
  WordGeneratorPtr local_ref = nullptr;
  BOOST_PP_SEQ_FOR_EACH_PRODUCT(MACRO, RANGE)
  return local_ref;
}


#undef MACRO
#undef RANGE


const int WordGeneratorFactory::MAX_ORDER;
