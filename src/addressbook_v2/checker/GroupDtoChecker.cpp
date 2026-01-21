#include "AddrMgrConfigDefs.h"
#include "AddrMgrLog.h"
#include "GroupDtoChecker.h"
#include <algorithm>
#include <set>

GroupDtoChecker::GroupDtoChecker()
  : m_rules{}
{
}

bool GroupDtoChecker::Verify(const GroupDto& dto) const
{
    bool ret = std::all_of(m_rules.cbegin(), m_rules.cend(), [&dto](const GroupDtoRule& rule) {
        return rule(dto);
    });
    return ret;
}

void GroupDtoChecker::InitInternalRules()
{
    m_rules.emplace_back(std::bind(&GroupDtoChecker::BasickInfoCheck, std::placeholders::_1));
}

bool GroupDtoChecker::BasickInfoCheck(const GroupDto& dto)
{
    bool ret = false;
    do
    {
        if (dto.GetGroupName().empty())
        {
            AB_LOG_E("Name is empty");
            break;
        }
        std::vector<uint32_t> ids = dto.GetMailRids();
        if (std::any_of(ids.cbegin(), ids.cend(), [](uint32_t rid) {
                return rid == 0;
            }))
        {
            AB_LOG_E("Invalid email rid");
            break;
        }
        if (ids.size() > kMaxEmailsPerGroup)
        {
            AB_LOG_E("Too many emails");
            break;
        }
        std::set<uint32_t> rid_set(ids.cbegin(), ids.cend());
        if (rid_set.size() != ids.size())
        {
            AB_LOG_E("Duplicate email rid");
            break;
        }
        ret = true;
    } while (false);
    return ret;
}