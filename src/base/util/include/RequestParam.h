#ifndef REQUESTPARAM_H
#define REQUESTPARAM_H

#include <map>
#include <stdint.h>
#include <string>
#include <vector>

/**
 * @enum Type
 * @brief 数据类型枚举
 * @details 定义了 RequestParam 支持的所有数据类型
 */
enum Type : uint8_t
{
    T_NULL = 0x00,    ///< 空类型
    T_BOOL = 0x01,    ///< 布尔类型
    T_INT32 = 0x02,   ///< 32位有符号整数
    T_UINT32 = 0x03,  ///< 32位无符号整数
    T_STRING = 0x04,  ///< 字符串类型
    T_ARRAY = 0x05,   ///< 数组类型
    T_OBJECT = 0x06   ///< 对象类型（键值对）
};

/**
 * @class RequestParam
 * @brief 动态类型参数类
 * @details 支持多种数据类型的存储、序列化和反序列化，类似JSON数据结构
 */
class RequestParam
{
  public:
    // ------------------- 构造函数 -------------------

    /**
     * @brief 默认构造函数
     * @details 创建一个空类型的 RequestParam 对象
     */
    RequestParam();

    /**
     * @brief 布尔类型构造函数
     * @param b 布尔值
     */
    explicit RequestParam(bool b);

    /**
     * @brief 32位有符号整数构造函数
     * @param i 32位有符号整数值
     */
    explicit RequestParam(int32_t i);

    /**
     * @brief 32位无符号整数构造函数
     * @param u 32位无符号整数值
     */
    explicit RequestParam(uint32_t u);

    /**
     * @brief 字符串构造函数
     * @param s 字符串值
     */
    explicit RequestParam(const char* s);

    /**
     * @brief 字符串构造函数
     * @param s 字符串值
     */
    explicit RequestParam(const std::string& s);

    /**
     * @brief 数组构造函数
     * @param arr RequestParam数组
     */
    explicit RequestParam(const std::vector<RequestParam>& arr);

    /**
     * @brief 对象构造函数
     * @param obj 键值对映射
     */
    explicit RequestParam(const std::map<std::string, RequestParam>& obj);

    /**
     * @brief 初始化列表构造函数（数组）
     * @param l RequestParam初始化列表
     */
    explicit RequestParam(const std::initializer_list<RequestParam>& l);

    /**
     * @brief 初始化列表构造函数（对象）
     * @param l 键值对初始化列表
     */
    explicit RequestParam(const std::initializer_list<std::pair<std::string, RequestParam>>& l);

    // ------------------- 拷贝与移动 -------------------

    /**
     * @brief 拷贝构造函数
     * @param other 另一个 RequestParam 对象
     */
    RequestParam(const RequestParam& other);

    /**
     * @brief 移动构造函数
     * @param other 右值引用的 RequestParam 对象
     */
    RequestParam(RequestParam&& other) noexcept;

    /**
     * @brief 拷贝赋值运算符
     * @param other 另一个 RequestParam 对象
     * @return 自身引用
     */
    RequestParam& operator=(const RequestParam& other);

    /**
     * @brief 移动赋值运算符
     * @param other 右值引用的 RequestParam 对象
     * @return 自身引用
     */
    RequestParam& operator=(RequestParam&& other) noexcept;

    /**
     * @brief 析构函数
     */
    ~RequestParam() = default;

    // ------------------- 类型检查 -------------------

    /**
     * @brief 获取数据类型
     * @return 当前存储的数据类型
     */
    Type GetType() const
    {
        return m_type;
    }

    /**
     * @brief 检查是否为空类型
     * @return 为空类型返回true，否则返回false
     */
    bool IsNull() const
    {
        return m_type == T_NULL;
    }

    /**
     * @brief 检查是否为布尔类型
     * @return 为布尔类型返回true，否则返回false
     */
    bool IsBool() const
    {
        return m_type == T_BOOL;
    }

    /**
     * @brief 检查是否为32位有符号整数类型
     * @return 为int32类型返回true，否则返回false
     */
    bool IsInt32() const
    {
        return m_type == T_INT32;
    }

    /**
     * @brief 检查是否为32位无符号整数类型
     * @return 为uint32类型返回true，否则返回false
     */
    bool IsUInt32() const
    {
        return m_type == T_UINT32;
    }

    /**
     * @brief 检查是否为字符串类型
     * @return 为字符串类型返回true，否则返回false
     */
    bool IsString() const
    {
        return m_type == T_STRING;
    }

