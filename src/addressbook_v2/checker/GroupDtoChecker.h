#ifndef GROUPDTOCHECKER_H
#define GROUPDTOCHECKER_H

#include "GroupDto.h"
#include <functional>

class GroupDtoChecker
{
  public:
    using GroupDtoRule = std::function<bool(const GroupDto&)>;

    GroupDtoChecker();
    ~GroupDtoChecker() = default;
    bool Verify(const GroupDto& dto) const;

  private:
    void InitInternalRules();
    bool BasickInfoCheck(const GroupDto& dto) const;

  private:
    std::vector<GroupDtoRule> m_rules;
};

#endif  // GROUPDTOCHECKER_H