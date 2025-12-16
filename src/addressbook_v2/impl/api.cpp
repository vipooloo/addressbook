#include "AddrCenter.h"
#include "api.h"

void Add()
{
    AddrCenter::GetInstance().GetEmailService().AddEmail(EmailDto{"1234", "Test Email", {}});
    AddrCenter::GetInstance().GetEmailService().AddGroup(GroupDto{0, "group1", {1, 2}});
}