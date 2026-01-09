#ifndef CHECKERPROVIDER_H
#define CHECKERPROVIDER_H

#include "EmailDtoChecker.h"

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

  private:
    CheckerProvider();
    ~CheckerProvider() {}
    CheckerProvider(const CheckerProvider&) = delete;
    CheckerProvider& operator=(const CheckerProvider&) = delete;
    CheckerProvider(CheckerProvider&&) = delete;
    CheckerProvider& operator=(CheckerProvider&&) = delete;

  private:
    EmailDtoChecker m_email_checker;
};
}  // namespace addrbook

#endif  // CHECKERPROVIDER_H
