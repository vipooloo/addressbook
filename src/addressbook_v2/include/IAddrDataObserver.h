#ifndef IADDRDATAOBSERVER_H
#define IADDRDATAOBSERVER_H

#include <cstdint>
#include <vector>

enum class ChangeType
{
    kAddEmail,
    kRemoveEmail,
    kUpdateEmail,
    kAddGroup,
    // ... 其他类型
};

class IAddrDataObserver
{
  public:
    IAddrDataObserver() = default;
    virtual ~IAddrDataObserver() = default;
    virtual void OnChanged(ChangeType type) = 0;
};

#endif  // IADDRDATAOBSERVER_H