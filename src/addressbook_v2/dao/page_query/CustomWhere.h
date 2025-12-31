#ifndef CUSTOMWHERE_H
#define CUSTOMWHERE_H

#include <string>
#include <vector>

class CustomWhere
{
  public:
    explicit CustomWhere(const char* where_sql)
      : CustomWhere("", where_sql)
    {
    }
    CustomWhere(const char* sql, const char* where_sql)
      : m_sql{sql}
      , m_whree_sql{where_sql}
      , m_args{}
    {
    }
    ~CustomWhere() = default;
    void AddWhereArg(const std::string& arg)
    {
        m_args.emplace_back(arg);
    }
    const std::string& GetSql() const
    {
        return m_sql;
    }
    const std::string& GetWhereSql() const
    {
        return m_whree_sql;
    }
    const std::vector<std::string>& GetWhereSqlArgs() const
    {
        return m_args;
    }

  private:
    std::string m_sql;
    std::string m_whree_sql;          // 例如: "WHERE (name LIKE ? OR email LIKE ?) AND status = ?"
    std::vector<std::string> m_args;  // 例如: {"%Tom%", "%Tom%", "1"}
};

#endif  // CUSTOMWHERE_H