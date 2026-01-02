#include "AbstractDao.h"
#include "AddrCenterLog.h"
#include "TransactionGuard.h"

static constexpr const char* SQL_BEGIN_TRANSACTION = "BEGIN TRANSACTION";
static constexpr const char* SQL_COMMIT_TRANSACTION = "COMMIT TRANSACTION";
static constexpr const char* SQL_ROLLBACK_TRANSACTION = "ROLLBACK TRANSACTION";

TransactionGuard::TransactionGuard()
  : m_bCommited{false}
{
    Execute(SQL_BEGIN_TRANSACTION);
}

TransactionGuard::~TransactionGuard()
{
    if (!m_bCommited)
    {
        AB_LOG_E("TransactionGuard: Transaction rollback");
        Execute(SQL_ROLLBACK_TRANSACTION);
    }
}

bool TransactionGuard::Commit()
{
    if (!m_bCommited)
    {
        m_bCommited = Execute("COMMIT TRANSACTION");
    }
    return m_bCommited;
}

bool TransactionGuard::Execute(const char* sql)
{
    bool ret = true;
    int32_t code = AbstractDao::GetDb().tryExec(sql);
    if (SQLite::OK != code)
    {
        AB_LOG_E("TransactionGuard: Transaction commit failed %s %d", sql, code);
        ret = false;
    }
    return ret;
}