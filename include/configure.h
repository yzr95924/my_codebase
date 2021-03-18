/**
 * @file configure.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the necessary variables in deduplication
 * @version 0.1
 * @date 2019-12-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef BASICDEDUP_CONFIGURE_h
#define BASICDEDUP_CONFIGURE_h

#include <bits/stdc++.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "define.h"
#include "constVar.h"
using namespace std;

class Configure {
private:

    // chunking setting
    uint64_t chunkingType_; // varSize \ fixedSize \ simple
    uint64_t maxChunkSize_;
    uint64_t avgChunkSize_;
    uint64_t minChunkSize_;
    uint64_t slidingWinSize_;
    uint64_t segmentSize_; // if exist segment function
    uint64_t readSize_; //128MB per time 
    

    // deduplication setting 
    std::string recipeRootPath_;
    std::string containerRootPath_;
    std::string metaRootPath_;
    std::string fp2ChunkDBName_;
    std::string fp2MetaDBName_;
    

    // restore setting
    uint64_t restoreBatchSize_;
    uint64_t readCacheSize_;
    
    // compression setting
    string enableCompression_;
    bool enableCompressionFlag_;
    string useCache_;
    bool useCacheFlag_;
    

    // backup setting
    std::string file2MetaDBName_;
    std::string tarDir_;
    std::string restoreTarDir_;


    // nCloud setting
    std::string redisIP_;
    uint16_t redisPort_;
    uint32_t redisReplyWaitTimeout_ = 0;
    std::string redisPassword_;

    // dataWriter setting
    std::string backupEndType_;
    uint32_t threadNum_; 

    // for storage ip
    std::string storageServerIp_;
    int storageServerPort_;

    // client id
    int clientID_;

    /**
     * @brief read the configure file
     * 
     * @param path the configure file name
     */
    void ReadConf(std::string path);

public:

    /**
     * @brief Construct a new Configure object
     * 
     * @param path the input configure file path
     */
    Configure(std::string path);

    /**
     * @brief Destroy the Configure object
     * 
     */
    ~Configure();
    
    // chunking setting
    /**
     * @brief Get the Chunking Type object
     * 
     * @return uint64_t chunking type
     */
    inline uint64_t GetChunkingType() {
        return chunkingType_;
    }

    /**
     * @brief Get the Max Chunk Size object
     * 
     * @return uint64_t max chunk size
     */
    inline uint64_t GetMaxChunkSize() {
        return maxChunkSize_;
    }

    /**
     * @brief Get the Min Chunk Size object
     * 
     * @return uint64_t min chunk size
     */
    inline uint64_t GetMinChunkSize() {
        return minChunkSize_;
    }

    /**
     * @brief Get the Avg Chunk Size object
     * 
     * @return uint64_t average chunk size
     */
    inline uint64_t GetAvgChunkSize() {
        return avgChunkSize_;
    }

    /**
     * @brief Get the Sliding Win Size object
     * 
     * @return uint64_t sliding window size
     */
    inline uint64_t GetSlidingWinSize() {
        return slidingWinSize_;
    }

    /**
     * @brief Get the Segment Size object
     * 
     * @return uint64_t segment size
     */
    inline uint64_t GetSegmentSize() {
        return segmentSize_;
    }

    /**
     * @brief Get the Read Size object
     * 
     * @return uint64_t the read buffer size
     */
    inline uint64_t GetReadSize() {
        return readSize_;
    }
    
    /**
     * @brief Get the Recipe Root Path object
     * 
     * @return string the file recipe path
     */
    inline string GetRecipeRootPath() {
        return recipeRootPath_;
    }

    /**
     * @brief Get the Container Root Path object
     * 
     * @return string the container root path
     */
    inline string GetContainerRootPath() {
        return containerRootPath_;
    }

    /**
     * @brief Get the Meta Root Path object
     * 
     * @return string the metadata root path
     */
    inline string GetMetaRootPath() {
        return metaRootPath_;
    }

    /**
     * @brief Get the Fp 2 Chunk D B Name object
     * 
     * @return string the database of fingerprint to chunk
     */
    inline string GetFp2ChunkDBName() {
        return fp2ChunkDBName_;
    }

    /**
     * @brief Get the Fp 2 Meta D B Name object
     * 
     * @return string the database of file metadata
     */
    inline string GetFp2MetaDBName() {
        return fp2MetaDBName_;
    }

    /**
     * @brief Get the Restore Batch Size object
     * 
     * @return uint64_t get the restore batch size
     */
    inline uint64_t GetRestoreBatchSize() {
        return restoreBatchSize_;
    }

    inline uint64_t GetReadCacheSize() {
        return readCacheSize_;
    }

    inline bool GetCompressionFlag() {
        return enableCompressionFlag_;
    }

    inline string GetFile2MetaDBName() {
        return file2MetaDBName_;
    }

    inline string GetTarDir() {
        return tarDir_;
    }

    inline string GetRestoreTarDir() {
        return restoreTarDir_;
    }


    // for nCloud redis connection
    /**
     * @brief Get the Redis IP object
     * 
     * @return string the redis ip address 
     */
    inline string GetRedisIP() {
        return redisIP_;
    }

    /**
     * @brief Get the Redis Port object
     * 
     * @return uint16_t the redis port 
     */
    inline uint16_t GetRedisPort() {
        return redisPort_;
    }

    /**
     * @brief Get the Reply Time Out object
     * 
     * @return uint32_t get the reply time of redis
     */
    inline uint32_t GetReplyTimeOut() {
        return redisReplyWaitTimeout_;
    }

    /**
     * @brief Get the Redis Password object
     * 
     * @return string the redis password 
     */
    inline string GetRedisPassword() {
        return redisPassword_;
    }

    /**
     * @brief Get the Backup End Type object
     * 
     * @return string the backup type
     */
    inline string GetBackupEndType() {
        return backupEndType_;
    }

    /**
     * @brief Get the Cache Flag object
     * 
     * @return true enable restore cache
     * @return false disable restore cache
     */
    inline bool GetCacheFlag() {
        return useCacheFlag_;
    }

    /**
     * @brief Get the Thread Num object
     * 
     * @return uint32_t the number of threads 
     */
    inline uint32_t GetThreadNum() {
        return threadNum_;
    }

    inline string GetStorageServerIP() {
        return storageServerIp_;
    }
    
    inline int GetStoragePort() {
        return storageServerPort_;
    }

    inline int GetClientID() {
        return clientID_;
    }
};

#endif //BASICDEDUP_CONFIGURE_h