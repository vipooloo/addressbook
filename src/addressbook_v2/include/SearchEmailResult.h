#ifndef SEARCHEMAILRESULT_H
#define SEARCHEMAILRESULT_H

#include "EmailDto.h"
#include "SearchResult.h"

class SearchEmailResult : public SearchResult
{
  public:
    SearchEmailResult(uint32_t current_page, uint32_t page_size)
      : SearchEmailResult(0, current_page, page_size)
    {}
    SearchEmailResult(uint32_t total_records, uint32_t current_page, uint32_t page_size)
      : SearchResult(total_records, current_page, page_size)
      , m_emails{}
    {}
    ~SearchEmailResult() override = default;

    const std::vector<EmailDto>& GetRecords() const
    {
        return m_emails;
    }

    void SetRecords(const std::vector<EmailDto>& records)
    {
        m_emails = records;
    }

    SearchEmailResult(SearchEmailResult&& other) noexcept
      : SearchResult(std::move(other))
      , m_emails(std::move(other.m_emails))
    {
    }

    SearchEmailResult& operator=(SearchEmailResult&& other) noexcept
    {
        if (this != &other)
        {
            SearchResult::operator=(std::move(other));
            m_emails = std::move(other.m_emails);
        }
        return *this;
    }

  private:
    std::vector<EmailDto> m_emails;
};

#endif  // SEARCHEMAILRESULT_H