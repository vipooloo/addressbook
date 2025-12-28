#ifndef PAGERESULT_H
#define PAGERESULT_H

#include "ConditionNode.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class AbstractEntity;

class PageResult
{
  public:
    PageResult()
      : m_total{0}
      , m_page{1}
      , m_page_size{10}
      , m_total_pages{0}
      , m_data{}
    {}
    ~PageResult() = default;
    uint32_t GetTotal() const
    {
        return m_total;
    }
    uint32_t GetPage() const
    {
        return m_page;
    }
    uint32_t GetPageSize() const
    {
        return m_page_size;
    }
    uint32_t GetTotalPages() const
    {
        return m_total_pages;
    }
    std::vector<std::shared_ptr<AbstractEntity>> GetData() const
    {
        return m_data;
    }
    void SetTotal(uint32_t total)
    {
        m_total = total;
    }
    void SetPage(uint32_t page)
    {
        m_page = page;
    }
    void SetPageSize(uint32_t page_size)
    {
        m_page_size = page_size;
    }
    void SetTotalPages(uint32_t total_pages)
    {
        m_total_pages = total_pages;
    }

  private:
    uint32_t m_total;                                     ///< 一共多少条
    uint32_t m_page;                                      ///< 当前第几页
    uint32_t m_page_size;                                 ///< 一页多少条
    uint32_t m_total_pages;                               ///< 一共多少页
    std::vector<std::shared_ptr<AbstractEntity>> m_data;  ///< 当前页的具体数据
};

#endif  // PAGERESULT_H