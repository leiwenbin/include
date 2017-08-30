// mapi.h: interface for the mapi class.
//
//////////////////////////////////////////////////////////////////////

#ifndef mdf_MAPI_H
#define mdf_MAPI_H

#include "stdio.h"
#include <string>
#include <string.h>
#include <time.h>
#include <vector>
#include "FixLengthInt.h"

namespace mdf {
#define MDF_MALL0C malloc
#define MDF_FREE free
#define MDF_NEW new
#define MDF_DELETE delete

#ifndef MDF_SAFE_FREE
#   define MDF_SAFE_FREE(ptr) if ((ptr) != NULL) {MDF_FREE(ptr); (ptr) = NULL;}
#endif

#ifndef MDF_SAFE_DELETE
#define MDF_SAFE_DELETE(ptr) if ((ptr) != NULL) {MDF_DELETE (ptr); (ptr) = NULL;}
#endif

#ifndef MDF_SAFE_DELETE_ARRAY
#   define MDF_SAFE_DELETE_ARRAY(ptr) if ((ptr) != NULL) {MDF_DELETE [] (ptr); (ptr) = NULL;}
#endif

//断言
    void mdf_assert(bool isTrue);

//睡眠
    void m_sleep(long lMillSecond);

//地址保存到int64
    bool addrToI64(uint64& addr64, const char* ip, int port);

//int64解析出地址
    void i64ToAddr(char* ip, int& port, uint64 addr64);

//压缩del包含的字符
    void TrimString(std::string& str, std::string del);

//压缩右del包含的字符
    void TrimStringLeft(std::string& str, std::string del);

//压缩左del包含的字符
    void TrimStringRight(std::string& str, std::string del);

//压缩空白字符
    char* Trim(char* str);

//压缩右空白字符
    char* TrimRight(char* str);

//压缩左空白字符
    char* TrimLeft(char* str);

//字节高低位顺序翻转
    int reversal(int i);

//取得文件大小
    unsigned long GetFileSize(const char* filename);

//获取CPU数量
//maxCpu可能的最大cpu数，如果检测出cpu数量大于此值，则表示检测有误，将返回defaultCpuNumber
//defaultCpuNumber默认cpu数量，当检测出错时，将认为cpu数量为此值
    unsigned int GetCUPNumber(unsigned int maxCpu, int defaultCpuNumber);

    uint64 CurThreadId(); //当前线程id

    time_t mdf_Date(); //返回0时0分0秒的当前日期

    time_t mdf_Time(); //返回当前日时间（精确到秒）

    bool GetExeDir(char* exeDir, int size); //取得可执行程序位置,linux不带斜杠win下带斜杠

    mdf::uint64 MillTime(); //毫秒级当前时间

    unsigned int APHash(const char* str); //HASH

    typedef struct CPU_PACKED //定义一个cpu occupy的结构体
    {
        char name[20]; //定义一个char类型的数组名name有20个元素
        unsigned int user; //定义一个无符号的int类型的user
        unsigned int nice; //定义一个无符号的int类型的nice
        unsigned int system; //定义一个无符号的int类型的system
        unsigned int idle; //定义一个无符号的int类型的idle
    } CPU_OCCUPY;

    typedef struct MEM_PACKED //定义一个mem occupy的结构体
    {
        char name[20]; //定义一个char类型的数组名name有20个元素
        unsigned long total;
        char name2[20];
        unsigned long free;
        char name3[20];
        unsigned long buffer;
        char name4[20];
        unsigned long cache;
    } MEM_OCCUPY;

    double get_mem_use(); //获取内存使用率

    double get_cpu_use(); //获取CPU使用率(每秒)

    std::vector<std::string> split(std::string str, std::string pattern); //分割字符串

}

#endif // !defined mdf_MAPI_H
