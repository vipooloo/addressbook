#ifndef CONDITIONNODE_H
#define CONDITIONNODE_H

#include <string>

class ConditionNode
{
  public:
    ConditionNode(const std::string& field, const std::string& op, const std::string& value)
      : m_field{field}
      , m_op{op}
      , m_value{value}
    {}
    ~ConditionNode() = default;
    std::string GetField() const
    {
        return m_field;
    }
    std::string GetOp() const
    {
        return m_op;
    }
    std::string GetValue() const
    {
        return m_value;
    }

  private:
    std::string m_field;  // 字段名
    std::string m_op;     // 操作符（如 "=", "LIKE"）
    std::string m_value;  // 值
};

#endif  // CONDITIONNODE_H