#ifndef SQLCOLUMN_H
#define SQLCOLUMN_H

#include <cstdint>
#include <string>

enum class SqlDataType : uint8_t
{
    INT32,
    INT64,
    STRING,
    DOUBLE,
    TEXT
};

class SqlColumn
{
  public:
    SqlColumn(const std::string& name, SqlDataType type, bool is_pk = false, bool is_auto_inc = false)
      : m_key_id{0}
      , m_name{name}
      , m_type{type}
      , m_is_primary_key{is_pk}
      , m_is_auto_increment{is_auto_inc}
    {
    }
    ~SqlColumn() = default;

    uint32_t GetKeyId() const
    {
        return m_key_id;
    }

    void SetKeyId(uint32_t key_id)
    {
        m_key_id = key_id;
    }

    std::string GetName() const
    {
        return m_name;
    }

    SqlDataType GetType() const
    {
        return m_type;
    }

    bool IsPrimaryKey() const
    {
        return m_is_primary_key;
    }

    bool IsAutoIncrement() const
    {
        return m_is_auto_increment;
    }

  private:
    uint32_t m_key_id;
    std::string m_name;
    SqlDataType m_type;
    bool m_is_primary_key;
    bool m_is_auto_increment;
};

#endif  // SQLCOLUMN_H