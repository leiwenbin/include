// EpollFrame.h: interface for the EpollFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDF_EPOLLFRAME_H
#define MDF_EPOLLFRAME_H

#include "NetEngine.h"

namespace mdf {
class EpollFrame : public NetEngine {
    public:
        EpollFrame();

        virtual ~EpollFrame();

    protected:
        //网络事件监听线程
        void* NetMonitor(void*) override;

        //接收数据，连接关闭时返回false，因具体响应器差别，需要派生类中实现
        connectState RecvData(NetConnect* pConnect, char* pData, unsigned short uSize) override ;

        //发送数据
        connectState SendData(NetConnect* pConnect, unsigned short uSize) override;

        int ListenPort(int port) override; //监听一个端口,返回创建的套接字

        void NewConnectMonitor();

        void DataMonitor();

        void SendAbleMonitor();
    };

} //namespace mdf

#endif // MDF_EPOLLFRAME_H
