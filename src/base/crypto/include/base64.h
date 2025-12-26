

#ifndef BASE64_H
#define BASE64_H

#include <stdint.h>
#include <string>
#include <vector>

constexpr size_t EncodeLength(size_t plain_text_length)
{
    return (plain_text_length + 2) / 3 * 4;
}

size_t DecodeLength(const char* base64_ptr, size_t base64_len);
size_t DecodeLength(const char* base64_str);
size_t DecodeLength(const std::string& base64_str);

/**
 * \brief   编码，写到指定缓存
 *
 * \param   raw_data_ptr  原文地址
 * \param   raw_data_len  原文长度
 * \param   base64_ptr    Base64输出地址
 * \param   base64_size   Base64输出空间大小
 *
 * \return  >0      实际Base64输出大小
 * \return  =0      base64_size不够
 *
 * \note    不会主动加字串结束符
 */
size_t Encode(const void* raw_data_ptr, size_t raw_data_len, char* base64_ptr, size_t base64_size);

/**
 * \brief   编码，返回std::string的base64字串
 *
 * \param   raw_data_ptr  原文地址
 * \param   raw_data_len  原文长度
 *
 * \return  std::string base64字串
 */
std::string Encode(const void* raw_data_ptr, size_t raw_data_len);

/**
 * \brief   编码，返回std::string的base64字串
 *
 * \param   in_data 需要编码的数据
 *
 * \return  std::string base64字串
 */
std::string Encode(const std::vector<uint8_t>& raw_data);

/**
 * \brief   解码，写到指定缓存
 *
 * \param   base64_ptr    Base64地址
 * \param   base64_len    Base64长度
 * \param   raw_data_ptr  原文输出地址
 * \param   raw_data_size 原文输出空间大小
 *
 * \return  >0      实际明文输出大小
 * \return  =0      raw_data_size不够
 */
size_t Decode(const char* base64_ptr, size_t base64_len, void* raw_data_ptr, size_t raw_data_size);

/**
 * \brief   解码，写到指定缓存
 *
 * \param   base64_str    Base64字串
 * \param   raw_data_ptr  原文输出地址
 * \param   raw_data_size 原文输出空间大小
 *
 * \return  >0      实际明文输出大小
 * \return  =0      raw_data_size不够
 */
size_t Decode(const char* base64_str, void* raw_data_ptr, size_t raw_data_size);

/**
 * \brief   解码，输出写到vector
 *
 * \param   base64_str    Base64字串
 * \param   raw_data      原文输出vector
 *
 * \return  >0      实际明文输出大小
 * \return  =0      raw_data_size不够
 */
size_t Decode(const std::string& base64_str, std::vector<uint8_t>& raw_data);

#endif
