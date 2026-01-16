#ifndef PAGERESULT_H
#define PAGERESULT_H

#include <cstdint>
#include <memory>
#include <vector>

class AbstractEntity;

class PageResult
{
  public:
    PageResult()
      : PageResult(0, 0)
    {}
    PageResult(size_t cur_page, size_t page_size)
      : m_total_records{0}
      , m_cur_page{cur_page}
      , m_page_size{page_size}
      , m_records{}
    {}

    ~PageResult() = default;

    size_t GetTotalRecords() const
    {
        return m_total_records;
    }

    size_t GetCurrentPage() const
    {
        return m_cur_page;
    }

    size_t GetPageSize() const
    {
        return m_page_size;
    }

    const std::vector<std::shared_ptr<AbstractEntity>>& GetRecords() const
    {
        return m_records;
    }

    void SetTotalRecords(size_t total_records)
    {
        m_total_records = total_records;
    }

    void SetCurrentPage(size_t cur_page)
    {
        m_cur_page = cur_page;
    }

    void SetPageSize(size_t page_size)
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
            m_cur_page = other.m_cur_page;
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
            m_cur_page = other.m_cur_page;
            m_page_size = other.m_page_size;
            m_records = std::move(other.m_records);
        }
    }
    PageResult(const PageResult&) = delete;
    PageResult& operator=(const PageResult&) = delete;

  private:
    size_t m_total_records;                                  ///< 总记录条数 (Total number of records)
    size_t m_cur_page;                                   ///< 当前页码 (Current page number, 1-based)
    size_t m_page_size;                                      ///< 每页容量 (Items per page)
    std::vector<std::shared_ptr<AbstractEntity>> m_records;  ///< 当前页的具体数据列表 (List of records)
};

#endif  // PAGERESULT_H