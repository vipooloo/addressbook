#include "RequestParam.h"
#include <cstring>
#include <sstream>

// ------------------- 构造函数 -------------------
RequestParam::RequestParam(Type type, bool b, int32_t i, uint32_t u, const std::string& s, const std::vector<RequestParam>& arr, const std::map<std::string, RequestParam>& obj)
  : m_type{type}
  , m_bool{b}
  , m_int32{i}
  , m_uint32{u}
  , m_str{s}
  , m_array{arr}
  , m_object{obj}
{}

RequestParam::RequestParam()
  : RequestParam(T_NULL, false, 0, 0U, {}, {}, {})
{}

RequestParam::RequestParam(bool b)
  : RequestParam(T_BOOL, b, 0, 0U, {}, {}, {})
{}

RequestParam::RequestParam(int32_t i)
  : RequestParam(T_INT32, false, i, 0U, {}, {}, {})
{}

RequestParam::RequestParam(uint32_t u)
  : RequestParam(T_UINT32, false, 0, u, {}, {}, {})
{}

RequestParam::RequestParam(const char* s)
  : RequestParam(T_STRING, false, 0, 0U, std::string(s), {}, {})
{}

RequestParam::RequestParam(const std::string& s)
  : RequestParam(T_STRING, false, 0, 0U, s, {}, {})
{
}

RequestParam::RequestParam(const std::vector<RequestParam>& arr)
  : RequestParam(T_ARRAY, false, 0, 0U, {}, arr, {})
{
}

RequestParam::RequestParam(const std::map<std::string, RequestParam>& obj)
  : RequestParam(T_OBJECT, false, 0, 0U, {}, {}, obj)
{
}

RequestParam::RequestParam(const std::initializer_list<RequestParam>& l)
  : RequestParam(T_ARRAY, false, 0, 0U, {}, std::vector<RequestParam>(l), {})
{}

RequestParam::RequestParam(const std::initializer_list<std::pair<std::string, RequestParam>>& l)
  : RequestParam(T_OBJECT, false, 0, 0U, {}, {}, {})
{
    for (const std::pair<std::string, RequestParam>& kv : l)
    {
        m_object.emplace(kv.first, kv.second);
    }
}

// ------------------- 拷贝与移动 -------------------
RequestParam::RequestParam(const RequestParam& other)
{
    CopyFrom(other);
}

RequestParam::RequestParam(RequestParam&& other) noexcept
{
    MoveFrom(std::move(other));
}

RequestParam& RequestParam::operator=(const RequestParam& other)
{
    if (this != &other)
    {
        Clear();
        CopyFrom(other);
    }
    return *this;
}

RequestParam& RequestParam::operator=(RequestParam&& other) noexcept
{
    if (this != &other)
    {
        Clear();
        MoveFrom(std::move(other));
    }
    return *this;
}

void RequestParam::Clear()
{
    m_type = T_NULL;
    m_bool = false;
    m_int32 = 0;
    m_uint32 = 0;
    m_str.clear();
    m_array.clear();
    m_object.clear();
}

// ------------------- 设置与修改 -------------------
void RequestParam::Append(const RequestParam& v)
{
    if (m_type == T_ARRAY)
    {
        m_array.emplace_back(v);
    }
}

void RequestParam::Append(RequestParam&& v)
{
    if (m_type == T_ARRAY)
    {
        m_array.emplace_back(std::move(v));
    }
}

void RequestParam::Insert(const std::string& key, const RequestParam& v)
{
    if (m_type == T_OBJECT)
    {
        m_object[key] = v;
    }
}

void RequestParam::Insert(const std::string& key, RequestParam&& v)
{
    if (m_type == T_OBJECT)
    {
        m_object[key] = std::move(v);
    }
}

void RequestParam::Remove(const std::string& key)
{
    if (m_type == T_OBJECT)
    {
        m_object.erase(key);
    }
}

void RequestParam::Remove(size_t index)
{
    if ((m_type == T_ARRAY) && (index < m_array.size()))
    {
        m_array.erase(m_array.begin() + static_cast<ptrdiff_t>(index));
    }
}

// 静态空对象实现
const RequestParam& RequestParam::NullParam()
{
    static RequestParam nullParam;
    return nullParam;
}

// ------------------- 访问 -------------------
const RequestParam& RequestParam::At(size_t index) const
{
    const RequestParam* req_param = &(NullParam());
    if ((m_type == T_ARRAY) && (index < m_array.size()))
    {
        req_param = &(m_array[index]);
    }
    return *req_param;
}

const RequestParam& RequestParam::At(const std::string& key) const
{
    const RequestParam* req_param = &(NullParam());
    if (m_type == T_OBJECT)
    {
        std::map<std::string, RequestParam>::const_iterator it = m_object.find(key);
        if (it != m_object.cend())
        {
            req_param = &(it->second);
        }
    }
    return *req_param;
}

