#ifndef UTIL_RAIIHELPER_H
#define UTIL_RAIIHELPER_H

#include <memory>


/**
 * RAII helper macro wrappers using std::unique_pointer
 *
 * example:
 *   CREATE_RAII_UP(MyClass*, destructorFunction) MyClassPtr
 *   Creates a typedef of MyClassPtr, which is a unique_ptr<MyClass>,
 *   that class destructorFunction upon deletion.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
#define CREATE_RAII_UP(OBJ, DEST)                           \
  struct OBJ ## Deleter {                                \
    void operator()( OBJ* ptr ) { if (ptr) DEST( ptr );} \
  };                                                     \
  typedef std::unique_ptr<OBJ, OBJ ## Deleter>


// Same as above, but with shared ptr.
#define CREATE_RAII_SP(OBJ, DEST)                           \
  struct OBJ ## Deleter {                                \
    void operator()( OBJ* ptr ) { if (ptr) DEST( ptr );} \
  };                                                     \
  typedef std::shared_ptr<OBJ, OBJ ## Deleter>


/**
 * RAII helper macro wrappers using std::unique_pointer
 *
 * For destructors that take reference, rather than pointer.
 *
 * example:
 *   CREATE_RAII_UP(MyClass*, destructorFunction) MyClassPtr
 *   Creates a typedef of MyClassPtr, which is a unique_ptr<MyClass>,
 *   that class destructorFunction upon deletion.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
#define CREATE_RAII2_UP(OBJ, DEST)                           \
  struct OBJ ## Deleter {                                 \
    void operator()( OBJ* ptr ) { if (ptr) DEST( *ptr );} \
  };                                                      \
  typedef std::unique_ptr<OBJ, OBJ ## Deleter>


#endif  // UTIL_RAIIHELPER_H
