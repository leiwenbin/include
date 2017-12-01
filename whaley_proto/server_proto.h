/*
 * server_proto.h
 *
 *  Created on: 2016年3月3日
 *      Author: leiwenbin
 */

#ifndef SERVER_PROTO_H_
#define SERVER_PROTO_H_

#pragma pack(push)
#pragma pack(1)

#include "transport_proto.h"
#include <string.h>

#define COMMON_SERVER_LOGIN_REQ         0x10000100    //服务器登录请求
#define COMMON_SERVER_LOGIN_RET         0x10000101    //服务器登录返回

#define COMMON_CLIENT_LOGOUT_REQ        0x10000102    //客户端退出请求
#define COMMON_CLIENT_LOGOUT_RET        0x10000103    //客户端退出返回

#define COMMON_SYNC_HOST_REQ            0x10000104    //HOST同步SESSION

#define COMMON_MSG_EXCEPTION_REQ        0x10000106    //HOST消息发送发生异常

#define COMMON_PUSH_RADIO_MSG_RET       0x10000107    //在线广播消息
#define COMMON_PUSH_ALL_RADIO_MSG_RET   0x10000109    //全量广播消息

#define COMMON_CLIENT_EXCEPTION_REQ     0x10000110    //客户端再上线之前异常连接断开的消息

#define COMMON_RadioMSG_STATUS_REQ       0x10000111    //在线广播消息回执状态码

#define COMMON_CONSLOE_LOGIN_REQ        0x10000301    //CONSOLE控制台服务登录请求
#define COMMON_CONSLOE_LOGIN_RET        0x10000303    //CONSOLE控制台服务登录返回

#define COMMON_AGENT_LOST_RET           0x10000501    //AGENT已断开

#define COMMON_SYSTEM_KICK_REQ          0x10000701    //系统踢出终端指令

#define PRODUCT_LENGTH      32

//SERVER ROLE
typedef enum ServerRole {
    NONE = 0, CENTER_SERVER, HOST_SERVER, PUSH_SERVER, CONSOLE_SERVER
} SERVER_ROLE;

//目标类型
typedef enum TargetType {
    TARGET_CLIENT = 0, //终端
    TARGET_USER,       //帐户
    TARGET_MULTI       //终端+帐户
} TARGET_TYPE;

//推送类型
typedef enum PushType {
    PUSH_NONE = 0,
    PUSH_ALL,          //在线+离线
    PUSH_ONLINE        //在线
} PUSH_TYPE;

//回执类型
typedef enum ReceiptLv {
    NOT_NEED_RECEIPT = 0,       //不需要回执
    MUST_NEED_RECEIPT           //需要回执
} RECEIPT_LV;

//系统指令
typedef enum SysCmd {
    NO_CMD = 0,
    CMD_KICK           //踢出终端
} SYS_CMD;

typedef struct Target {
    char uid_key[128];
    char account_key[128];
    TARGET_TYPE target_type;
} TARGET;

typedef struct Targets {
    uint32_t target_count;
    Target target[0];
} TARGETS;

typedef struct PushMsg {
    long long int failure_time;
    long long int send_time;
    char product[32];
    char msg_id[64];
    RECEIPT_LV receipt_lv;
    PUSH_TYPE push_type;
    uint32_t msg_len;
    char msg_buf[0];
} PUSH_MSG;

typedef struct SvrInfo {
    char ip[32];
    uint32_t port;
    uint32_t connection_count;
    uint32_t connection_max;
    uint32_t client_count;
    uint32_t user_count;
    char area[32];
    char isp[32];
    uint32_t load;
    uint32_t memory_used;
    uint32_t online;
    uint32_t credibility;
    ServerRole svr_role;
    long long timestamp;
} SVR_INFO;

typedef struct HostSession {
    uint32_t have_user;
    char uid_key[128];
    char account_key[128];
} HOST_SESSION;

typedef struct DataBlock {
    char ip[32];
    char product[32];
    char uid[128];
    char accountid[128];
    uint32_t block_len;
    char block_buf[0];
} DATA_BLOCK;