const RequestParam& RequestParam::operator[](const std::string& key) const
{
    return At(key);
}

// ------------------- 打印 -------------------
std::string RequestParam::ToString(size_t indent) const
{
    std::ostringstream oss;
    std::string pad(indent, ' ');

    switch (m_type)
    {
        case T_NULL:
            oss << "null";
            break;
        case T_BOOL:
            oss << (m_bool ? "true" : "false");
            break;
        case T_INT32:
            oss << m_int32;
            break;
        case T_UINT32:
            oss << m_uint32;
            break;
        case T_STRING:
            oss << "\"" << EscapeString(m_str) << "\"";
            break;
        case T_ARRAY:
        {
            oss << "[\n";
            for (size_t i = 0; i < m_array.size(); ++i)
            {
                oss << pad << "  " << m_array[i].ToString(indent + 2);
                if (i != m_array.size() - 1)
                {
                    oss << ",";
                }
                oss << "\n";
            }
            oss << pad << "]";
            break;
        }
        case T_OBJECT:
        {
            oss << "{\n";
            size_t count = 0;
            for (const std::pair<const std::string, RequestParam>& kv : m_object)
            {
                oss << pad << "  \"" << kv.first
                    << "\": " << kv.second.ToString(indent + 2);
                if (count != m_object.size() - 1)
                {
                    oss << ",";
                }
                oss << "\n";
                ++count;
            }
            oss << pad << "}";
            break;
        }
    }

    return oss.str();
}

// ------------------- 序列化 -------------------
std::string RequestParam::Serialize() const
{
    std::string out;
    out.reserve(64 + m_array.size() * 32);
    out.push_back(static_cast<char>(m_type));

    switch (m_type)
    {
        case T_BOOL:
            out.push_back(m_bool ? 1 : 0);
            break;
        case T_INT32:
            WriteInt32(out, m_int32);
            break;
        case T_UINT32:
            WriteUInt32(out, m_uint32);
            break;
        case T_STRING:
        {
            uint32_t len = static_cast<uint32_t>(m_str.size());
            WriteUInt32(out, len);
            if (!m_str.empty())
            {
                out.append(m_str);
            }
            break;
        }
        case T_ARRAY:
        {
            uint32_t size = static_cast<uint32_t>(m_array.size());
            WriteUInt32(out, size);
            for (const RequestParam& elem : m_array)
            {
                out.append(elem.Serialize());
            }
            break;
        }
        case T_OBJECT:
        {
            uint32_t size = static_cast<uint32_t>(m_object.size());
            WriteUInt32(out, size);
            for (const std::pair<const std::string, RequestParam>& kv : m_object)
            {
                // 序列化键
                uint32_t key_len = static_cast<uint32_t>(kv.first.size());
                WriteUInt32(out, key_len);
                out.append(kv.first);
                // 序列化值
                out.append(kv.second.Serialize());
            }
            break;
        }
        default:
            // T_NULL 不需要额外数据
            break;
    }

    return out;
}

// ------------------- 反序列化 -------------------
bool RequestParam::ParseBool(const std::string& buf, size_t& pos)
{
    bool ret = false;
    if (pos < buf.size())
    {
        m_bool = (buf[pos++] != 0);
        ret = true;
    }
    return ret;
}

bool RequestParam::ParseInt32(const std::string& buf, size_t& pos)
{
    return ReadInt32(buf, pos, m_int32);
}

bool RequestParam::ParseUInt32(const std::string& buf, size_t& pos)
{
    return ReadUInt32(buf, pos, m_uint32);
}

bool RequestParam::ParseString(const std::string& buf, size_t& pos)
{
    uint32_t len = 0;
    bool ret = ReadUInt32(buf, pos, len);
    if (ret && ((pos + len) <= buf.size()))
    {
        m_str = buf.substr(pos, len);
        pos += len;
    }
    else
    {
        ret = false;
    }
    return ret;
}

