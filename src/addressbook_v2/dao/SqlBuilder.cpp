#include "SqlBuilder.h"
#include <algorithm>
#include <sstream>

SqlBuilder::SqlBuilder(const std::string& table_name, const std::initializer_list<SqlColumn>& columns)
  : m_table_name{table_name}
  , m_columns{columns}
{
    for (SqlColumn& col : m_columns)
    {
        // TBD
        col.SetKeyId(0);
    }
}

std::string SqlBuilder::DataTypeToString(SqlDataType type) const
{
    static const std::vector<std::string> type_names = {
        "INTEGER",
        "BIGINT",
        "VARCHAR(255)",
        "DOUBLE",
        "TEXT"};

    uint32_t index = static_cast<uint32_t>(type);

    if (index < static_cast<uint32_t>(type_names.size()))
    {
        return type_names[index];
    }
    else
    {
        return "TEXT";
    }
}

const SqlColumn& SqlBuilder::GetColumnName(uint32_t id) const
{
    std::vector<SqlColumn>::const_iterator it = std::find_if(m_columns.cbegin(), m_columns.cend(), [id](const SqlColumn& col) {
        return col.GetKeyId() == id;
    });
    static SqlColumn dummy("", SqlDataType::TEXT, false, false);
    return it != m_columns.cend() ? *it : dummy;
}

const SqlColumn& SqlBuilder::GetColumnId(const std::string& name) const
{
    std::vector<SqlColumn>::const_iterator it = std::find_if(m_columns.cbegin(), m_columns.cend(), [&name](const SqlColumn& col) {
        return col.GetName() == name;
    });
    static SqlColumn dummy("", SqlDataType::TEXT, false, false);
    return it != m_columns.cend() ? *it : dummy;
}

std::string SqlBuilder::BuildCreateTableSql() const
{
    std::string result;
    if (!m_table_name.empty() && !m_columns.empty())
    {
        std::stringstream ss;
        ss << "CREATE TABLE IF NOT EXISTS " << m_table_name << " (\n";

        for (size_t i = 0; i < m_columns.size(); ++i)
        {
            const SqlColumn& col = m_columns[i];
            ss << "    " << col.GetName() << " " << DataTypeToString(col.GetType());

            if (col.IsPrimaryKey())
            {
                ss << " PRIMARY KEY";
            }
            if (col.IsAutoIncrement())
            {
                ss << " AUTOINCREMENT";
            }

            if (i < m_columns.size() - 1)
            {
                ss << ",";
            }
            ss << "\n";
        }
        ss << ");";
        result = ss.str();
    }

    return result;
}

std::string SqlBuilder::BuildInsertSql() const
{
    std::string result;
    if (!m_table_name.empty())
    {
        std::vector<const SqlColumn*> target_cols;
        for (size_t i = 0; i < m_columns.size(); ++i)
        {
            if (!m_columns[i].IsAutoIncrement())
            {
                target_cols.push_back(&m_columns[i]);
            }
        }

        if (!target_cols.empty())
        {
            std::stringstream ss;
            ss << "INSERT INTO " << m_table_name << " (";
            for (size_t i = 0; i < target_cols.size(); ++i)
            {
                ss << target_cols[i]->GetName();
                if (i < target_cols.size() - 1)
                {
                    ss << ", ";
                }
            }

            ss << ") VALUES (";
            for (size_t i = 0; i < target_cols.size(); ++i)
            {
                ss << "?";

                if (i < target_cols.size() - 1)
                {
                    ss << ", ";
                }
            }
            ss << ");";

            result = ss.str();
        }
    }

    return result;
}

std::string SqlBuilder::BuildSelectExceptLastColumn() const
{
    std::string m_result;

    if (m_table_name.empty())
    {
        m_result = "";
    }
    else if (m_columns.size() <= 1)
    {
        // 0 或 1 列时，直接返回 *
        m_result = "SELECT * FROM " + m_table_name + ";";
    }
    else
    {
        std::stringstream m_ss;
        m_ss << "SELECT ";

        size_t m_last_index = m_columns.size() - 1;

        for (size_t i = 0; i < m_last_index; ++i)
        {
            m_ss << m_columns[i].GetName();

            if (i < m_last_index - 1)
            {
                m_ss << ", ";
            }
        }

        m_ss << " FROM " << m_table_name << ";";
        m_result = m_ss.str();
    }

    return m_result;
}