typedef enum MsgError {
    SEND_SUCCESS = 0x00,
    CLIENT_NOT_FOUND,       //终端未找到
    SEND_ERROR,             //数据发送错误
    MSG_EXPIRE,             //消息已经失效
    FORMAT_INVALID,         //消息格式错误
    TARGET_NOT_FOUND,       //消息目标未找到
    REDIS_EXEC_ERROR,       //数据操作异常
    NODE_NOT_FOUND,         //无可用节点
    NOT_RECEIPT,            //消息未回执
    CONVERT_OFFLINE,        //转存离线消息
    CONVERT_DISCARD,        //离线丢弃
    SEND_BEGIN,             //在线消息开始推送
    CREATE_FAILED,          //消息推送任务创建失败
    DUPLICATE_DISCARD,      //重复消息过滤
    OFFLINE_SEND_BEGIN,      //离线消息开始推送
    TOO_MANY_RETRIES        //消息重发次数过多
} MSG_ERROR;

typedef struct MsgException {
    TARGET target;
    MSG_ERROR msg_error;
    PUSH_MSG push_msg;
} MSG_EXCEPTION;

typedef struct MsgReceiptVal {
    char msg_id[64];
    TARGET target;
} MSG_RECEIPT_VAL;

typedef struct ClientLoginParams {
    char product[128];
    char uid_key[128];
    char ip[32];
    SVR_INFO* pSvrInfo;
    char client_info[1024];
} CLIENT_LOGIN_PARAMS;

typedef struct ClientLogoutParams {
    char uid_key[128];
} CLIENT_LOGOUT_PARAMS;

typedef struct UserLoginParams {
    char product[128];
    char uid_key[128];
    char account_key[128];
    char ip[32];
    SVR_INFO* pSvrInfo;
    char user_info[1024];
} USER_LOGIN_PARAMS;

typedef struct UserLogoutParams {
    char uid_key[128];
    char account_key[128];
    int unbindStatus;
} USER_LOGOUT_PARAMS;


typedef struct OfflineMsgParams {
    TARGET target;
} OFFLINE_MSG_PARAMS;

struct ConnByProduct
{
public:
    ConnByProduct():product({0}),client_count(0),user_count(0)
    {

    }
    ConnByProduct(const char*product,int32_t client_count,int32_t user_count)
            :product({0}),client_count(client_count),user_count(user_count)
    {
        if(product != NULL)
        {
            memcpy(this->product,product,strlen(product));
        }
    }

public:
    char product[PRODUCT_LENGTH];
    int32_t client_count;
    int32_t user_count;
};

struct ConnByProducts
{
    ConnByProducts():number(0)
    {

    }

    uint32_t number;  //需要发送的产品线的数目
    ConnByProduct connByProducts[0];
};

struct RadioMsgData
{
    RadioMsgData(int32_t qps,const char* in_svr_key, const char* business_type,int32_t command_type, TARGET_TYPE target_type)
            :qps(qps),svr_key({0}), business_type({0}),command_type(command_type),target_type(target_type)
    {
        {
            size_t len = strlen(in_svr_key);
            len = len > 63 ? 63 : len;
            memcpy(this->svr_key, in_svr_key, len);
        }

        {
            size_t len = strlen(business_type);
            len = len > 63?63:len;
            memcpy(this->business_type,business_type,len);
        }
    }

    int32_t qps;
    char svr_key[64];
    char business_type[64];
    int32_t command_type;
    TARGET_TYPE target_type;
};

enum BroadCast_CommandType
{
    BROADCAST = 0,
    MULTICAST_USER_ONLINE,
    MULTICAST_USER_OFFLINE
};

struct RadioMsg_status
{
    char svr_key[64];
    int is_finished;
    int64_t timeStamp;
    MSG_ERROR msg_error;
    TARGETS targets;
};

struct BroadCastMsgParams_ret
{
    PUSH_MSG pPushMsg;
    Target radioTarget;
    RadioMsg_status pRadioMsg;
};

struct BroadCastMsgParams_req
{
    RadioMsgData radioMsgData;
    PUSH_MSG pushMsg;
};


#pragma pack(pop)

#endif /* SERVER_PROTO_H_ */
