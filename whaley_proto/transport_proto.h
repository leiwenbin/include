/*
 * transport_proto.h
 *
 *  Created on: 2016年3月2日
 *      Author: leiwenbin
 */

#ifndef TRANSPORT_PROTO_H_
#define TRANSPORT_PROTO_H_

#pragma pack(push)
#pragma pack(1)

#include "type_proto.h"

//VERSION
#define COMMON_PROTOCOL_VER      130           //协议版本号

//COMMAND
#define COMMON_KEEP_ALIVE_REQ    0x10000020    //全局心跳请求
#define COMMON_KEEP_ALIVE_RET    0x10000021    //全局心跳返回

#define COMMON_CLIENT_LOGIN_REQ  0x10000200    //客户端登录请求
#define COMMON_CLIENT_LOGIN_RET  0x10000201    //客户端登录返回

#define COMMON_USER_LOGIN_REQ    0x10000202    //用户登录请求
#define COMMON_USER_LOGIN_RET    0x10000203    //用户登录返回

#define COMMON_USER_LOGOUT_REQ   0x10000204    //用户登出请求
#define COMMON_USER_LOGOUT_RET   0x10000205    //用户登出返回

#define COMMON_PUSH_MSG_RET      0x10000211    //消息推送通知
#define COMMON_RECEIPT_MSG_REQ   0x10000212    //消息回执

#define COMMON_EXEC_CMD_REQ      0x10000401    //控制台执行命令请求
#define COMMON_EXEC_CMD_RET      0x10000403    //终端执行命令结果返回

#define COMMON_AGENT_LOGIN_REQ     0x10000600    //AGENT登录请求
#define COMMON_AGENT_LOGIN_RET     0x10000601    //AGENT登录返回
#define COMMON_AGENT_CHANNEL_REQ   0x10000602    //AGENT安全通道建立请求
#define COMMON_AGENT_CHANNEL_RET   0x10000603    //AGENT安全通道建立返回
#define COMMON_AGENT_CHANNEL_ACK   0x10000604    //AGENT安全通道建立确认
#define COMMON_AGENT_LOGOUT_REQ    0x10000606    //AGENT主动退出请求

//UUID
struct UUID_s {
    union {
        BYTE uuid[16];
    };
};

//ERROR
typedef enum ErrorCode {
    OK = 0
} ERROR_CODE;

//TASK STATUS
typedef enum TaskStatus {
    FINISH = 0
} TASK_STATUS;

//BASIC MSG HEAD
struct BaseHeader {
    uint32_t uiSize; //消息长度
    uint32_t uiCmd; //指令
    uint32_t uiVersion; //版本号
};

//KEEP ALIVE MSG
typedef struct CommonKeepAlive : public BaseHeader {
    uint32_t uiBuffSize; //BUFF的长度
    BYTE chBuff[0]; //BUFF内容
} MsgCommonKeepAlive, * PMsgCommonKeepAlive;

//SYSTEM MIDDLE HEAD
typedef struct PlatformMiddleHeader : public BaseHeader {
    long long int llTimeStamp; //时间戳
    UUID_s uTaskID; //任务唯一ID
    TASK_STATUS uiTaskStatus; //任务状态
    ERROR_CODE uiRetCode; //返回码
    uint16_t usIsCompress; //BUFF是否压缩(0:未压缩, 1:压缩)
    uint32_t uiPreCompressSize; //压缩前长度(如果usIsCompress=0,则该字段无意义)
    uint32_t uiToken; //安全令牌
    uint32_t uiBuffSize; //BUFF的长度
    BYTE chBuff[0]; //BUFF内容
} MsgPlatformMiddleHeader, * PMsgPlatformMiddleHeader;

#pragma pack(pop)

#endif /* TRANSPORT_PROTO_H_ */
