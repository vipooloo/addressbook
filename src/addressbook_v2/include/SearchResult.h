#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <cstdint>
#include <vector>

class SearchResult
{
  public:
    SearchResult(uint32_t current_page, uint32_t page_size)
      : SearchResult(0, current_page, page_size)
    {}
    SearchResult(uint32_t total_records, uint32_t current_page, uint32_t page_size)
      : m_total_records{total_records}
      , m_current_page{current_page}
      , m_page_size{page_size}
    {}

    virtual ~SearchResult() = default;

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

    SearchResult(SearchResult&& other) noexcept
    {
        m_total_records = other.m_total_records;
        m_current_page = other.m_current_page;
        m_page_size = other.m_page_size;
    }

    SearchResult& operator=(SearchResult&& other) noexcept
    {
        if (this != &other)
        {
            m_total_records = other.m_total_records;
            m_current_page = other.m_current_page;
            m_page_size = other.m_page_size;
        }
        return *this;
    }

    SearchResult(const SearchResult&) = delete;
    SearchResult& operator=(const SearchResult&) = delete;

  private:
    uint32_t m_total_records;  ///< 总记录条数 (Total number of records)
    uint32_t m_current_page;   ///< 当前页码 (Current page number, 1-based)
    uint32_t m_page_size;      ///< 每页容量 (Items per page)
};

#endif  // SEARCHRESULT_H