bool RequestParam::ParseArray(const std::string& buf, size_t& pos)
{
    bool ret = true;
    uint32_t size = 0;
    if (ReadUInt32(buf, pos, size))
    {
        std::vector<RequestParam> array;
        array.reserve(size);
        for (uint32_t i = 0; i < size && ret; ++i)
        {
            RequestParam elem;
            if (elem.Parse(buf, pos))
            {
                array.push_back(std::move(elem));
            }
            else
            {
                ret = false;
                break;
            }
        }
        if (ret)
        {
            m_array = array;
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}

bool RequestParam::ParseObject(const std::string& buf, size_t& pos)
{
    bool ret = true;
    uint32_t size = 0;
    if (ReadUInt32(buf, pos, size))
    {
        std::map<std::string, RequestParam> object;
        for (uint32_t i = 0; i < size && ret; ++i)
        {
            uint32_t key_len = 0;
            if (ReadUInt32(buf, pos, key_len) && (pos + key_len <= buf.size()))
            {
                std::string key(buf.substr(pos, key_len));
                pos += key_len;
                RequestParam value;
                if (value.Parse(buf, pos))
                {
                    object[key] = std::move(value);
                }
                else
                {
                    ret = false;
                    break;
                }
            }
            else
            {
                ret = false;
                break;
            }
        }
        if (ret)
        {
            m_object = object;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool RequestParam::Parse(const std::string& buf)
{
    size_t pos = 0;
    return Parse(buf, pos);
}

bool RequestParam::Parse(const std::string& buf, size_t& pos)
{
    bool ret = false;
    if (pos < buf.size())
    {
        Clear();
        m_type = static_cast<Type>(buf[pos++]);
        switch (m_type)
        {
            case T_NULL:
                ret = true;
                break;
            case T_BOOL:
                ret = ParseBool(buf, pos);
                break;
            case T_INT32:
                ret = ParseInt32(buf, pos);
                break;
            case T_UINT32:
                ret = ParseUInt32(buf, pos);
                break;
            case T_STRING:
                ret = ParseString(buf, pos);
                break;
            case T_ARRAY:
                ret = ParseArray(buf, pos);
                break;
            case T_OBJECT:
                ret = ParseObject(buf, pos);
                break;
            default:
                break;
        }
    }
    return ret;
}

// ------------------- 工具函数 -------------------
uint32_t RequestParam::ToBE32(uint32_t v)
{
    return ((v & 0x000000FFU) << 24U) | ((v & 0x0000FF00U) << 8U) | ((v & 0x00FF0000U) >> 8U) | ((v & 0xFF000000U) >> 24U);
}

int32_t RequestParam::ToBE32(int32_t v)
{
    return static_cast<int32_t>(ToBE32(static_cast<uint32_t>(v)));
}

uint32_t RequestParam::FromBE32(uint32_t v)
{
    // 对于 32 位值，ToBE32 和 FromBE32 是相同的操作
    return ToBE32(v);
}

int32_t RequestParam::FromBE32(int32_t v)
{
    return static_cast<int32_t>(FromBE32(static_cast<uint32_t>(v)));
}

void RequestParam::WriteInt32(std::string& out, int32_t v)
{
    uint32_t be_value = ToBE32(static_cast<uint32_t>(v));
    out.append(reinterpret_cast<const char*>(&be_value), 4);
}

void RequestParam::WriteUInt32(std::string& out, uint32_t v)
{
    uint32_t be_value = ToBE32(v);
    out.append(reinterpret_cast<const char*>(&be_value), 4);
}

bool RequestParam::ReadInt32(const std::string& buf, size_t& pos, int32_t& out)
{
    bool ret = true;
    size_t offset = sizeof(out);
    if (pos + offset > buf.size())
    {
        ret = false;
    }
    else
    {
        uint32_t be_value = 0;
        std::memcpy(&be_value, buf.data() + pos, offset);
        pos += offset;
        out = static_cast<int32_t>(FromBE32(be_value));
    }
    return ret;
}

bool RequestParam::ReadUInt32(const std::string& buf, size_t& pos, uint32_t& out)
{
    bool ret = true;
    size_t offset = sizeof(out);
    if (pos + offset > buf.size())
    {
        ret = false;
    }
    else
    {
        uint32_t be_value = 0;
        std::memcpy(&be_value, buf.data() + pos, offset);
        pos += offset;
        out = FromBE32(be_value);
    }
    return ret;
}

// ------------------- 辅助函数 -------------------
void RequestParam::CopyFrom(const RequestParam& other)
{
    m_type = other.m_type;
    m_bool = other.m_bool;
    m_int32 = other.m_int32;
    m_uint32 = other.m_uint32;
    m_str = other.m_str;
    m_array = other.m_array;
    m_object = other.m_object;
}

void RequestParam::MoveFrom(RequestParam&& other)
{
    m_type = other.m_type;
    m_bool = other.m_bool;
    m_int32 = other.m_int32;
    m_uint32 = other.m_uint32;
    m_str = std::move(other.m_str);
    m_array = std::move(other.m_array);
    m_object = std::move(other.m_object);
    other.Clear();
}

std::string RequestParam::EscapeString(const std::string& s)
{
    std::string result;
    for (char c : s)
    {
        switch (c)
        {
            case '\"':
                result += "\\\"";
                break;
            case '\\':
                result += "\\\\";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            default:
                if (static_cast<unsigned char>(c) < 0x20)
                {
                    char buf[7];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    result += buf;
                }
                else
                {
                    result += c;
                }
                break;
        }
    }
    return result;
}