    /**
     * @brief 检查是否为数组类型
     * @return 为数组类型返回true，否则返回false
     */
    bool IsArray() const
    {
        return m_type == T_ARRAY;
    }

    /**
     * @brief 检查是否为对象类型
     * @return 为对象类型返回true，否则返回false
     */
    bool IsObject() const
    {
        return m_type == T_OBJECT;
    }

    // ------------------- 设置与修改 -------------------

    /**
     * @brief 向数组追加元素（拷贝）
     * @param v 要追加的元素
     * @note 仅当对象为数组类型时有效
     */
    void Append(const RequestParam& v);

    /**
     * @brief 向数组追加元素（移动）
     * @param v 要追加的元素（右值引用）
     * @note 仅当对象为数组类型时有效
     */
    void Append(RequestParam&& v);

    /**
     * @brief 向对象插入键值对（拷贝）
     * @param key 键名
     * @param v 值
     * @note 仅当对象为对象类型时有效
     */
    void Insert(const std::string& key, const RequestParam& v);

    /**
     * @brief 向对象插入键值对（移动）
     * @param key 键名
     * @param v 值（右值引用）
     * @note 仅当对象为对象类型时有效
     */
    void Insert(const std::string& key, RequestParam&& v);

    /**
     * @brief 从对象中移除指定键
     * @param key 要移除的键名
     * @note 仅当对象为对象类型时有效
     */
    void Remove(const std::string& key);

    /**
     * @brief 从数组中移除指定索引的元素
     * @param index 要移除的元素索引
     * @note 仅当对象为数组类型且索引有效时有效
     */
    void Remove(size_t index);

    // ------------------- 访问 -------------------

    /**
     * @brief 访问数组指定索引的元素
     * @param index 元素索引
     * @return 元素的常量引用，索引无效时返回空对象
     * @note 仅当对象为数组类型时有效
     */
    const RequestParam& At(size_t index) const;

    /**
     * @brief 访问对象指定键的元素
     * @param key 键名
     * @return 元素的常量引用，键不存在时返回空对象
     * @note 仅当对象为对象类型时有效
     */
    const RequestParam& At(const std::string& key) const;

    /**
     * @brief 重载[]运算符访问对象元素
     * @param key 键名
     * @return 元素的常量引用，键不存在时返回空对象
     * @note 仅当对象为对象类型时有效
     */
    const RequestParam& operator[](const std::string& key) const;

    // ------------------- 转换 -------------------

    /**
     * @brief 转换为布尔值
     * @param def 类型不匹配时的默认值
     * @return 当前存储的布尔值或默认值
     */
    bool AsBool(bool def) const
    {
        return (m_type == T_BOOL) ? m_bool : def;
    }

    /**
     * @brief 转换为32位有符号整数
     * @param def 类型不匹配时的默认值
     * @return 当前存储的整数值或默认值
     */
    int32_t AsInt32(int32_t def) const
    {
        return (m_type == T_INT32) ? m_int32 : def;
    }

    /**
     * @brief 转换为32位无符号整数
     * @param def 类型不匹配时的默认值
     * @return 当前存储的整数值或默认值
     */
    uint32_t AsUInt32(uint32_t def) const
    {
        return (m_type == T_UINT32) ? m_uint32 : def;
    }

    /**
     * @brief 转换为字符串
     * @param def 类型不匹配时的默认值
     * @return 当前存储的字符串或默认值
     */
    std::string AsString(const std::string& def) const
    {
        return (m_type == T_STRING) ? m_str : def;
    }

    // ------------------- 打印 -------------------

    /**
     * @brief 转换为格式化字符串（JSON风格）
     * @param indent 缩进空格数
     * @return 格式化的字符串表示
     */
    std::string ToString(size_t indent = 0) const;

    // ------------------- 序列化 -------------------

    /**
     * @brief 序列化为二进制数据
     * @return 二进制序列化字符串
     */
    std::string Serialize() const;

    // ------------------- 反序列化 -------------------

    /**
     * @brief 从二进制数据反序列化
     * @param buf 二进制数据缓冲区
     * @return 成功返回true，失败返回false
     */
    bool Parse(const std::string& buf);

  private:
    /**
     * @brief 私有构造函数，用于统一初始化
     * @param type 数据类型
     * @param b 布尔值
     * @param i 32位有符号整数
     * @param u 32位无符号整数
     * @param s 字符串
     * @param arr 数组
     * @param obj 对象
     */
    RequestParam(Type type, bool b, int32_t i, uint32_t u, const std::string& s, const std::vector<RequestParam>& arr, const std::map<std::string, RequestParam>& obj);

