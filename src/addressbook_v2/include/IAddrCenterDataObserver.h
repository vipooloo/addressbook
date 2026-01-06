#ifndef IADDRCENTERDATAOBSERVER_H
#define IADDRCENTERDATAOBSERVER_H

#include <cstdint>
#include <vector>

enum class ChangeType : uint8_t
{
    kAddEmail,
    kRemoveEmail,
    kUpdateEmail,
    kAddGroup,
};

class IAddrCenterDataObserver
{
  public:
    IAddrCenterDataObserver() = default;
    virtual ~IAddrCenterDataObserver() = default;
    virtual void OnChanged(ChangeType type) = 0;
};

#endif  // IADDRCENTERDATAOBSERVER_H