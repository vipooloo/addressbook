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
    m_rules.emplace_back(std::bind(&EmailDtoChecker::BasickInfoCheck, std::placeholders::_1));
}

bool EmailDtoChecker::BasickInfoCheck(const EmailDto& dto)
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
        std::vector<uint32_t> ids = dto.GetGroupRids();
        if (std::any_of(ids.cbegin(), ids.cend(), [](uint32_t rid) {
                return rid == 0;
            }))
        {
            AB_LOG_E("Invalid group rid");
            break;
        }
        if (ids.size() > kMaxGroupsPerEmail)
        {
            AB_LOG_E("Too many groups");
            break;
        }
        if (dto.GetGroupNames().size() > kMaxGroupsPerEmail)
        {
            AB_LOG_E("Too many groups");
            break;
        }
        if (dto.GetGroupNames().size() != ids.size())
        {
            AB_LOG_E("Group names and ids size not match");
            break;
        }
        std::set<uint32_t> rid_set(ids.cbegin(), ids.cend());
        if (rid_set.size() != ids.size())
        {
            AB_LOG_E("Duplicate group rid");
            break;
        }
        ret = true;
    } while (false);
    return ret;
}