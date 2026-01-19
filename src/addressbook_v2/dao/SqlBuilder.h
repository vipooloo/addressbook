#ifndef SQLBUILDER_H
#define SQLBUILDER_H

#include "SqlColumn.h"
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <string>
#include <vector>

class SqlBuilder
{
  public:
    SqlBuilder()
      : SqlBuilder("", {})
    {
    }

    SqlBuilder(const std::string& table_name, const std::initializer_list<SqlColumn>& columns);

    ~SqlBuilder() = default;

    void SetTableName(const std::string& table_name)
    {
        m_table_name = table_name;
    }

    void AddColumn(const std::string& name, SqlDataType type, bool is_pk = false, bool is_auto_inc = false)
    {
        m_columns.push_back(SqlColumn(name, type, is_pk, is_auto_inc));
    }
    const SqlColumn& GetColumnName(uint32_t id) const;
    const SqlColumn& GetColumnId(const std::string& name) const;

    std::string BuildCreateTableSql() const;
    std::string BuildInsertSql() const;
    std::string BuildSelectExceptLastColumn() const;

  private:
    std::string DataTypeToString(SqlDataType type) const;

  private:
    std::string m_table_name;
    std::vector<SqlColumn> m_columns;
};

#endif  // SQLBUILDER_H