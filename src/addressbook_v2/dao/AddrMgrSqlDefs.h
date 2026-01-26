#ifndef ADDRMGRSQLDEFS_H
#define ADDRMGRSQLDEFS_H

//###########################################################################################################################################
static constexpr const char* SQL_COMMON_QUERY_COUNT = "SELECT COUNT(*) FROM %s WHERE %s;";
static constexpr const char* SQL_COMMON_ENABLE_FOREIGN_KEYS = "PRAGMA foreign_keys = ON;";
static constexpr const char* SQL_DCOMMON_INTEGRITY_CHECK = "PRAGMA integrity_check;";
static constexpr const char* SQL_DCOMMON_VACUUM = "PRAGMA VACUUM;";
//###########################################################################################################################################
/// @brief 电子邮件数据库表名
static constexpr const char* SQL_EMAIL_TABLE_NAME = "email";

/// @brief 创建电子邮件表的 SQL 语句
/// @details 定义了表结构：
/// - rid: 主键，自增整数
/// - email_address: 邮箱地址
/// - email_name: 邮箱显示名称
static constexpr const char* SQL_EMAIL_CREATE_TABLE = R"(
    CREATE TABLE IF NOT EXISTS email (
        rid INTEGER PRIMARY KEY AUTOINCREMENT, 
        email_address TEXT, 
        email_name TEXT
    );
)";

/// @brief 插入新邮件记录的 SQL 语句
/// @note 绑定参数顺序:
/// 1. email_address (TEXT)
/// 2. email_name (TEXT)
static constexpr const char* SQL_EMAIL_INSERT = "INSERT INTO email (email_address, email_name) VALUES (?, ?);";

/// @brief 更新邮件记录的 SQL 语句
/// @note 绑定参数顺序:
/// 1. email_address (TEXT)
/// 2. email_name (TEXT)
/// 3. rid (INTEGER) - 用于指定要更新的记录 ID
static constexpr const char* SQL_EMAIL_UPDATE = "UPDATE email SET email_address = ?, email_name = ? WHERE rid = ?;";

/// @brief 分页查询邮件列表及其关联群组信息的 SQL 语句
/// @details
/// 此查询执行以下操作：
/// 1. 从 email 表 (m) 查询基础信息。
/// 2. 左连接 GROUPMAPPING (r) 和 mail_group (g) 表以获取关联数据。
/// 3. 使用 GROUP_CONCAT 聚合关联的群组 ID 和名称。
///
/// 格式化字符串参数 (%s):
/// - Arg 1: 表名 (通常为 "email")
/// - Arg 2: WHERE 过滤条件子句 (例如 "1=1" 或 "email_name LIKE ?")
/// - Arg 3: 排序方式 (例如 "ASC" 或 "DESC")
///
/// 绑定参数 (?):
/// - Param 1: LIMIT (每页数量)
/// - Param 2: OFFSET (分页偏移量)
static constexpr const char* SQL_EMAIL_SELECT_WITH_GROUPS_BY_PAGE = R"(
    SELECT  m.rid
        ,m.email_address
        ,m.email_name
        ,GROUP_CONCAT(g.rid,',')           AS group_rids
        ,GROUP_CONCAT(g.group_name,'|##|') AS group_names
    FROM %s m
    LEFT JOIN GROUPMAPPING r
    ON m.rid = r.m_rid
    LEFT JOIN mail_group g
    ON r.g_rid = g.rid
    WHERE %s
    GROUP BY m.rid
    ORDER BY m.rid %s
    LIMIT ? OFFSET ?;
)";

/// @brief 基于关键字模糊搜索的 WHERE 子句片段
/// @details 用于在 email_address 或 email_name 中进行模糊匹配。
/// 需要绑定两个相同的参数（搜索关键字）。
static constexpr const char* SQL_EMAIL_WHERE_SEARCH_KEYWORD = "email_address LIKE ? OR email_name LIKE ?";

/// @brief 默认无过滤条件的 WHERE 子句片段
/// @details "1 = 1" 恒为真，用于在没有搜索条件时占位，保证 SQL 语法的完整性。
static constexpr const char* SQL_EMAIL_WHERE_NO_FILTER = "1 = 1";

static constexpr const char* SQL_SELECT_EMAIL_BY_ADDRESS_AND_NAME = "SELECT rid FROM email WHERE email_address = ? AND email_name= ?;";
//###########################################################################################################################################
/// @brief 邮件组数据库表名
static constexpr const char* SQL_GROUP_TABLE_NAME = "mail_group";

/// @brief 创建邮件组表的 SQL 语句
/// @details 定义了表结构：
/// - rid: 主键，自增整数
/// - group_name: 邮件组显示名称
static constexpr const char* SQL_GROUP_CREATE_TABLE = R"(
CREATE TABLE IF NOT EXISTS mail_group (
    rid INTEGER PRIMARY KEY AUTOINCREMENT, 
    group_name TEXT
    );
)";

/// @brief 插入新邮件组记录的 SQL 语句
/// @note 绑定参数顺序:
/// 1. group_name (TEXT)
static constexpr const char* SQL_GROUP_INSERT = "INSERT INTO mail_group (group_name) VALUES (?);";

