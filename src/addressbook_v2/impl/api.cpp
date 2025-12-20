#include "AddrCenter.h"
#include "api.h"

void Add()
{
    AddrCenter::GetInstance().GetEmailService().AddEmail(EmailDto{"1234", "Test Email", {}});
    AddrCenter::GetInstance().GetEmailService().AddGroup(GroupDto{"group1", {}});
    AddrCenter::GetInstance().GetEmailService().AddEmail(EmailDto{"1234", "Test Email", {1, 2}});
    AddrCenter::GetInstance().GetEmailService().AddGroup(GroupDto{"group1", {1, 2}});
    AddrCenter::GetInstance().GetEmailService().RemoveEmail({12, 333332, 33});
}