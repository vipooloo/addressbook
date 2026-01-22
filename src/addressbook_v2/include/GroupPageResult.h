#ifndef GROUPPAGERESULT_H
#define GROUPPAGERESULT_H

#include "GroupDto.h"
#include "PageQueryResult.h"

class GroupPageResult : public PageQueryResult
{
  public:
    GroupPageResult(uint32_t cur_page, uint32_t page_size)
      : GroupPageResult(0, cur_page, page_size)
    {}
    GroupPageResult(uint32_t total_records, uint32_t cur_page, uint32_t page_size)
      : PageQueryResult(total_records, cur_page, page_size)
      , m_groups{}
    {}
    ~GroupPageResult() override = default;

    const std::vector<GroupDto>& GetRecords() const
    {
        return m_groups;
    }

    void SetRecords(const std::vector<GroupDto>& records)
    {
        m_groups = records;
    }

    GroupPageResult(GroupPageResult&& other) noexcept
      : PageQueryResult(std::move(other))
      , m_groups(std::move(other.m_groups))
    {
    }

    GroupPageResult& operator=(GroupPageResult&& other) noexcept
    {
        if (this != &other)
        {
            PageQueryResult::operator=(std::move(other));
            m_groups = std::move(other.m_groups);
        }
        return *this;
    }

  private:
    std::vector<GroupDto> m_groups;
};

#endif  // GROUPPAGERESULT_H