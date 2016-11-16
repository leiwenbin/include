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

#define COMMON_SERVER_LOGIN_REQ         0x10000100    //服务器登录请求
#define COMMON_SERVER_LOGIN_RET         0x10000101    //服务器登录返回

#define COMMON_CLIENT_LOGOUT_REQ        0x10000102    //客户端退出请求
#define COMMON_CLIENT_LOGOUT_RET        0x10000103    //客户端退出返回

#define COMMON_SYNC_HOST_REQ            0x10000104    //HOST同步SESSION

#define COMMON_MSG_EXCEPTION_REQ        0x10000106    //HOST消息发送发生异常

#define COMMON_PUSH_RADIO_MSG_RET       0x10000107    //在线广播消息
#define COMMON_PUSH_ALL_RADIO_MSG_RET   0x10000109    //全量广播消息

#define COMMON_CLIENT_EXCEPTION_REQ     0x10000110    //客户端再上线之前异常连接断开的消息

#define COMMON_CONSLOE_LOGIN_REQ        0x10000301    //CONSOLE控制台服务登录请求
#define COMMON_CONSLOE_LOGIN_RET        0x10000303    //CONSOLE控制台服务登录返回

#define COMMON_AGENT_LOST_RET           0x10000501    //AGENT已断开

//SERVER ROLE
typedef enum ServerRole {
    NONE = 0, CENTER_SERVER, HOST_SERVER, PUSH_SERVER, CONSOLE_SERVER
} SERVER_ROLE;

//TARGET TYPE
typedef enum TargetType {
    TARGET_CLIENT = 0, TARGET_USER, TARGET_MULTI
} TARGET_TYPE;

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
    char product[32];
    char msg_id[64];
    int receipt;
    int push_type;
    uint32_t msg_len;
    char msg_buf[0];
} PUSH_MSG;

typedef struct SortPushMsg {
    long long int send_time;
    PUSH_MSG* push_msg;
} SORT_PUSH_MSG;

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
    ServerRole svr_role;
    long long timestamp;
} SVR_INFO;

typedef struct HostSession {
    uint32_t have_user;
    char uid_key[128];
    char account_key[128];
} HOST_SESSION;

typedef struct DataBlock {
    uint32_t block_len;
    char product[128];
    char block_buf[0];
} DATA_BLOCK;

typedef enum MsgError {
    SEND_SUCCESS = 0x00,
    CLIENT_NOT_FOUND,
    SEND_ERROR,
    /* retry error 3~7 */
    NOT_RECEIPT = 0x08,
    CONVERT_OFFLINE
} MSG_ERROR;

typedef struct MsgException {
    TARGET target;
    MSG_ERROR msg_error;
    PUSH_MSG push_msg;
} MSG_EXCEPTION;

typedef struct MsgKeyAndAccountKey {
    char msg_id[64];
    char account_key[128];
} MSG_KEY_AND_ACCOUNT_KEY;

//消息推送类型
typedef enum PushType {
    PUSH_NONE = 0,
    PUSH_SINGLE = 2,            //单播在线+离线
    PUSH_SINGLE_ONLINE,         //单播在线
    PUSH_SINGLE_MULTI,          //设备+帐号单播在线+离线
    PUSH_SINGLE_MULTI_ONLINE,   //设备+帐号单播在线
    PUSH_ALL,                   //广播在线+离线
    PUSH_ALL_ONLINE,            //广播在线
    PUSH_MULTICAST_NO_LOGIN,    //多播未登录帐号的客户端
    PUSH_MULTICAST_LOGIN        //多播已登录帐号的客户端
} PUSH_TYPE;

#pragma pack(pop)

#endif /* SERVER_PROTO_H_ */
