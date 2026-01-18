#ifndef IADDRESSMGRDATAOBSERVER_H
#define IADDRESSMGRDATAOBSERVER_H

#include <cstdint>
#include <vector>

enum class ChangeType : uint8_t
{
    kAddEmail = 0,
    kRemoveEmail,
    kUpdateEmail,
    kAddGroup,
    kImportEmail,
};

class IAddressMgrDataObserver
{
  public:
    IAddressMgrDataObserver() = default;
    virtual ~IAddressMgrDataObserver() = default;
    virtual void OnChanged(ChangeType type) = 0;
};

#endif  // IADDRESSMGRDATAOBSERVER_H