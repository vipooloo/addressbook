#ifndef QUERYPARAMS_H
#define QUERYPARAMS_H

#include "CustomWhere.h"
#include <cstdint>
#include <vector>

enum class OrderType
{
    ASC,
    DESC
};

class QueryParams
{
  public:
    QueryParams()
      : QueryParams(1, 10, OrderType::ASC, CustomWhere{""})
    {}
    explicit QueryParams(const CustomWhere& condition)
      : QueryParams(1, 10, OrderType::ASC, condition)
    {}
    QueryParams(uint32_t page, uint32_t page_size, const CustomWhere& conditions)
      : QueryParams(page, page_size, OrderType::ASC, conditions)
    {}
    QueryParams(uint32_t page, uint32_t page_size, OrderType order_by, const CustomWhere& conditions)
      : m_page{page}
      , m_page_size{page_size}
      , m_order_by{order_by}
      , m_conditions{conditions}
    {}
    ~QueryParams() = default;
    uint32_t GetPage() const
    {
        return m_page;
    }
    uint32_t GetPageSize() const
    {
        return m_page_size;
    }
    OrderType GetOrderBy() const
    {
        return m_order_by;
    }
    CustomWhere GetConditions() const
    {
        return m_conditions;
    }
    void SetPage(uint32_t page)
    {
        m_page = page;
    }
    void SetPageSize(uint32_t page_size)
    {
        m_page_size = page_size;
    }
    void SetOrderBy(OrderType order_by)
    {
        m_order_by = order_by;
    }
    void SetConditions(const CustomWhere& conditions)
    {
        m_conditions = conditions;
    }

  private:
    uint32_t m_page;
    uint32_t m_page_size;
    OrderType m_order_by;
    CustomWhere m_conditions;
};

#endif  // QUERYPARAMS_H