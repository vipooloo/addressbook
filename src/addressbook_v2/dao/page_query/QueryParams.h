#ifndef QUERYPARAMS_H
#define QUERYPARAMS_H

#include "ConditionNode.h"
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
      : QueryParams(1, 10, OrderType::ASC, {})
    {}
    QueryParams(uint32_t page, uint32_t page_size, OrderType order_by, std::vector<ConditionNode> conditions)
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
    std::vector<ConditionNode> GetConditions() const
    {
        return m_conditions;
    }

  private:
    uint32_t m_page;
    uint32_t m_page_size;
    OrderType m_order_by;
    std::vector<ConditionNode> m_conditions;
};

#endif  // QUERYPARAMS_H