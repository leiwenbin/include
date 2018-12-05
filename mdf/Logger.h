// Logger.h: interface for the Logger class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDF_LOGGER_H
#define MDF_LOGGER_H

#include <stdio.h>
#include <string>
#include "Lock.h"

namespace mdf {

    typedef enum {
        LEVEL_LOG_FAILED = 1, //faild
        LEVEL_LOG_ERROR, //error
        LEVEL_LOG_WARNING, //warning
        LEVEL_LOG_INFO, //info
        LEVEL_LOG_DEBUG //debug
    } LEVEL_LOG;

    class Logger {
    public:
        Logger();

        virtual ~Logger();

    public:
        //设置日志名，日志将被保存到"log/名字/"下目录下
        //只能设置1次,以后都将失败
        //如果构造函数已经传递了日志名,则此方法不能再设置
        void SetLogName(const char* name); //必须在SetLogDir之前调用才有效
        bool SetLogDir(const char* logDir); //做初始化,只能调用1次

        void SetPrintLog(bool bPrint); //写日志时，是否同时打印到控制台
        bool Info(LEVEL_LOG level, const char* findKey, const char* format, ...); //输出信息，输出格式为 时间+空格+(findKey)+空格+(Tid:线程ID)+空格+参数传入信息
        bool StreamInfo(LEVEL_LOG level, const char* findKey, unsigned char* stream, int nLen, const char* format, ...); //输出带数据流的日至信息, stream数据将接在format信息之后输出，以stream:为标记
        void SetMaxLogSize(int maxLogSize); //设置单个日志文件最大尺寸，单位M
        void SetMaxExistDay(int maxExistDay); //设置日志文件最大保留天数，比如设置为30则，最多保存最近30天的日志
        //删除nDay天前的日志
        void DelLog(int nDay);

    private:
        bool OpenRunLog();
        bool CreateFreeDir(const char* dir); //创建自由（最大权限）目录
        void RenameMaxLog(); //重名超大日志
        void FindDelLog(char* lpPath, int maxExistDay);

    private:
        bool m_isInit; //是否初始化
        bool m_bPrint; //输出日志时,是否同时打印到控制台
        Mutex m_writeMutex;
        bool m_bRLogOpened; //运行日志文件是否打开
        //运行日志当前日期
        int m_nRunLogCurYear;
        int m_nRunLogCurMonth;
        int m_nRunLogCurDay;
        FILE* m_fpRunLog; //日志文件指针
        std::string m_name; //日志名
        std::string m_runLogDir; //日志目录
        int m_maxLogSize; //日志文件最大尺寸
        int m_maxExistDay;
        int m_index; //日志编号
        char* m_exeDir;
        time_t m_lastDelTime;
    };

} //namespace mdf

#endif // MDF_LOGGER_H
