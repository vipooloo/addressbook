#include "AddressManager.h"
#include "Common.h"
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>
#include <string>

class SQLiteGuard
{
    sqlite3* db;

  public:
    SQLiteGuard(sqlite3* _db)
      : db(_db)
    {}
    ~SQLiteGuard()
    {
        if (db)
            sqlite3_close(db);
    }
};

/**
 * 执行备份/恢复的核心函数
 * @param pDest 目标数据库连接
 * @param pSrc  源数据库连接
 * @return 成功返回 true
 */
bool PerformBackup(sqlite3* pDest, sqlite3* pSrc)
{
    // 1. 初始化备份对象
    // "main" 表示主数据库，如果有 attach 数据库需修改此处
    sqlite3_backup* pBackup = sqlite3_backup_init(pDest, "main", pSrc, "main");

    if (!pBackup)
    {
        std::cerr << "Backup failed to initialize: " << sqlite3_errmsg(pDest) << std::endl;
        return false;
    }

    // 2. 循环拷贝数据页
    // 每次拷贝 5 个页面，step 之间可以插入 sleep 以避免长时间锁死主线程
    int rc = 0;
    do
    {
        rc = sqlite3_backup_step(pBackup, 5);

        // 可以在这里加进度条逻辑
        // int remaining = sqlite3_backup_remaining(pBackup);
        // int pagecount = sqlite3_backup_pagecount(pBackup);

        if (rc == SQLITE_BUSY || rc == SQLITE_LOCKED)
        {
            sqlite3_sleep(250);  // 等待一会再试
        }
    } while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);

    // 3. 释放备份对象
    // 如果 rc 是 SQLITE_DONE，说明成功
    sqlite3_backup_finish(pBackup);

    if (rc == SQLITE_DONE)
    {
        return true;
    }
    else
    {
        std::cerr << "Backup failed with error code: " << rc << std::endl;
        return false;
    }
}

// 导出函数：将当前数据库导出到文件
bool ExportDatabase(const std::string& currentDbPath, const std::string& exportPath)
{
    sqlite3* pSrc = nullptr;
    sqlite3* pDest = nullptr;

    // 打开源数据库
    if (sqlite3_open(currentDbPath.c_str(), &pSrc) != SQLITE_OK)
    {
        std::cerr << "Cannot open source DB" << std::endl;
        return false;
    }
    SQLiteGuard g1(pSrc);

    // 打开/创建导出文件（作为目标数据库）
    if (sqlite3_open(exportPath.c_str(), &pDest) != SQLITE_OK)
    {
        std::cerr << "Cannot open export file" << std::endl;
        return false;
    }
    SQLiteGuard g2(pDest);

    std::cout << "Starting Export..." << std::endl;
    return PerformBackup(pDest, pSrc);
}

// 导入函数：从文件恢复到当前数据库（覆盖模式）
bool ImportDatabase(const std::string& currentDbPath, const std::string& importFilePath)
{
    sqlite3* pSrc = nullptr;   // 备份文件
    sqlite3* pDest = nullptr;  // 本地工作的数据库

    // 打开备份文件（只读模式即可，这里用常规打开）
    if (sqlite3_open(importFilePath.c_str(), &pSrc) != SQLITE_OK)
    {
        std::cerr << "Cannot open import file" << std::endl;
        return false;
    }
    SQLiteGuard g1(pSrc);

    // 打开本地目标数据库
    if (sqlite3_open(currentDbPath.c_str(), &pDest) != SQLITE_OK)
    {
        std::cerr << "Cannot open destination DB" << std::endl;
        return false;
    }
    SQLiteGuard g2(pDest);

    std::cout << "Starting Import (Overwrite)..." << std::endl;
    // 注意参数顺序：本地 DB 是 Dest，文件是 Src
    return PerformBackup(pDest, pSrc);
}

namespace {
class ExampleClassUT : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
};

TEST_F(ExampleClassUT, case1)
{
    EXPECT_TRUE(true);
    std::string workingDb = "app_data.db";
    std::string backupFile = "data_backup.db";

    // 1. 导出
    if (ImportDatabase(workingDb, backupFile))
    {
        std::cout << "Import Success!" << std::endl;
    }
}
}  // namespace