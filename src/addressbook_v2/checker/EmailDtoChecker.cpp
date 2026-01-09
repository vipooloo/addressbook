#include "AddrMgrConfigDefs.h"
#include "AddrMgrLog.h"
#include "EmailDtoChecker.h"
#include <algorithm>
#include <set>

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
    m_rules.emplace_back(std::bind(&EmailDtoChecker::BasickInfoCheck, this, std::placeholders::_1));
}

bool EmailDtoChecker::BasickInfoCheck(const EmailDto& dto) const
{
    bool ret = false;
    do
    {
        if (dto.GetName().empty())
        {
            AB_LOG_E("Name is empty");
            break;
        }
        if (dto.GetAddress().empty())
        {
            AB_LOG_E("Email is empty");
            break;
        }
        std::vector<uint32_t> rids = dto.GetGroupRids();
        if (std::any_of(rids.cbegin(), rids.cend(), [](uint32_t rid) {
                return rid == 0;
            }))
        {
            AB_LOG_E("Invalid group rid");
            break;
        }
        if (rids.size() > kMaxGroupsPerEmail)
        {
            AB_LOG_E("Too many groups");
            break;
        }
        std::set<uint32_t> rid_set(rids.cbegin(), rids.cend());
        if (rid_set.size() != rids.size())
        {
            AB_LOG_E("Duplicate group rid");
            break;
        }
        ret = true;
    } while (false);
    return ret;
}