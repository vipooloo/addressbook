#ifndef PAGERESULT_H
#define PAGERESULT_H

#include <cstdint>
#include <memory>
#include <vector>

// 前置声明，减少头文件依赖
class AbstractEntity;

class PageResult
{
  public:
    PageResult()
      : PageResult(1, 10)
    {}
    PageResult(uint32_t current_page, uint32_t page_size)
      : m_total_records{0}
      , m_current_page{current_page}
      , m_page_size{page_size}
      , m_records{}
    {}

    ~PageResult() = default;

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

    const std::vector<std::shared_ptr<AbstractEntity>>& GetRecords() const
    {
        return m_records;
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

    void SetRecords(std::vector<std::shared_ptr<AbstractEntity>> records)
    {
        m_records = std::move(records);
    }

    void AddRecord(const std::shared_ptr<AbstractEntity>& record)
    {
        m_records.push_back(record);
    }
    PageResult& operator=(PageResult&& other) noexcept
    {
        if (this != &other)
        {
            m_total_records = other.m_total_records;
            m_current_page = other.m_current_page;
            m_page_size = other.m_page_size;
            m_records = std::move(other.m_records);
        }
        return *this;
    }
    PageResult(PageResult&& other) noexcept
    {
        // 移动构造函数体
        if (this != &other)
        {
            m_total_records = other.m_total_records;
            m_current_page = other.m_current_page;
            m_page_size = other.m_page_size;
            m_records = std::move(other.m_records);
        }
    }
    PageResult(const PageResult&) = delete;
    PageResult& operator=(const PageResult&) = delete;

  private:
    uint32_t m_total_records;                                ///< 总记录条数 (Total number of records)
    uint32_t m_current_page;                                 ///< 当前页码 (Current page number, 1-based)
    uint32_t m_page_size;                                    ///< 每页容量 (Items per page)
    std::vector<std::shared_ptr<AbstractEntity>> m_records;  ///< 当前页的具体数据列表 (List of records)
};

#endif  // PAGERESULT_H