/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013-2014, Leiwenbin
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _XREDIS_CLIENT_H_
#define _XREDIS_CLIENT_H_

#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

namespace xrcp {

#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6


#define MAX_ERR_STR_LEN 128

    typedef std::string KEY;
    typedef std::string VALUE;
    typedef std::vector<KEY> KEYS;
    typedef KEYS FILEDS;
    typedef std::vector<VALUE> VALUES;
    typedef std::vector<std::string> VDATA;
    typedef std::set<std::string> SETDATA;

    typedef struct _REDIS_NODE_ {
        uint32_t sliceIndex;        //  节点编号索引，从0开始，自动生成
        const char* host;           //  REDIS节点主机IP地址
        uint32_t port;              //  REDIS服务端口
        const char* passwd;         //  REDIS认证密码
        uint32_t poolSize;          //  此节点上每个每片的连接池大小
        uint32_t timeout;           //  连接超时时间(秒)
        uint32_t role;              //  节点角色
    } RedisNode;

/* This is the reply object returned by redisCommand() */
    typedef struct rReply {
        int32_t type; /* REDIS_REPLY_* */
        long long integer; /* The integer when type is REDIS_REPLY_INTEGER */
        int32_t len; /* Length of string */
        char* str; /* Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
        size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */
        struct rReply** element; /* elements vector for REDIS_REPLY_ARRAY */
    } rReply;

    typedef uint32_t (* HASHFUN)(const char*);

    class RedisPool;

    class xRedisClient;

    class SliceIndex {
    public:
        SliceIndex(xRedisClient* xRedisClient, u_int32_t nodeIndex);

        ~SliceIndex();

        bool Create(const char* key, HASHFUN fun);

        bool CreateByID(int64_t id);

        const char* GetErrInfo() { return mStrerr.c_str(); }

        void SetIOMaster();

        uint32_t GetSliceIndex();

    private:
        bool SetErrInfo(const char* info, size_t len);

        void IOtype(uint32_t ioType);

        friend class xRedisClient;

    private:
        uint32_t mNodeIndex;
        uint32_t mSliceIndex;
        int32_t mErrlen;
        xRedisClient* mClient;
        uint32_t mIOtype;
        bool mIOFlag;
        string mStrerr;
    };

    typedef struct _DATA_ITEM_ {
        int32_t type;
        std::string str;

        _DATA_ITEM_& operator=(const _DATA_ITEM_& data) {
            type = data.type;
            str = data.str;
            return *this;
        }
    } DataItem;

    typedef std::vector<DataItem> ReplyData;
    typedef ReplyData ArrayReply;
    typedef std::map<std::string, double> ZSETDATA;
    typedef std::vector<SliceIndex> DBIArray;

    typedef struct xRedisContext_ {
        void* conn;
    } xRedisContext;

    typedef enum _SET_TYPE_ {
        TYPE_NONE = 0,
        PX = 1,
        EX = 2
    } SETPXEX;

    typedef enum _SET_TYPE_NXEX_ {
        TNXXX_NONE = 0,
        NX = 1,
        XX = 2
    } SETNXXX;

    typedef enum _BIT_OP_ {
        AND = 0,
        OR = 1,
        XOR = 2,
        NOT = 3
    } BITOP;

    typedef enum _LIST_MODEL_ {
        BEFORE = 0,
        AFTER = 1
    } LMODEL;


    typedef enum _SORT_ORDER_ {
        ASC = 0,
        DESC = 1
    } SORTODER;

    typedef struct _SORT_LIMIT_ {
        int32_t offset;
        int32_t count;
    } LIMIT;

    template<class T>
    std::string toString(const T& t) {
        ostringstream oss;
        oss << t;
        return oss.str();
    }

    typedef enum _REDIS_ROLE_ {
        MASTER = 0,
        SLAVE = 1
    } ROLE;

#define SETDEFAULTIOTYPE(type) if (!index.mIOFlag) {SetIOtype(index, type);}

    class xRedisClient {
    public:
        xRedisClient();

        ~xRedisClient();

        bool Init(uint32_t nodeCount);

        void Release();

        void Keepalive();

        inline RedisPool* GetRedisPool();

        static int32_t GetReply(xRedisContext* ctx, ReplyData& vData);

        bool GetxRedisContext(SliceIndex& index, xRedisContext* ctx);

