#ifndef SQLITECONN_H
#define SQLITECONN_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>
#include <string>

namespace addrbook {
class SQLiteConn
{
  public:
    explicit SQLiteConn(const std::string& path);
    ~SQLiteConn() = default;
    SQLite::Database& GetDb()
    {
        return *m_db_sptr;
    }

  private:
    SQLiteConn(const SQLiteConn&) = delete;
    SQLiteConn& operator=(const SQLiteConn&) = delete;
    SQLiteConn(SQLiteConn&&) noexcept = delete;
    SQLiteConn& operator=(SQLiteConn&&) noexcept = delete;
    void Open();

    void CheckIntegrity();

    void Rebuild();

    void EnableForeignKeys();

  private:
    std::string m_path;
    std::shared_ptr<SQLite::Database> m_db_sptr;
};
}  // namespace addrbook

#endif  // SQLITECONN_H