#ifndef SEARCHEMAILRESULT_H
#define SEARCHEMAILRESULT_H

#include "EmailDto.h"
#include "PageQueryResult.h"

class EmailPageResult : public PageQueryResult
{
  public:
    EmailPageResult(uint32_t cur_page, uint32_t page_size)
      : EmailPageResult(0, cur_page, page_size)
    {}
    EmailPageResult(uint32_t total_records, uint32_t cur_page, uint32_t page_size)
      : PageQueryResult(total_records, cur_page, page_size)
      , m_emails{}
    {}
    ~EmailPageResult() override = default;

    const std::vector<EmailDto>& GetRecords() const
    {
        return m_emails;
    }

    void SetRecords(const std::vector<EmailDto>& records)
    {
        m_emails = records;
    }

    EmailPageResult(EmailPageResult&& other) noexcept
      : PageQueryResult(std::move(other))
      , m_emails(std::move(other.m_emails))
    {
    }

    EmailPageResult& operator=(EmailPageResult&& other) noexcept
    {
        if (this != &other)
        {
            PageQueryResult::operator=(std::move(other));
            m_emails = std::move(other.m_emails);
        }
        return *this;
    }

  private:
    std::vector<EmailDto> m_emails;
};

#endif  // SEARCHEMAILRESULT_H