        void FreexRedisContext(xRedisContext* ctx);

        bool ConnectRedisCache(RedisNode* redisNodeList, uint32_t redisNodeCount, uint32_t nodeIndex);

        bool ConnectRedisCacheWithNodeIndex(RedisNode* redisNodeList, uint32_t nodeCount, uint32_t nodeIndexForce);

        static void SetLogLevel(uint32_t level, void (*emit)(int level, const char* line));

    public:

        //              Connection
        /* AUTH        */  /* nonsupport */
        /* ECHO        */  bool echo(const SliceIndex& index, const std::string& str, std::string& value);
        /* PING        */  /* nonsupport */
        /* QUIT        */  void quit();

        /* SELECT      */  bool select(const SliceIndex& index, int32_t pos);

        //                 Commands operating on std::string values
        /* APPEND      */  bool append(const SliceIndex& index, const std::string& key, const std::string& value);

        /* BITCOUNT    */  bool bitcount(const SliceIndex& index, const std::string& key, int32_t& count, int32_t start = 0, int32_t end = 0);

        /* BITOP       */  bool bitop(const SliceIndex& index, const BITOP operation, const std::string& destkey, const KEYS& keys, int32_t& lenght);

        /* BITPOS      */  bool bitpos(const SliceIndex& index, const std::string& key, int32_t bit, int64_t& pos, int32_t start = 0, int32_t end = 0);

        /* DECR        */  bool decr(const SliceIndex& index, const std::string& key, int64_t& result);

        /* DECRBY      */  bool decrby(const SliceIndex& index, const std::string& key, int32_t by, int64_t& result);

        /* GET         */  bool get(const SliceIndex& index, const std::string& key, std::string& value);

        /* GETBIT      */  bool getbit(const SliceIndex& index, const std::string& key, int32_t& offset, int32_t& bit);

        /* GETRANGE    */  bool getrange(const SliceIndex& index, const std::string& key, int32_t start, int32_t end, std::string& out);

        /* GETSET      */  bool getset(const SliceIndex& index, const std::string& key, const std::string& newValue, std::string& oldValue);

        /* INCR        */  bool incr(const SliceIndex& index, const std::string& key, int64_t& result);

        /* INCRBY      */  bool incrby(const SliceIndex& index, const std::string& key, int32_t by, int64_t& result);
        /* INCRBYFLOAT */
        /* MGET        */  bool mget(const DBIArray& index, const KEYS& keys, ReplyData& vDdata);

        /* MSET        */  bool mset(const DBIArray& index, const VDATA& data);
        /* MSETNX      */
        /* PSETEX      */  bool psetex(const SliceIndex& index, const std::string& key, int32_t milliseconds, const std::string& value);

        /* SET         */  bool set(const SliceIndex& index, const std::string& key, const std::string& value);

        /* SET         */  bool set(const SliceIndex& index, const std::string& key, const char* value, int32_t len, int32_t second = 0);

        /* SET         */  bool set(const SliceIndex& index, const std::string& key, const std::string& value, SETPXEX pxex = TYPE_NONE, int32_t expiretime = 0, SETNXXX nxxx = TNXXX_NONE);

        /* SETBIT      */  bool setbit(const SliceIndex& index, const std::string& key, int32_t offset, int64_t newbitValue, int64_t oldbitValue);

        /* SETEX       */  bool setex(const SliceIndex& index, const std::string& key, int32_t seconds, const std::string& value);

        /* SETNX       */  bool setnx(const SliceIndex& index, const std::string& key, const std::string& value);

        /* SETRANGE    */  bool setrange(const SliceIndex& index, const std::string& key, int32_t offset, const std::string& value, int32_t& length);

        /* STRLEN      */  bool strlen(const SliceIndex& index, const std::string& key, int32_t& length);

        /* DEL         */  bool del(const SliceIndex& index, const std::string& key);

        /* DEL         */  bool del(const DBIArray& index, const KEYS& vkey, int64_t& count);
        /* DUMP        */
        /* EXISTS      */  bool exists(const SliceIndex& index, const std::string& key);

        /* EXPIRE      */  bool expire(const SliceIndex& index, const std::string& key, uint32_t second);

        /* EXPIREAT    */  bool expireat(const SliceIndex& index, const std::string& key, uint32_t timestamp);

