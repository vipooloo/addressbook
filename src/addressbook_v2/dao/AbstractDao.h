#ifndef ABSTRACTDAO_H
#define ABSTRACTDAO_H

#include "AbstractEntity.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

static constexpr const char* DB_NAME = "addressbook.db";

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

// 邮件组映射表查询条件（通用）
class MailGroupMappingQueryCond
{
  public:
    MailGroupMappingQueryCond()
      : MailGroupMappingQueryCond(0, 0, 0)
    {}
    MailGroupMappingQueryCond(uint32_t rid, uint32_t mail_rid, uint32_t group_rid)
      : m_rid{rid}
      , m_mail_rid{mail_rid}
      , m_group_rid{group_rid}
    {}
    ~MailGroupMappingQueryCond() = default;

    void SetRid(uint32_t rid)
    {
        m_rid = rid;
    }
    uint32_t GetRid() const
    {
        return m_rid;
    }

    void SetMailRid(uint32_t mail_rid)
    {
        m_mail_rid = mail_rid;
    }
    uint32_t GetMailRid() const
    {
        return m_mail_rid;
    }

    void SetGroupRid(uint32_t group_rid)
    {
        m_group_rid = group_rid;
    }
    uint32_t GetGroupRid() const
    {
        return m_group_rid;
    }

    bool InValid() const
    {
        return 0 == m_rid && 0 == m_mail_rid && 0 == m_group_rid;
    }

  private:
    uint32_t m_rid;
    uint32_t m_mail_rid;
    uint32_t m_group_rid;
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
    AbstractDao()
    {}
    virtual ~AbstractDao() = default;
    static SQLite::Database& GetDb()
    {
        static SQLiteConn db(DB_NAME);
        return db.GetDb();
    }
    virtual bool Init()
    {
        return true;
    }

    virtual size_t GetCount() const
    {
        return 0;
    }
    virtual size_t CountByCond(const MailGroupMappingQueryCond& cond) const
    {
        return 0;
    }

    // ========== 基础 CRUD 纯虚接口 ==========
    virtual bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr)
    {
        return false;
    }
    virtual bool InsertBatch(const std::vector<std::shared_ptr<AbstractEntity>>& items)
    {
        return false;
    }
    virtual bool IsExist(const std::vector<uint32_t>& rids)
    {
        return false;
    }
    virtual bool Remove(const std::vector<uint32_t>& rids)
    {
        return false;
    }
    virtual bool RemoveAll()
    {
        return false;
    }
    /*---------------------*/
    virtual DaoErrCode Update(const AbstractEntity& entity)
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
    bool OnExecuteSql(const std::string& sql);
    size_t OnGetCount(const std::string& table_name) const;
    uint32_t CalcPageOffset(const QueryParams& params) const;
    DaoErrCode ValidatePageParams(const QueryParams& params) const;
    std::string BuildWhereClause(const std::vector<ConditionNode>& conditions, SQLite::Statement& stmt, uint32_t& param_idx) const;
    static std::string JoinIds(const std::vector<uint32_t>& rids);
};

#endif  // ABSTRACTDAO_H