static constexpr const char* SQL_SELECT_GROUP_BY_NAME = "SELECT rid FROM mail_group WHERE group_name = ?;";
static constexpr const char* SQL_GROUP_UPDATE = "UPDATE mail_group SET group_name = ? WHERE rid = ?;";
static constexpr const char* SQL_GROUP_SELECT_WITH_EMAIL_BY_PAGE = R"(
    SELECT g.rid
        ,g.group_name
        ,GROUP_CONCAT(m.rid, ',')           AS mail_rids
        ,GROUP_CONCAT(m.email_name, '|##|') AS email_names
        ,GROUP_CONCAT(m.email_address, '|##|') AS email_addresses
    FROM %s g
    LEFT JOIN GROUPMAPPING r
        ON g.rid = r.g_rid
    LEFT JOIN email m
        ON r.m_rid = m.rid
    WHERE %s
    GROUP BY g.rid
    ORDER BY g.rid %s
    LIMIT ? OFFSET ?;
)";
static constexpr const char* SQL_GROUP_WHERE_SEARCH_KEYWORD = "group_name LIKE ?";
static constexpr const char* SQL_GROUP_SELECT_WITH_GROUPS_BY_PAGE = R"(
    SELECT g.rid
        ,g.group_name
        ,GROUP_CONCAT(m.rid, ',')              AS email_rids
        ,GROUP_CONCAT(m.email_name,'|##|')     AS email_names
        ,GROUP_CONCAT(m.email_address,'|##|')  AS email_addresses
    FROM %s g
    LEFT JOIN GROUPMAPPING r
        ON g.rid = r.g_rid
    LEFT JOIN email m
        ON r.m_rid = m.rid
    WHERE %s
    GROUP BY g.rid
    ORDER BY g.rid %s
    LIMIT ? OFFSET ?;
)";

//###########################################################################################################################################
/// @brief 邮件与群组关联映射表的表名
static constexpr const char* SQL_EMAILGROUP_TABLE_NAME = "GROUPMAPPING";

/// @brief 创建关联映射表的 SQL 语句
/// @details 定义了邮件 (m_rid) 和群组 (g_rid) 的多对多关系。
/// 表结构包含：
/// - id: 自增主键
/// - m_rid: 邮件表主键 (外键)
/// - g_rid: 群组表主键 (外键)
///
/// 约束特性：
/// - 外键约束: 启用级联删除 (ON DELETE CASCADE)，当邮件或群组被删时，关联关系自动移除。
/// - 唯一约束: (m_rid, g_rid) 联合唯一，防止重复添加相同的关联。
static constexpr const char* SQL_EMAILGROUP_CREATE_TABLE = R"(
    CREATE TABLE IF NOT EXISTS GROUPMAPPING (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        m_rid INTEGER,  
        g_rid INTEGER,  
        FOREIGN KEY (g_rid) REFERENCES mail_group (rid) ON DELETE CASCADE ON UPDATE CASCADE,
        FOREIGN KEY (m_rid) REFERENCES email      (rid) ON DELETE CASCADE ON UPDATE CASCADE,
        UNIQUE (m_rid, g_rid)
        );
)";

/// @brief 插入一条新的关联记录
/// @note 绑定参数顺序:
/// 1. m_rid (邮件 RID)
/// 2. g_rid (群组 RID)
static constexpr const char* SQL_EMAILGROUP_INSERT = "INSERT INTO GROUPMAPPING (m_rid, g_rid) VALUES (?, ?);";

/// @brief 检查邮件所属的群组数量是否超过限制
/// @details
/// 用于在涉及特定群组操作时，校验是否存在某个邮件加入的群组总数超标。
///
/// 格式化字符串参数 (%s):
/// - 涉及的群组 ID 列表字符串 (例如 "1,2,3")，用于缩小检查范围。
///
/// 绑定参数 (?):
/// - Param 1: 允许的最大群组数量 (limit)
///
/// @return 如果查询返回 1，表示存在超限情况。
static constexpr const char* SQL_EMAILGROUP_CHECK_OVER_GROUP_LIMIT = R"(
    SELECT  1
    FROM GROUPMAPPING
    WHERE g_rid IN (%s)
    GROUP BY  m_rid
    HAVING COUNT(g_rid) > ?
    LIMIT 1;
)";

/// @brief 检查群组包含的成员(邮件)数量是否超过限制
/// @details
/// 用于在涉及特定邮件操作时，校验是否存在某个群组拥有的成员总数超标。
///
/// 格式化字符串参数 (%s):
/// - 涉及的邮件 ID 列表字符串 (例如 "10,20")。
///
/// 绑定参数 (?):
/// - Param 1: 允许的最大成员数量 (limit)
///
/// @return 如果查询返回 1，表示存在超限情况。
static constexpr const char* SQL_EMAILGROUP_CHECK_OVER_EMAIL_LIMIT = R"(
    SELECT 1
    FROM GROUPMAPPING
    WHERE m_rid IN (%s)
    GROUP BY g_rid
    HAVING COUNT(m_rid) > ?
    LIMIT 1;
)";

/// @brief 删除指定邮件的所有关联关系
/// @details 通常用于在彻底删除邮件前清理关联，或重置某邮件的群组关系。
/// @note 绑定参数:
/// 1. m_rid (邮件 RID)
static constexpr const char* SQL_EMAILGROUP_REMOVE_BY_EMAIL_RID = R"(DELETE FROM GROUPMAPPING WHERE m_rid = ?;)";

//###########################################################################################################################################
#endif  // ADDRMGRSQLDEFS_H