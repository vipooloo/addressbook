#ifndef CUSTOMWHERE_H
#define CUSTOMWHERE_H

#include <string>
#include <vector>

class CustomWhere
{
  public:
    explicit CustomWhere(const std::string sql)
      : m_sql{sql}
      , m_args{}
    {
    }
    ~CustomWhere() = default;
    void AddArg(const std::string& arg)
    {
        m_args.emplace_back(arg);
    }
    std::string GetSql() const
    {
        return m_sql;
    }
    const std::vector<std::string>& GetArgs() const
    {
        return m_args;
    }

  private:
    std::string m_sql;                // 例如: "WHERE (name LIKE ? OR email LIKE ?) AND status = ?"
    std::vector<std::string> m_args;  // 例如: {"%Tom%", "%Tom%", "1"}
};

#endif  // CUSTOMWHERE_H