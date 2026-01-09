#ifndef EMAILDTOCHECKER_H
#define EMAILDTOCHECKER_H

#include "EmailDto.h"
#include <functional>

class EmailDtoChecker
{
  public:
    using EmailDtoRule = std::function<bool(const EmailDto&)>;

    EmailDtoChecker();
    ~EmailDtoChecker() = default;
    bool Verify(const EmailDto& dto) const;

  private:
    void InitInternalRules();
    bool BasickInfoCheck(const EmailDto& dto) const;

  private:
    std::vector<EmailDtoRule> m_rules;
};

#endif  // EMAILDTOCHECKER_H