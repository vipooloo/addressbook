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

class IAddressDataObserver
{
  public:
    IAddressDataObserver() = default;
    virtual ~IAddressDataObserver() = default;
    virtual void OnChanged(ChangeType type) = 0;
};

#endif  // IADDRCENTERDATAOBSERVER_H