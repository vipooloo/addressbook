#ifndef ABSTRACTDAO_H
#define ABSTRACTDAO_H

#include "AbstractEntity.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

// 1. 错误码枚举（强类型）
enum class DaoErrCode : uint8_t
{
    SUCCESS = 0,     ///< 成功
    UNSUPPORTED,     ///< 不支持的操作
    SQLITE_ERROR,    ///< SQLite 错误
    INVALID_PARAM,   ///< 参数非法
    NOT_FOUND,       ///< 数据未找到
    INTERNAL_ERROR,  ///< 内部错误
    UNKNOWN,         ///< 未知错误
};

struct ConditionNode
{
    std::string field;  // 字段名
    std::string op;     // 操作符（如 "=", "LIKE"）
    std::string value;  // 值
};

struct QueryParams
{
    int page = 1;
    int page_size = 10;
    std::string order_by;
    std::vector<ConditionNode> conditions;
};

template<typename T>
struct PageResult
{
    std::vector<T> data;
    int total = 0;
    int page = 1;
    int page_size = 10;
    int total_pages = 0;
};

class SQLiteConn
{
  public:
    explicit SQLiteConn(const std::string& db_path)
      : m_db(db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
    {
    }

    SQLite::Database& GetDb()
    {
        return m_db;
    }

  private:
    SQLite::Database m_db;
};

class AbstractDao
{
  public:
    AbstractDao(const std::string& tbl_name, const std::shared_ptr<SQLiteConn>& db_sptr)
      : m_table_name{tbl_name}
      , m_conn_sptr{db_sptr}
    {}
    virtual ~AbstractDao() = default;

    virtual bool Init()
    {
        return true;
    }

    uint32_t GetCount() const;

    // ========== 基础 CRUD 纯虚接口 ==========
    virtual DaoErrCode Insert(const AbstractEntity& entity)
    {
        return DaoErrCode::UNSUPPORTED;
    }
    virtual DaoErrCode Update(const AbstractEntity& entity)
    {
        return DaoErrCode::UNSUPPORTED;
    }
    virtual DaoErrCode Remove(uint32_t rid)
    {
        return DaoErrCode::UNSUPPORTED;
    }
    virtual std::pair<DaoErrCode, std::shared_ptr<AbstractEntity>> FindById(uint32_t rid)
    {
        return {DaoErrCode::UNSUPPORTED, nullptr};
    }
    virtual std::pair<DaoErrCode, std::vector<std::shared_ptr<AbstractEntity>>> FindAll()
    {
        return {DaoErrCode::UNSUPPORTED, {}};
    }

    // ========== 通用查询/删除接口 ==========
    virtual std::pair<DaoErrCode, PageResult<std::shared_ptr<AbstractEntity>>> FindByPage(const QueryParams& params)
    {
        return {DaoErrCode::UNSUPPORTED, {}};
    }
    virtual DaoErrCode RemoveByConditions(const std::vector<ConditionNode>& conditions)
    {
        return DaoErrCode::UNSUPPORTED;
    }

  protected:
    SQLite::Database& GetDb() const
    {
        static SQLite::Database s_db = SQLite::Database("");
        SQLite::Database* db_ptr = &s_db;
        if (m_conn_sptr)
        {
            db_ptr = &(m_conn_sptr->GetDb());
        }
        return *db_ptr;
    }
    uint32_t CalcPageOffset(const QueryParams& params) const;
    DaoErrCode ValidatePageParams(const QueryParams& params) const;
    std::string BuildWhereClause(const std::vector<ConditionNode>& conditions, SQLite::Statement& stmt, uint32_t& param_idx) const;

  protected:
    std::string m_table_name;
    std::shared_ptr<SQLiteConn> m_conn_sptr;
};

#endif  // ABSTRACTDAO_H