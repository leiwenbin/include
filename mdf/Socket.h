// Socket.h: interface for the Socket class.
//
//////////////////////////////////////////////////////////////////////
/***********************************************************************
 类说明：	封装Socket
 使用说明：	如果是windows，在该类的对象所在进程主线程中要调用1次静态成员函数SocketInit(NULL)
 ************************************************************************/

#ifndef MDF_SOCKET_H
#define MDF_SOCKET_H

#ifdef WIN32

#pragma warning(disable:4996)
#define SOCK_STREAM     1               /* stream socket */
#define SOCK_DGRAM      2               /* datagram socket */
#define SOL_SOCKET      0xffff          /* options for socket level */
#ifndef SOMAXCONN
#define SOMAXCONN       5
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  -1
#endif

/*
 * Socket address, internet style.
 */
struct sockaddr_in;

//#include <winsock2.h>
//#include <mswsock.h>
//#define SOCK_STREAM 1
//#define SOCK_DGRAM 2
//#define SOL_SOCKET 0xffff
//#define SOMAXCONN       5

//typedef unsigned int int;
//struct sockaddr_in;
//{
//};
#define socklen_t int
#else

#include <errno.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>//为了兼容gcc4.7.2 gcc4.7.3

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

#include <time.h>
#include <string>

namespace mdf {

    class Socket {
    public:
        enum SocketError {
            seTimeOut = -2, seSocketClose = -1, seError = -3,
        };
        enum protocol {
            tcp = SOCK_STREAM, udp = SOCK_DGRAM,
        };

        Socket();

        Socket(int hSocket, protocol nProtocolType);

        virtual ~Socket();

    public:
        //将域名转换为真实IP，如果lpszHostAddress本来就是ip，不影响转换结果
        //转换失败返回""
        static char* HostName2IP(char* hostname);

        //取得套接字句柄
        int GetSocket();

        /*
         功能：一致性初始化，m_hSocket来至于外部创建，还是内部创建
         参数：
         nProtocolType	protocol		[In]	Socket::tcp或Socket::udp
         返回值：成功返回TRUE,失败返回FALSE
         */
        bool Init(protocol nProtocolType);

        /**
         功能：断开socket连接
         参数:
         参数名	类型	[In/Out]	说明
         返回值:
         注意:
         支持傻瓜式调用，任何时候都可以调用此函数断开链接，变为断开状态
         */
        void Close();

        //是关闭状态返回true,否则返回false
        bool IsClosed();

        //////////////////////////////////////////////////////////////////////////
        //TCP收发
        /*
         功能：调用库函数send发送数据
         参数：
         lpBuf	const void*	[In]	发送的数据
         nBufLen	int		[In]	数据长度
         nFlags	int		[In]	An indicator specifying the way in which the call is made
         返回值：成功实际发送的字节数，失败返回小于0
         */
        int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

        /*
         功能：接收数据
         参数：
         lpBuf		void*		[Out]	保存接收的数据
         nBufLen		__int32			[Out]	收到数据的长度
         lSecond		long		[In]	超时时间秒
         lMinSecond	long		[In]	超时时间毫秒
         返回值：实际接收到的字节数，超时返回-2，断开连接返回-1，其它错误返回-3
         */
        int Receive(void* lpBuf, int nBufLen, bool bCheckDataLength = false, long lSecond = 0, long lMinSecond = 0);

        //////////////////////////////////////////////////////////////////////////
        //UDP收发
        /*
         功能：调用库函数sendto发送UDP数据
         参数：
         strIP	const char*	[In]	接收方IP
         nPort	int		[In]	接收方端口
         lpBuf	const void*	[In]	发送的数据
         nBufLen	int		[In]	数据长度
         nFlags	int		[In]	An indicator specifying the way in which the call is made
         返回值：成功返回实际发送字节数，可能小于请求发送的长度，失败返回常量SOCKET_ERROR,调用WSAGetLastError函数可获取错误信息
         */
        int SendTo(const char* strIP, int nPort, const void* lpBuf, int nBufLen, int nFlags = 0);

        /*
         功能：调用库函数recvFrom接收UDP数据
         参数：
         lpBuf		void*		[Out]	保存接收的数据
         nBufLen		int			[Out]	收到数据的长度
         strFromIP	string&		[Out]	发送方IP
         nFromPort	int&		[Out]	发送方端口
         lSecond		long		[In]	超时时间秒
         lMinSecond	long		[In]	超时时间毫秒
         返回值：实际接收到的字节数，超时返回-1
         */
        int ReceiveFrom(char* lpBuf, int nBufLen, std::string& strFromIP, int& nFromPort, bool bCheckDataLength = false, long lSecond = 0, long lMinSecond = 0);

        /*
         功能：客户端方法，连接服务器
         参数：
         lpszHostAddress	LPCTSTR		[In]	对方Ip地址或域名
         nHostPort		UINT		[In]	对方监听的端口
         返回值：成功返回TRUE,失败返回FALSE
         */
        bool Connect(const char* lpszHostAddress, unsigned short nHostPort, long lSecond = 10);

        /*
         功能：服务端方法，开始网络服务
         参数：
         rConnectedSocket	Socket	[In]	client socket对象
         返回值：成功返回TRUE，否则返回FALSE
         ※注：返回TRUE不表示rConnectedSocket等返回参数值有效，因为如果是非阻塞模式，
         无连接到达，该函数一样返回TRUE，而此时rConnectedSocket对象句柄将指向INVALID_SOCKET
         */
        bool StartServer(int nPort);

