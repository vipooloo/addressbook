#ifndef ADDRCENTERDEFS_H
#define ADDRCENTERDEFS_H

#include <functional>
#include <string>

using ImportExportCallback = std::function<void(
    const std::string& file_name,  ///< 导出文件名字
    bool result                    ///< 导出是否成功
    )>;

#endif  // ADDRCENTERDEFS_H