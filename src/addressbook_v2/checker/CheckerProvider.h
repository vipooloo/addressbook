#ifndef CHECKERPROVIDER_H
#define CHECKERPROVIDER_H

#include "EmailDtoChecker.h"
#include "GroupDtoChecker.h"

namespace addrbook {
class CheckerProvider
{
  public:
    static CheckerProvider& GetInstance()
    {
        static CheckerProvider instance;
        return instance;
    }

    bool Verify(const EmailDto& dto) const
    {
        return m_email_checker.Verify(dto);
    }
    bool Verify(const GroupDto& dto) const
    {
        return m_group_checker.Verify(dto);
    }

  private:
    CheckerProvider();
    ~CheckerProvider() {}
    CheckerProvider(const CheckerProvider&) = delete;
    CheckerProvider& operator=(const CheckerProvider&) = delete;
    CheckerProvider(CheckerProvider&&) noexcept = delete;
    CheckerProvider& operator=(CheckerProvider&&) noexcept = delete;

  private:
    EmailDtoChecker m_email_checker;
    GroupDtoChecker m_group_checker;
};
}  // namespace addrbook

#endif  // CHECKERPROVIDER_H