        /* KEYS        */  bool keys(const SliceIndex& index, const string& key, KEYS& keys);

        /* MIGRATE     */
        /* MOVE        */
        /* OBJECT      */
        /* PERSIST     */  bool persist(const SliceIndex& index, const std::string& key);

        /* PEXPIRE     */  bool pexpire(const SliceIndex& index, const std::string& key, uint32_t milliseconds);

        /* PEXPIREAT   */  bool pexpireat(const SliceIndex& index, const std::string& key, uint32_t millisecondstimestamp);

        /* PTTL        */  bool pttl(const SliceIndex& index, const std::string& key, int64_t& milliseconds);

        /* RANDOMKEY   */  bool randomkey(const SliceIndex& index, KEY& key);
        /* RENAME      */
        /* RENAMENX    */
        /* RESTORE     */
        /* SCAN        */  bool scan(const SliceIndex& index, int64_t& cursor, const char* pattern, uint32_t count, ArrayReply& array, xRedisContext& ctx);

        /* SORT        */  bool sort(const SliceIndex& index, ArrayReply& array, const std::string& key, const char* by = NULL, LIMIT* limit = NULL, bool alpha = false, const FILEDS* get = NULL, const SORTODER order = ASC, const char* destination = NULL);

        /* TTL         */  bool ttl(const SliceIndex& index, const std::string& key, int64_t& seconds);

        /* TYPE        */  bool type(const SliceIndex& index, const std::string& key, std::string& value);

        /* HDEL        */  bool hdel(const SliceIndex& index, const std::string& key, const std::string& field, int64_t& num);

        /* HDEL        */  bool hdel(const SliceIndex& index, const std::string& key, const KEYS& vfiled, int64_t& num);

        /* HEXISTS     */  bool hexist(const SliceIndex& index, const std::string& key, const std::string& field);

        /* HGET        */  bool hget(const SliceIndex& index, const std::string& key, const std::string& field, std::string& value);

        /* HGETALL     */  bool hgetall(const SliceIndex& index, const std::string& key, ArrayReply& array);

        /* HINCRBY     */  bool hincrby(const SliceIndex& index, const std::string& key, const std::string& field, int64_t increment, int64_t& value);

        /* HINCRBYFLOAT*/  bool hincrbyfloat(const SliceIndex& index, const std::string& key, const std::string& field, const float increment, float& value);

        /* HKEYS       */  bool hkeys(const SliceIndex& index, const std::string& key, KEYS& keys);

        /* HLEN        */  bool hlen(const SliceIndex& index, const std::string& key, int64_t& count);

        /* HMGET       */  bool hmget(const SliceIndex& index, const std::string& key, const KEYS& field, ArrayReply& array);

        /* HMSET       */  bool hmset(const SliceIndex& index, const std::string& key, const VDATA& vData);

        /* HSCAN       */  bool hscan(const SliceIndex& index, const std::string& key, int64_t& cursor, const char* pattern, uint32_t count, ArrayReply& array, xRedisContext& ctx);

        /* HSET        */  bool hset(const SliceIndex& index, const std::string& key, const std::string& field, const std::string& value, int64_t& retval);

        /* HSETNX      */  bool hsetnx(const SliceIndex& index, const std::string& key, const std::string& field, const std::string& value);

        /* HVALS       */  bool hvals(const SliceIndex& index, const std::string& key, VALUES& values);

        /* BLPOP       */  bool blpop(const SliceIndex& index, const std::string& key, VALUES& values, int64_t& timeout);

        /* BRPOP       */  bool brpop(const SliceIndex& index, const std::string& key, VALUES& values, int64_t& timeout);
        /* BRPOPLPUSH  */
        /* LINDEX      */  bool lindex(const SliceIndex& index, const std::string& key, int64_t idx, VALUE& value);

        /* LINSERT     */  bool linsert(const SliceIndex& index, const std::string& key, LMODEL mod, const std::string& pivot, const std::string& value, int64_t& retval);

        /* LLEN        */  bool llen(const SliceIndex& index, const std::string& key, int64_t& len);

        /* LPOP        */  bool lpop(const SliceIndex& index, const std::string& key, std::string& value);

        /* LPUSH       */  bool lpush(const SliceIndex& index, const std::string& key, const VALUES& vValue, int64_t& length);

