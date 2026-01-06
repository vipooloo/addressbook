#ifndef ADDRCENTERSEARCHRESULT_H
#define ADDRCENTERSEARCHRESULT_H

#include "EmailDto.h"
#include <vector>

class AddrCenterSearchResult
{
  public:
    AddrCenterSearchResult(uint32_t current_page, uint32_t page_size)
      : AddrCenterSearchResult(0, current_page, page_size)
    {}
    AddrCenterSearchResult(uint32_t total_records, uint32_t current_page, uint32_t page_size)
      : m_total_records{total_records}
      , m_current_page{current_page}
      , m_page_size{page_size}
      , m_emails{}
    {}
    ~AddrCenterSearchResult() = default;
    uint32_t GetTotalRecords() const
    {
        return m_total_records;
    }

    uint32_t GetCurrentPage() const
    {
        return m_current_page;
    }

    uint32_t GetPageSize() const
    {
        return m_page_size;
    }

    const std::vector<EmailDto>& GetRecords() const
    {
        return m_emails;
    }

    uint32_t GetTotalPages() const
    {
        uint32_t total_pages = 0;
        if (m_page_size != 0)
        {
            total_pages = (m_total_records + m_page_size - 1) / m_page_size;
        }
        return total_pages;
    }

    void SetTotalRecords(uint32_t total_records)
    {
        m_total_records = total_records;
    }
    void SetCurrentPage(uint32_t current_page)
    {
        m_current_page = current_page;
    }

    void SetPageSize(uint32_t page_size)
    {
        m_page_size = page_size;
    }

    void SetRecords(const std::vector<EmailDto>& records)
    {
        m_emails = records;
    }
    AddrCenterSearchResult& operator=(AddrCenterSearchResult&& other) noexcept
    {
        if (this != &other)
        {
            m_total_records = other.m_total_records;
            m_current_page = other.m_current_page;
            m_page_size = other.m_page_size;
            m_emails = std::move(other.m_emails);
        }
        return *this;
    }
    AddrCenterSearchResult(AddrCenterSearchResult&& other) noexcept
    {
        // 移动构造函数体
        if (this != &other)
        {
            m_total_records = other.m_total_records;
            m_current_page = other.m_current_page;
            m_page_size = other.m_page_size;
            m_emails = std::move(other.m_emails);
        }
    }
    AddrCenterSearchResult(const AddrCenterSearchResult&) = delete;
    AddrCenterSearchResult& operator=(const AddrCenterSearchResult&) = delete;

  private:
    uint32_t m_total_records;  ///< 总记录条数 (Total number of records)
    uint32_t m_current_page;   ///< 当前页码 (Current page number, 1-based)
    uint32_t m_page_size;      ///< 每页容量 (Items per page)
    std::vector<EmailDto> m_emails;
};

#endif  // ADDRCENTERSEARCHRESULT_H