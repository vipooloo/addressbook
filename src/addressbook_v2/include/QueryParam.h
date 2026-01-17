#ifndef QUERYPARAM_H
#define QUERYPARAM_H

#include <cstdint>
#include <string>

enum class OrderType : uint8_t
{
    ASC,
    DESC
};

class QueryParam
{
  public:
    QueryParam(uint32_t cur_page, uint32_t page_size)
      : QueryParam("", cur_page, page_size)
    {}
    QueryParam(const std::string& keyword, uint32_t cur_page, uint32_t page_size)
      : QueryParam(keyword, cur_page, page_size, OrderType::ASC)
    {}
    QueryParam(uint32_t cur_page, uint32_t page_size, OrderType order_by)
      : QueryParam("", cur_page, page_size, order_by)
    {}
    QueryParam(const std::string& keyword, uint32_t cur_page, uint32_t page_size, OrderType order_by)
      : m_keyword{keyword}
      , m_cur_page{cur_page}
      , m_page_size{page_size}
      , m_order_by{order_by}
    {}

    ~QueryParam() = default;
    const std::string& GetKeyword() const
    {
        return m_keyword;
    }
    uint32_t GetCurPage() const
    {
        return m_cur_page;
    }
    uint32_t GetPageSize() const
    {
        return m_page_size;
    }
    OrderType GetOrderBy() const
    {
        return m_order_by;
    }

  private:
    std::string m_keyword;
    uint32_t m_cur_page;
    uint32_t m_page_size;
    OrderType m_order_by;
};

#endif  // QUERYPARAM_H