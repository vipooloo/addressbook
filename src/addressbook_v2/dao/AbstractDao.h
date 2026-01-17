#ifndef ABSTRACTDAO_H
#define ABSTRACTDAO_H

#include "AbstractEntity.h"
#include "AddrMgrConfigDefs.h"
#include "CustomWhere.h"
#include "PageQueryParam.h"
#include "PageResult.h"
#include "StmtParam.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace addrbook {

class AbstractDao
{
  public:
    explicit AbstractDao(const std::string& table_name);

    virtual ~AbstractDao() = default;
    static SQLite::Database& GetDb();
    virtual bool Create()
    {
        return true;
    }
    virtual size_t GetCount() const;
    virtual bool IsExist(const std::vector<uint32_t>& ids) const;
    virtual bool Remove(const std::vector<uint32_t>& ids);
    virtual bool RemoveAll()
    {
        return OnExecuteSql(m_sql_delete_all);
    }

  protected:
    bool OnExecuteSql(const std::string& sql) const;
    bool OnExecuteSql(const std::string& sql, const std::vector<StmtParam>& stmt_params) const;
    bool OnExecuteSql(const std::string& sql, const std::vector<std::vector<StmtParam>>& stmt_params_vec) const;

    uint32_t CalcPageOffset(const PageQueryParam& params) const
    {
        return (params.GetCurPage() - 1) * params.GetPageSize();
    }
    void BindWhereParams(SQLite::Statement& stmt, const std::vector<std::string>& args, uint32_t start_idx) const;
    PageResult DoFindByPage(const PageQueryParam& params, const CustomWhere& conditions);
    virtual std::shared_ptr<AbstractEntity> OnCreateEntity(const SQLite::Statement& stmt)
    {
        static_cast<void>(&stmt);
        return nullptr;
    }

  private:
    static bool BindStmtParams(SQLite::Statement& stmt, const std::vector<StmtParam>& stmt_params);
    size_t QueryCount(const std::string& where_sql, const CustomWhere& conditions);
    std::vector<std::shared_ptr<AbstractEntity>> QueryRecords(const std::string& sql, const std::string& where_sql, const PageQueryParam& params, const CustomWhere& conditions);

  private:
    std::string m_table_name;
    std::string m_sql_count;
    std::string m_sql_delete_by_rid;
    std::string m_sql_delete_all;
    std::string m_sql_exist;
};
}  // namespace addrbook

#endif  // ABSTRACTDAO_H