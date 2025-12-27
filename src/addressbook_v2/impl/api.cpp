#include "AddrCenter.h"
#include "api.h"

void Add()
{
    AddrCenter::GetInstance().GetEmailService().AddGroup(GroupDto{"group1", {}});
    AddrCenter::GetInstance().GetEmailService().AddGroup(GroupDto{"group2", {}});
    AddrCenter::GetInstance().GetEmailService().AddEmail(EmailDto{"email1", "email1", {}});
    AddrCenter::GetInstance().GetEmailService().AddEmail(EmailDto{"email2", "email12", {1, 2}});
    AddrCenter::GetInstance().GetEmailService().UpdateEmail(EmailDto{1, "XXXXX", "YYY", {3, 4}});
    //AddrCenter::GetInstance().GetEmailService().RemoveEmail({12, 333332, 33});
}