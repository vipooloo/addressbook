#ifndef PAGEQUERYPARAM_H
#define PAGEQUERYPARAM_H

#include <cstdint>
#include <string>

enum class OrderType : uint8_t
{
    ASC,
    DESC
};

class PageQueryParam
{
  public:
    PageQueryParam(uint32_t cur_page, uint32_t page_size)
      : PageQueryParam("", cur_page, page_size)
    {}
    PageQueryParam(const std::string& keyword, uint32_t cur_page, uint32_t page_size)
      : PageQueryParam(keyword, cur_page, page_size, OrderType::ASC)
    {}
    PageQueryParam(uint32_t cur_page, uint32_t page_size, OrderType order_by)
      : PageQueryParam("", cur_page, page_size, order_by)
    {}
    PageQueryParam(const std::string& keyword, uint32_t cur_page, uint32_t page_size, OrderType order_by)
      : m_keyword{keyword}
      , m_cur_page{cur_page}
      , m_page_size{page_size}
      , m_order_by{order_by}
    {}

    ~PageQueryParam() = default;
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

#endif  // PAGEQUERYPARAM_H