        /*
         功能：服务端方法，接收客户端连接
         参数：
         rConnectedSocket	Socket	[In]	client socket对象
         返回值：成功返回TRUE，否则返回FALSE
         ※注：返回TRUE不表示rConnectedSocket等返回参数值有效，因为如果是非阻塞模式，
         无连接到达，该函数一样返回TRUE，而此时rConnectedSocket对象句柄将指向INVALID_SOCKET
         */
        bool Accept(Socket& rConnectedSocket);

        //取得对方地址
        void GetPeerAddress(std::string& strWanIP, int& nWanPort);

        //取得本方地址
        void GetLocalAddress(std::string& strWanIP, int& nWanPort);

        /*
         功能：阻塞方式设置
         参数：
         bWait		bool	[In]	TRUE阻塞，FALSE非阻塞
         返回值：成功返回TRUE，否则返回FALSE
         */
        bool SetSockMode(bool bWait = false);

        /*
         功能：套接字设置，端口重用等
         参数：
         nOptionName		__int32		[In]	The socket option for which the value is to be set
         lpOptionValue	const void*	[In]	A pointer to the buffer in which the value for the requested option is supplied
         nOptionLen		__int32		[In]	lpOptionValue的大小
         nLevel			__int32		[In]	The level at which the option is defined; the supported levels include SOL_SOCKET and IPPROTO_TCP. See the Windows Sockets 2 Protocol-Specific Annex (a separate document included with the Platform SDK) for more information on protocol-specific levels
         返回值：成功返回TRUE，否则返回FALSE
         */
        bool SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET);

        //开启TCP_NODELAY设置,对于要频繁发送小数据的连接需要设置此设置提高吞吐能力
        bool SetNoDelay(bool yes);

        static bool SetNoDelay(int sock, bool yes);

        //设置发送缓冲大小
        bool SetSendBufSize(int buffsize);

        static bool SetSendBufSize(int sock, int buffsize);

        //设置接收缓冲大小
        bool SetRecvBufSize(int buffsize);

        static bool SetRecvBufSize(int sock, int buffsize);

        //设置发送超时
        bool SetSendTimeout(long sec, long usec);

        static bool SetSendTimeout(int sock, long sec, long usec);

        //设置接收超时
        bool SetRecvTimeout(long sec, long usec);

        static bool SetRecvTimeout(int sock, long sec, long usec);

        /*
         功能：Socket初始化
         参数：
         lpwsaData	WSADATA*	[In]	A pointer to a WSADATA structure. If lpwsaData is not equal to NULL, then the address of the WSADATA structure is filled by the call to ::WSAStartup. This function also ensures that ::WSACleanup is called for you before the application terminates.

         返回值：超时返回TRUE，否则返回FALSE
         */
        static bool SocketInit(void* lpVoid = NULL);

        static void SocketDestory();

        //返回最后的错误信息
        void GetLastErrorMsg(std::string& strError);

        //针对IOCP，可使用GetPeerName取地址信息
        //※只能在Connect之后调用
        static bool InitForIOCP(int hSocket, int listenSock);

        /*
         功能：绑定一个socket句柄，让该类对象在这个句柄上进行操作
         逻辑：
         傻瓜式绑定，不管类对象之前是否已经绑定了其它套接字，首先会调用傻瓜式函数close关闭连接，然后在绑定新的套接字，
         如果没有实现调用Detach解除旧绑定，那么旧的绑定sock将丢失
         参数：
         hSocket	int	[In]	要绑定sock句柄
         */
        void Attach(int hSocket);

        /*
         功能：解除绑定，返回绑定的socket句柄
         返回值：已绑定的socket句柄，可能是一个INVALID_SOCKET，说明之前没有任何绑定
         */
        int Detach();

        //初始化对方地址
        //※只能在Connect之后调用
        bool InitPeerAddress();

        //初始化本方地址
        //※只能在Connect之后调用
        bool InitLocalAddress();

    protected:
        /*
         功能：超时测试
         参数：
         lSecond		long	[In]	超时设置秒
         lMinSecond	long	[In]	超时设置毫秒
         返回值：超时返回TRUE，否则返回FALSE
         */
        bool TimeOut(long lSecond, long lMinSecond);

        /*
         功能：从sockaddr结构转换成常见类型表示的地址
         参数：
         sockAddr	sockaddr_in		[In]	地址信息
         strIP		string			[Out]	ip
         nPort		int				[Out]	端口
         */
        void GetAddress(const sockaddr_in* sockAddr, std::string& strIP, int& nPort);

        /*
         功能：服务端函数，绑定监听的端口与IP
         参数：
         nSocketPort			UINT		[In]	监听的端口
         lpszSocketAddress	LPCTSTR		[In]	IP
         返回值：成功返回TRUE，否则返回FALSE
         */
        bool Bind(unsigned short nPort, char* strIP = NULL);

        /*
         功能：服务端函数，开始监听
         参数：
         nConnectionBacklog	__int32	[In]	最大连接数
         返回值：成功返回TRUE，否则返回FALSE
         */
        bool Listen(int nConnectionBacklog = SOMAXCONN);

    public:
    private:
        int m_hSocket;    //sock句柄
        bool m_bBlock;    //阻塞标记
        bool m_bOpened;    //打开状态
        //为了不包含windows.h,定义一个绝对大于sockaddr_in的buffer
        char m_sockAddr[64];    //sockaddr_in
        std::string m_strWanIP;
        int m_nWanPort;
        std::string m_strLocalIP;
        int m_nLocalPort;
    };

}    //namespace mdf

#endif // MDF_SOCKET_H
