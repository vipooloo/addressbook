#include "AddressManager.h"
#include "api.h"
#include <iostream>

void Add()
{
    AddressManager::AddGroup(GroupDto{"group1", {}});
    AddressManager::AddGroup(GroupDto{"group2", {}});
    AddressManager::CreateEmail(EmailDto{"email1", "email1"});
    AddressManager::CreateEmail(EmailDto{"email2", "email12", {1, 2}});
    AddressManager::UpdateEmail(EmailDto{1, "XXXXX", "YYY", {3, 4}});
    AddressManager::DeleteEmails({12, 333332, 33});
    std::pair<ResultCode, EmailPageResult> search_result = AddressManager::PageQueryEmail(PageQueryParam("ai", 1, 2));
    EmailPageResult& result = search_result.second;
    std::cout << "总记录条数:" << result.GetTotalRecords() << std::endl;
    std::cout << "总页码数:" << result.GetTotalPages() << std::endl;
    std::cout << "当前页码:" << result.GetCurrentPage() << std::endl;
    std::cout << "每页容量:" << result.GetPageSize() << std::endl;

    auto items = result.GetRecords();
    for (auto& item : items)
    {
        std::cout << item.GetRid() << "  ";
        std::cout << item.GetAddress() << "  ";
        std::cout << item.GetName() << "  ";
        const std::vector<uint32_t>& group_rids = item.GetGroupRids();
        const std::vector<std::string>& group_names = item.GetGroupNames();
        for (size_t i = 0; i < group_rids.size(); ++i)
        {
            std::cout << "[ ";
            std::cout << group_rids[i] << "  ";
            std::cout << group_names[i] << "  ";
            std::cout << " ]";
        }
        std::cout << std::endl;
    }

    ImportExportCallback callback = [](const std::string& file_name, bool) {
        std::cout << "文件名:" << file_name << std::endl;
    };
    AddressManager::ExportEmails(callback);

    AddressManager::ImportEmails("1.csv", callback);
}