        /* LPUSHX      */  bool lpushx(const SliceIndex& index, const std::string& key, const std::string& value, int64_t& length);

        /* LRANGE      */  bool lrange(const SliceIndex& index, const std::string& key, int64_t start, int64_t end, ArrayReply& array);

        /* LREM        */  bool lrem(const SliceIndex& index, const std::string& key, int32_t count, const std::string& value, int64_t num);

        /* LSET        */  bool lset(const SliceIndex& index, const std::string& key, int32_t idx, const std::string& value);

        /* LTRIM       */  bool ltrim(const SliceIndex& index, const std::string& key, int32_t start, int32_t end);

        /* RPOP        */  bool rpop(const SliceIndex& index, const std::string& key, std::string& value);

        /* RPOPLPUSH   */  bool rpoplpush(const SliceIndex& index, const std::string& key_src, const std::string& key_dest, std::string& value);

        /* RPUSH       */  bool rpush(const SliceIndex& index, const std::string& key, const VALUES& vValue, int64_t& length);

        /* RPUSHX      */  bool rpushx(const SliceIndex& index, const std::string& key, const std::string& value, int64_t& length);

        /* SADD        */  bool sadd(const SliceIndex& index, const KEY& key, const VALUES& vValue, int64_t& count);

        /* SCARD       */  bool scard(const SliceIndex& index, const KEY& key, int64_t& count);

        /* SDIFF       */  bool sdiff(const DBIArray& index, const KEYS& vKkey, VALUES& vValue);

        /* SDIFFSTORE  */  bool sdiffstore(const SliceIndex& index, const KEY& destinationkey, const DBIArray& vdbi, const KEYS& vkey, int64_t& count);

        /* SINTER      */  bool sinter(const DBIArray& index, const KEYS& vkey, VALUES& vValue);

        /* SINTERSTORE */  bool sinterstore(const SliceIndex& index, const KEY& destinationkey, const DBIArray& vdbi, const KEYS& vkey, int64_t& count);

        /* SISMEMBER   */  bool sismember(const SliceIndex& index, const KEY& key, const VALUE& member);

        /* SMEMBERS    */  bool smembers(const SliceIndex& index, const KEY& key, VALUES& vValue);

        /* SMOVE       */  bool smove(const SliceIndex& index, const KEY& srckey, const KEY& deskey, const VALUE& member);

        /* SPOP        */  bool spop(const SliceIndex& index, const KEY& key, VALUE& member);

        /* SRANDMEMBER */  bool srandmember(const SliceIndex& index, const KEY& key, VALUES& vmember, int32_t num = 0);

        /* SREM        */  bool srem(const SliceIndex& index, const KEY& key, const VALUES& vmembers, int64_t& count);

        /* SSCAN       */  bool sscan(const SliceIndex& index, const std::string& key, int64_t& cursor, const char* pattern, uint32_t count, ArrayReply& array, xRedisContext& ctx);

        /* SUNION      */  bool sunion(const DBIArray& index, const KEYS& vkey, VALUES& vValue);

        /* SUNIONSTORE */  bool sunionstore(const SliceIndex& index, const KEY& deskey, const DBIArray& vdbi, const KEYS& vkey, int64_t& count);


        /* ZADD        */  bool zadd(const SliceIndex& index, const KEY& deskey, const VALUES& vValues, int64_t& count);

        /* ZCARD       */  bool zscrad(const SliceIndex& index, const std::string& key, int64_t& num);
        /* ZCOUNT      */
        /* ZINCRBY     */  bool zincrby(const SliceIndex& index, const std::string& key, const double& increment, const std::string& member, std::string& value);
        /* ZINTERSTORE */
        /* ZRANGE      */  bool zrange(const SliceIndex& index, const std::string& key, int32_t start, int32_t end, VALUES& vValues, bool withscore = false);

        /* ZRANGEBYSCORE*/ bool zrangebyscore(const SliceIndex& index, const std::string& key, const std::string& min, const std::string& max, VALUES& vValues, bool withscore = false, LIMIT* limit = NULL);

        /* ZRANK       */  bool zrank(const SliceIndex& index, const std::string& key, const std::string& member, int64_t& rank);

        /* ZREM        */  bool zrem(const SliceIndex& index, const KEY& key, const VALUES& vmembers, int64_t& num);

