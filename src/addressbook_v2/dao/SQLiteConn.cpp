#include "AddrMgrLog.h"
#include "AddrMgrSqlDefs.h"
#include "SQLiteConn.h"
#include <sqlite3.h>

namespace addrbook {
SQLiteConn::SQLiteConn(const std::string& path)
  : m_path{path}
  , m_db{m_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE}
{
    CheckIntegrity();
    EnableForeignKeys();
}

void SQLiteConn::Open()
{
    m_db = SQLite::Database(m_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}

void SQLiteConn::CheckIntegrity()
{
    int32_t code = GetDb().tryExec(SQL_DCOMMON_INTEGRITY_CHECK);
    if (code != SQLITE_OK)
    {
        AB_LOG_E("database corrupted, rebuilding %d", code);
        Rebuild();
    }
}

void SQLiteConn::Rebuild()
{
    std::string bad_file = m_path + ".corrupt.bak";
    std::remove(bad_file.c_str());
    std::rename(m_path.c_str(), bad_file.c_str());
    Open();
}

void SQLiteConn::EnableForeignKeys()
{
    int32_t code = GetDb().tryExec(SQL_COMMON_ENABLE_FOREIGN_KEYS);
    if (code != SQLITE_OK)
    {
        AB_LOG_E("failed to enable foreign keys %d", code);
    }
}
}  // namespace addrbook