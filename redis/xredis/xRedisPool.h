/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013-2014, Leiwenbin
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _XREDIS_POOL_H_
#define _XREDIS_POOL_H_

#include "hiredis.h"
#include "xLock.h"
#include <string.h>
#include <string>
#include <list>
#include "xRedisClient.h"

using namespace std;

namespace xrcp {

#define MAX_REDIS_NODE_COUNT        128      // 最大支持的NODE数量
#define MAX_REDIS_SLICE_COUNT       128      // NODE中最大支持的分片数量
#define MAX_REDIS_CONN_POOL_COUNT     128      // SLICE中每个连接池支最大支持的连接数

#define GET_CONNECT_ERROR       "get connection error"
#define CONNECT_CLOSED_ERROR    "redis connection be closed"

#ifdef WIN32
#define   strcasecmp   stricmp
#define   strncasecmp  strnicmp
#endif

    enum {
        REDISDB_UNCONN,
        REDISDB_WORKING,
        REDISDB_DEAD
    };

    class RedisConn {
    public:
        RedisConn();

        ~RedisConn();

        void Init(uint32_t nodeIndex,
                  uint32_t sliceIndex,
                  const std::string& host,
                  uint32_t port,
                  const std::string& pass,
                  uint32_t poolSize,
                  uint32_t timeout,
                  uint32_t role,
                  uint32_t slaveidx
        );

        bool RedisConnect();

        bool RedisReConnect();

        bool Ping();

        redisContext* getCtx() const { return mCtx; }

        uint32_t getSliceIndex() const { return mSliceIndex; }

        uint32_t GetNodeIndex() const { return mNodeIndex; }

        uint32_t GetRole() const { return mRole; }

        uint32_t GetSlaveIdx() const { return mSlaveIdx; }

        bool GetConnstatus() const { return mConnStatus; }

    private:
        bool auth();

        redisContext* ConnectWithTimeout();

    private:
        // redis connector context
        redisContext* mCtx;
        string mHost;               // redis host
        uint32_t mPort;             // redis sever port
        string mPass;               // redis server password
        uint32_t mTimeout;          // connect timeout second
        uint32_t mPoolsize;         // connect pool size for each redis DB
        uint32_t mNodeIndex;        // redis node index
        uint32_t mSliceIndex;       // redis slice index
        uint32_t mRole;             // redis role
        uint32_t mSlaveIdx;         // the index in the slave group
        bool mConnStatus;           // redis connection status
    };

    typedef std::list<RedisConn*> RedisConnPool;
    typedef std::list<RedisConn*>::iterator RedisConnIter;

    typedef std::vector<RedisConnPool*> RedisSlaveGroup;
    typedef std::vector<RedisConnPool*>::iterator RedisSlaveGroupIter;

    typedef struct _RedisDBSlice_Conn_ {
        RedisConnPool RedisMasterConn;
        RedisSlaveGroup RedisSlaveConn;
        xLock MasterLock;
        xLock SlaveLock;
    } RedisSliceConn;


    class RedisDBSlice {
    public:
        RedisDBSlice();

        ~RedisDBSlice();

        void Init(uint32_t nodeIndex, uint32_t sliceIndex);

        // 连到到一个REDIS服务节点
        bool ConnectRedisNodes(uint32_t nodeIndex, uint32_t sliceIndex, const std::string& host, uint32_t port, const std::string& passwd, uint32_t poolsize, uint32_t timeout, uint32_t role);

        RedisConn* GetMasterConn();

        RedisConn* GetSlaveConn();

        RedisConn* GetConn(int32_t ioRole);

        void FreeConn(RedisConn* redisconn);

        void CloseConnPool();

        void ConnPoolPing();

        uint32_t GetStatus() const;

    private:
        RedisSliceConn mSliceConn;
        bool mHaveSlave;
        uint32_t mNodeIndex;          // redis node index
        uint32_t mSliceIndex;         // redis slice index
        uint32_t mStatus;             // redis DB status
    };

    class RedisCacheNode {
    public:
        RedisCacheNode();

        virtual ~RedisCacheNode();

        bool InitDB(uint32_t mNodeCount, uint32_t mSliceCount);

        bool ConnectRedisDB(uint32_t nodeIndex, uint32_t sliceIndex, const string& host, uint32_t port, const string& passwd, uint32_t poolSize, uint32_t timeout, uint32_t role);

        RedisConn* GetConn(uint32_t dbindex, uint32_t ioRole);

        void FreeConn(RedisConn* redisconn);

        void ClosePool();

        void KeepAlive();

        uint32_t GetSliceStatus(uint32_t sliceIndex);

        uint32_t GetSliceCount() const;

    private:
        RedisDBSlice* mRedisDBSliceList;
        uint32_t mNodeIndex;
        uint32_t mSliceCount;
    };


    class RedisPool {
    public:
        RedisPool();

        ~RedisPool();

        bool Init(uint32_t nodeCount);

        bool SetSliceCount(uint32_t nodeIndex, uint32_t sliceCount);

        uint32_t GetSliceCount(uint32_t nodeIndex);

        bool ConnectRedisDB(uint32_t cachetype, uint32_t dbindex, const string& host, uint32_t port, const string& passwd, uint32_t poolSize, uint32_t timeout, uint32_t role);

        static bool CheckReply(const redisReply* reply);

        static void FreeReply(const redisReply* reply);

        RedisConn* GetConnection(uint32_t cachetype, uint32_t index, uint32_t ioType = MASTER);

        void FreeConnection(RedisConn* redisconn);

        void Keepalive();

        void Release();

        uint32_t GetNodeCount();

    private:
        RedisCacheNode* mRedisCacheNodeList;
        uint32_t mNodeCount;
    };
}


#endif