        /* ZREMRANGEBYRANK  */  bool zremrangebyrank(const SliceIndex& index, const std::string& key, int32_t start, int32_t stop, int64_t& num);

        /* ZREMRANGEBYSCORE */  bool zremrangebyscore(const SliceIndex& index, const KEY& key, double min, double max, int64_t& count);

        /* ZREVRANGE        */  bool zrevrange(const SliceIndex& index, const std::string& key, int32_t start, int32_t end, VALUES& vValues, bool withscore = false);
        /* ZREVRANGEBYSCORE */
        /* ZREVRANK         */  bool zrevrank(const SliceIndex& index, const std::string& key, const std::string& member, int64_t& rank);

        /* ZSCAN            */  bool zscan(const SliceIndex& index, const std::string& key, int64_t& cursor, const char* pattern, uint32_t count, ArrayReply& array, xRedisContext& ctx);

        /* ZSCORE           */  bool zscore(const SliceIndex& index, const std::string& key, const std::string& member, std::string& score);
        /* ZUNIONSTORE      */


        /*  注意： 使用下面的 pub/sub 命令时，一定要保证数据都在同一个REDIS实例里，xredis目前的pub/sub命令实现不支持多节点数据分布的场景。  */
        /* PSUBSCRIBE   */     bool psubscribe(const SliceIndex& index, const KEYS& patterns, xRedisContext& ctx);

        /* PUBLISH      */     bool publish(const SliceIndex& index, const KEY& channel, const std::string& message, int64_t& count);

        /* PUBSUB       */     bool pubsub_channels(const SliceIndex& index, const std::string& pattern, ArrayReply& reply);

        /* PUBSUB       */     bool pubsub_numsub(const SliceIndex& index, const KEYS& keys, ArrayReply& reply);

        /* PUBSUB       */     bool pubsub_numpat(const SliceIndex& index, int64_t& count);

        /* PUNSUBSCRIBE */     bool punsubscribe(const SliceIndex& index, const KEYS& patterns, xRedisContext& ctx);

        /* SUBSCRIBE    */     bool subscribe(const SliceIndex& index, const KEYS& channels, xRedisContext& ctx);

        /* UNSUBSCRIBE  */     bool unsubscribe(const SliceIndex& index, const KEYS& channels, xRedisContext& ctx);


        /* DISCARD  */
        /* EXEC     */
        /* MULTI    */
        /* UNWATCH  */
        /* WATCH    */


    private:
        void addParam(VDATA& vDes, const VDATA& vSrc);

        void SetErrInfo(const SliceIndex& index, void* p);

        void SetErrString(const SliceIndex& index, const char* str, size_t len);

        void SetErrMessage(const SliceIndex& index, const char* fmt, ...);

        void SetIOtype(const SliceIndex& index, uint32_t ioType, bool ioFlag = false);

        bool ScanFun(const char* cmd, const SliceIndex& index, const std::string* key, int64_t& cursor, const char* pattern, uint32_t count, ArrayReply& array, xRedisContext& ctx);

    public:

        bool command_bool(const SliceIndex& index, const char* cmd, ...);

        bool command_status(const SliceIndex& index, const char* cmd, ...);

        bool command_integer(const SliceIndex& index, int64_t& intval, const char* cmd, ...);

        bool command_string(const SliceIndex& index, std::string& data, const char* cmd, ...);

        bool command_list(const SliceIndex& index, VALUES& vValue, const char* cmd, ...);

        bool command_array(const SliceIndex& index, ArrayReply& array, const char* cmd, ...);

        rReply* command(const SliceIndex& index, const char* cmd);

    private:
        bool commandargv_bool(const SliceIndex& index, const VDATA& vData);

        bool commandargv_status(const SliceIndex& index, const VDATA& vData);

        bool commandargv_array(const SliceIndex& index, const VDATA& vDataIn, ArrayReply& array);

        bool commandargv_array(const SliceIndex& index, const VDATA& vDataIn, VALUES& array);

        bool commandargv_integer(const SliceIndex& index, const VDATA& vDataIn, int64_t& retval);

        bool commandargv_array_ex(const SliceIndex& index, const VDATA& vDataIn, xRedisContext& ctx);

    private:
        RedisPool* mRedisPool;
    };
}

#endif





