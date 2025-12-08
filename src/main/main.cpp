#include "Api.h"
#include "Log.h"
#include <cstring>
#include <limits.h>
#include <string>
#include <unistd.h>

void change_workpath()
{
    char proc_path[PATH_MAX] = {0};

    // 获取可执行文件路径
    ssize_t len = readlink("/proc/self/exe", proc_path, sizeof(proc_path) - 1);
    if (len == -1)
    {
        LOGX_ERROR_M("main", "Failed to read process path:{}", strerror(errno));
        return;
    }
    proc_path[len] = '\0';  // 确保字符串正确终止

    LOGX_INFO_M("main", "Process pathname:{}", proc_path);

    // 找到最后一个斜杠的位置
    char* last_slash = strrchr(proc_path, '/');
    if (nullptr == last_slash)
    {
        LOGX_ERROR_M("main", "No directory separator found in path");
        return;
    }

    // 截断路径，只保留目录部分
    *last_slash = '\0';

    LOGX_INFO_M("main", "Changing directory to:{}", proc_path);

    // 切换工作目录
    if (chdir(proc_path) != 0)
    {
        LOGX_ERROR_M("main", "Failed to change directory:{}", strerror(errno));
        return;
    }
}

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    LOG_INIT();

    change_workpath();
    Add();
    return 0;
}