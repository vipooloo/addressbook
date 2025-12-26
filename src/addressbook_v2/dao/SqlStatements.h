#ifndef SQLSTATEMENTS_H
#define SQLSTATEMENTS_H

#include <string>
#include <vector>

struct SqlStatements {
    // 通用的 CRUD 语句
    const std::string count;
    const std::string delete_by_rid;
    
    // 【新增】建表语句 (每个表特有)
    const std::string create_table;
    
    // 【新增】表名 (方便后续做 ALTER 操作)
    const std::string table_name;

    // 构造函数：传入 表名 + 字段定义
    SqlStatements(const std::string& tbl_name, const std::string& schema_def)
        : table_name(tbl_name),
          // 自动拼接通用的 SQL
          count("SELECT COUNT(*) FROM " + tbl_name + ";"),
          delete_by_rid("DELETE FROM " + tbl_name + " WHERE rid = ?;"),
          // 拼接建表语句：注意使用 IF NOT EXISTS 防止报错
          create_table("CREATE TABLE IF NOT EXISTS " + tbl_name + " (" + schema_def + ");")
    {}
};

#endif // SQLSTATEMENTS_H