    // ------------------- 工具 -------------------

    /**
     * @brief 将32位无符号整数转换为大端序
     * @param v 主机字节序的数值
     * @return 大端序的数值
     */
    static uint32_t ToBE32(uint32_t v);

    /**
     * @brief 将32位有符号整数转换为大端序
     * @param v 主机字节序的数值
     * @return 大端序的数值
     */
    static int32_t ToBE32(int32_t v);

    /**
     * @brief 将大端序32位无符号整数转换为主机字节序
     * @param v 大端序的数值
     * @return 主机字节序的数值
     */
    static uint32_t FromBE32(uint32_t v);

    /**
     * @brief 将大端序32位有符号整数转换为主机字节序
     * @param v 大端序的数值
     * @return 主机字节序的数值
     */
    static int32_t FromBE32(int32_t v);

    /**
     * @brief 获取静态空对象实例
     * @return 空对象的常量引用
     */
    static const RequestParam& NullParam();

    /**
     * @brief 转义字符串中的特殊字符
     * @param s 原始字符串
     * @return 转义后的字符串
     */
    static std::string EscapeString(const std::string& s);

    /**
     * @brief 将32位有符号整数写入缓冲区（大端序）
     * @param out 输出缓冲区
     * @param v 要写入的数值
     */
    static void WriteInt32(std::string& out, int32_t v);

    /**
     * @brief 将32位无符号整数写入缓冲区（大端序）
     * @param out 输出缓冲区
     * @param v 要写入的数值
     */
    static void WriteUInt32(std::string& out, uint32_t v);

    /**
     * @brief 从缓冲区读取32位有符号整数（大端序）
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @param out 输出数值
     * @return 成功返回true，失败返回false
     */
    static bool ReadInt32(const std::string& buf, size_t& pos, int32_t& out);

    /**
     * @brief 从缓冲区读取32位无符号整数（大端序）
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @param out 输出数值
     * @return 成功返回true，失败返回false
     */
    static bool ReadUInt32(const std::string& buf, size_t& pos, uint32_t& out);

    // ------------------- 深拷贝与移动 -------------------

    /**
     * @brief 从另一个对象深拷贝数据
     * @param other 源对象
     */
    void CopyFrom(const RequestParam& other);

    /**
     * @brief 从另一个对象移动数据
     * @param other 源对象（右值引用）
     */
    void MoveFrom(RequestParam&& other);

    /**
     * @brief 清空所有数据，重置为空类型
     */
    void Clear();

    // ------------------- 内部反序列化 -------------------

    /**
     * @brief 解析布尔类型数据
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @return 成功返回true，失败返回false
     */
    bool ParseBool(const std::string& buf, size_t& pos);

    /**
     * @brief 解析32位有符号整数类型数据
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @return 成功返回true，失败返回false
     */
    bool ParseInt32(const std::string& buf, size_t& pos);

    /**
     * @brief 解析32位无符号整数类型数据
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @return 成功返回true，失败返回false
     */
    bool ParseUInt32(const std::string& buf, size_t& pos);

    /**
     * @brief 解析字符串类型数据
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @return 成功返回true，失败返回false
     */
    bool ParseString(const std::string& buf, size_t& pos);

    /**
     * @brief 解析数组类型数据
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @return 成功返回true，失败返回false
     */
    bool ParseArray(const std::string& buf, size_t& pos);

    /**
     * @brief 解析对象类型数据
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @return 成功返回true，失败返回false
     */
    bool ParseObject(const std::string& buf, size_t& pos);

    /**
     * @brief 内部解析函数
     * @param buf 输入缓冲区
     * @param pos 当前位置（会被更新）
     * @return 成功返回true，失败返回false
     */
    bool Parse(const std::string& buf, size_t& pos);

  private:
    Type m_type;                                   ///< 当前数据类型
    bool m_bool;                                   ///< 布尔值存储
    int32_t m_int32;                               ///< 32位有符号整数存储
    uint32_t m_uint32;                             ///< 32位无符号整数存储
    std::string m_str;                             ///< 字符串存储
    std::vector<RequestParam> m_array;             ///< 数组存储
    std::map<std::string, RequestParam> m_object;  ///< 对象存储
};

#endif  // REQUESTPARAM_H