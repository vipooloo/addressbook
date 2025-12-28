#ifndef ABSTRACTDAO_H
#define ABSTRACTDAO_H

#include "AbstractEntity.h"
#include "AddressBookConfigDefs.h"
#include "ConditionNode.h"
#include "PageResult.h"
#include "QueryParams.h"
#include "StmtParam.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

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
    explicit AbstractDao(const std::string& table_name);

    virtual ~AbstractDao() = default;
    static SQLite::Database& GetDb()
    {
        static SQLiteConn db(DB_NAME);
        return db.GetDb();
    }
    virtual bool Create()
    {
        return true;
    }
    virtual size_t GetCount() const;
    virtual bool Insert(const std::shared_ptr<AbstractEntity>& in_entity_sptr, const std::shared_ptr<AbstractEntity>& out_entity_sptr)
    {
        (void)in_entity_sptr;
        (void)out_entity_sptr;
        return false;
    }
    virtual bool InsertBatch(const std::vector<std::shared_ptr<AbstractEntity>>& items)
    {
        (void)items;
        return false;
    }
    virtual bool IsExist(const std::vector<uint32_t>& rids);
    virtual bool Remove(const std::vector<uint32_t>& rids);
    virtual bool RemoveAll()
    {
        return OnExecuteSql(m_sql_delete_all);
    }

    virtual bool Update(const std::shared_ptr<AbstractEntity>& entity_sptr)
    {
        return false;
    }

    virtual std::pair<bool, PageResult> FindAll()
    {
        return {false, {}};
    }

    virtual std::pair<bool, PageResult> FindByPage(const QueryParams& params)
    {
        return {false, {}};
    }

  protected:
    bool OnExecuteSql(const std::string& sql) const
    {
        std::vector<StmtParam> stmt_params;
        return OnExecuteSql(sql, stmt_params);
    }
    bool OnExecuteSql(const std::string& sql, const std::vector<StmtParam>& stmt_params) const;
    bool OnExecuteSql(const std::string& sql, const std::vector<std::vector<StmtParam>>& stmt_params_vec) const;

    uint32_t CalcPageOffset(const QueryParams& params) const;
    bool ValidatePageParams(const QueryParams& params) const;
    std::string BuildWhereClause(const std::vector<ConditionNode>& conditions, SQLite::Statement& stmt, uint32_t& param_idx) const;

  private:
    static bool BindStmtParams(SQLite::Statement& stmt, const std::vector<StmtParam>& stmt_params);

  private:
    std::string m_table_name;
    std::string m_sql_count;
    std::string m_sql_delete_by_rid;
    std::string m_sql_delete_all;
};

#endif  // ABSTRACTDAO_H