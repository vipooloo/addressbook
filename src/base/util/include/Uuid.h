#ifndef UUID_H
#define UUID_H

#include <stdint.h>
#include <string>

namespace BW {
class Uuid
{
  public:
    static std::string generateUUID();
    static uint64_t getNextUUID();
};
}  // namespace BW

#endif  // UUID_H