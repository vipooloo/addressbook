#ifndef IADDRESSMGRDATAOBSERVER_H
#define IADDRESSMGRDATAOBSERVER_H

#include <cstdint>

enum class ChangeType : uint8_t
{
    CreateEmail = 0x00,
    DeleteEmails,
    DeleteAllEmails,
    UpdateEmail,
    ImportEmails,
    CreateGroup = 0x10,
    DeleteGroups,
    DeleteAllGroups,
    UpdateGroup,
};

class IAddressMgrDataObserver
{
  public:
    IAddressMgrDataObserver() = default;
    virtual ~IAddressMgrDataObserver() = default;
    virtual void OnChanged(ChangeType type) = 0;
};

#endif  // IADDRESSMGRDATAOBSERVER_H