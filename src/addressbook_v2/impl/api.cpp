#include "AddrCenter.h"
#include "EmailEntity.h"
#include "api.h"
#include <iostream>

void Add()
{
    AddrCenter::GetInstance().GetEmailService().AddGroup(GroupDto{"group1", {}});
    AddrCenter::GetInstance().GetEmailService().AddGroup(GroupDto{"group2", {}});
    AddrCenter::GetInstance().GetEmailService().AddEmail(EmailDto{"email1", "email1", {}});
    AddrCenter::GetInstance().GetEmailService().AddEmail(EmailDto{"email2", "email12", {1, 2}});
    AddrCenter::GetInstance().GetEmailService().UpdateEmail(EmailDto{1, "XXXXX", "YYY", {3, 4}});
    AddrCenter::GetInstance().GetEmailService().RemoveEmail({12, 333332, 33});
    PageResult result = AddrCenter::GetInstance().GetEmailService().SearchEmail("ai", 1, 2);
    std::cout << "总记录条数:" << result.GetTotalRecords() << std::endl;
    std::cout << "当前页码:" << result.GetCurrentPage() << std::endl;
    std::cout << "每页容量:" << result.GetPageSize() << std::endl;

    auto items = result.GetRecords();
    for (auto& item : items)
    {
        std::shared_ptr<EmailEntity> email_sptr = std::dynamic_pointer_cast<EmailEntity>(item);
        if (email_sptr)
        {
            std::cout << email_sptr->GetEmailAddress() << "  ";
            std::cout << email_sptr->GetEmailName() << "  ";
            std::cout << std::endl;
        }
    }
}