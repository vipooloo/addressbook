#include "EmailDtoChecker.h"
#include <algorithm>

EmailDtoChecker::EmailDtoChecker()
  : m_rules{}
{
}

bool EmailDtoChecker::Verify(const EmailDto& dto) const
{
    bool ret = std::all_of(m_rules.cbegin(), m_rules.cend(), [&dto](const EmailDtoRule& rule) {
        return rule(dto);
    });
    return ret;
}

void EmailDtoChecker::InitInternalRules()
{
}