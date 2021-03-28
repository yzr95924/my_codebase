/**
 * @file configure.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement interfaces defined in configure 
 * @version 0.1
 * @date 2019-12-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "../../include/configure.h"

/**
 * @brief Destroy the Configure:: Configure object
 * 
 */
Configure::~Configure() {
    fprintf(stderr, "Destory the configure module.\n");
}

/**
 * @brief Construct a new Configure:: Configure object
 * 
 * @param path the configure file path
 */
Configure::Configure(std::string path) {
    this->ReadConf(path);
}

/**
 * @brief read the configure file
 * 
 * @param path the configure file name
 */
void Configure::ReadConf(std::string path) {
    using namespace boost;
    using namespace boost::property_tree;
    ptree root;
    read_json<ptree>(path, root);
    // Chunker configure
    chunkingType_ = root.get<uint64_t>("ChunkerConfig.chunkingType_");
    maxChunkSize_ = root.get<uint64_t>("ChunkerConfig.maxChunkSize_");
    minChunkSize_ = root.get<uint64_t>("ChunkerConfig.minChunkSize_");
    avgChunkSize_ = root.get<uint64_t>("ChunkerConfig.avgChunkSize_");
    slidingWinSize_ = root.get<uint64_t>("ChunkerConfig.slidingWinSize_");
    segmentSize_ = root.get<uint64_t>("ChunkerConfig.segmentSize_");
    readSize_ = root.get<uint64_t>("ChunkerConfig.readSize_");

    // StorageCore configure
    recipeRootPath_ = root.get<std::string>("StorageCore.recipeRootPath_");
    containerRootPath_ = root.get<std::string>("StorageCore.containerRootPath_");
    metaRootPath_ = root.get<std::string>("StorageCore.metaRootPath_");
    fp2ChunkDBName_ = root.get<std::string>("StorageCore.fp2ChunkDBName_");
    fp2MetaDBName_ = root.get<std::string>("StorageCore.fp2MetaDBName_");

    // restore writer
    restoreBatchSize_ = root.get<uint64_t>("RestoreWriter.restoreBatchSize_");
    readCacheSize_ = root.get<uint64_t>("RestoreWriter.readCacheSize_");

    // compression setting
    enableCompression_ = root.get<std::string>("Compressor.enableCompression_");
    if (enableCompression_.compare("true") == 0) {
        enableCompressionFlag_ = true;
    } else {
        enableCompressionFlag_ = false;
    }

    // restore container cache 
    useCache_ = root.get<std::string>("RestoreWriter.useCache_");
    if (useCache_.compare("true") == 0) {
        useCacheFlag_ = true;
    } else {
        useCacheFlag_ = false;
    }
    // container cache setting
    
    // for backup
    file2MetaDBName_ = root.get<std::string>("Backup.file2MetaDBName_");
    tarDir_ = root.get<std::string>("Backup.tarDir_");
    restoreTarDir_ = root.get<std::string>("Backup.restoreTarDir_");

    // for NCloud connection 
    redisIP_ = root.get<std::string>("NCloud.redisIP_");
    redisPort_ = root.get<uint16_t>("NCloud.redisPort_");
    redisReplyWaitTimeout_ = root.get<uint32_t>("NCloud.redisReplyWaitTimeout_");
    redisPassword_ = root.get<std::string>("NCloud.redisPassword_");

    // back-end type
    backupEndType_ = root.get<std::string>("DataWriter.type_");
    threadNum_ = root.get<uint32_t>("DataWriter.threadNum_");

    // for storage server 
    storageServerIp_ = root.get<std::string>("DataSender.storageServerIp_");
    storageServerPort_ = root.get<int>("DataSender.storageServerPort_");

    // for client id
    clientID_ = root.get<int>("DataSender.clientID_");

    if (SHOW_CONFIGURE) {
        fprintf(stderr, "=================DataWriter Configure===============\n");
        fprintf(stderr, "Backup end type: %s\n", backupEndType_.c_str());
        fprintf(stderr, "Datawriter thread num: %u\n", threadNum_);
        fprintf(stderr, "=================Chunker Configure==================\n");
        fprintf(stderr, "Max chunk size: %lu\n", maxChunkSize_);
        fprintf(stderr, "Min chunk size: %lu\n", minChunkSize_);
        fprintf(stderr, "Avg chunk size: %lu\n", avgChunkSize_);
        fprintf(stderr, "Sliding window size: %lu\n", slidingWinSize_);
        fprintf(stderr, "Segment size: %lu\n", segmentSize_);
        fprintf(stderr, "Read size: %lu\n", readSize_);
        fprintf(stderr, "=================StorageCore Configure==============\n");
        fprintf(stderr, "Recipe root path: %s\n", recipeRootPath_.c_str());
        fprintf(stderr, "Container root path: %s\n",containerRootPath_.c_str());
        fprintf(stderr, "Meta root path %s\n", metaRootPath_.c_str());
        fprintf(stderr, "fp2ChunkDBName_: %s\n", fp2ChunkDBName_.c_str());
        fprintf(stderr, "fp2MetaDBName_: %s\n", fp2MetaDBName_.c_str());
        fprintf(stderr, "================Restore Configure===================\n");
        fprintf(stderr, "restoreBatchSize_: %lu\n", restoreBatchSize_);
        fprintf(stderr, "read cache size: %lu\n", readCacheSize_);
        fprintf(stderr, "================Compression Configure===============\n");
        fprintf(stderr, "enableCompression_: %s\n", enableCompression_.c_str());
        fprintf(stderr, "================Backup Configure====================\n");
        fprintf(stderr, "file2MetaDBName_: %s\n", file2MetaDBName_.c_str());
        fprintf(stderr, "tarDir_: %s\n", tarDir_.c_str());
        fprintf(stderr, "restoreTarDir_: %s\n", restoreTarDir_.c_str());
        fprintf(stderr, "================nCloud connection===================\n");
        fprintf(stderr, "redis ip: %s\n", redisIP_.c_str());
        fprintf(stderr, "redis port: %hu\n", redisPort_);
        fprintf(stderr, "redis timeout: %u\n", redisReplyWaitTimeout_);
        fprintf(stderr, "redis password: %s\n", redisPassword_.c_str());
        fprintf(stderr, "===================Storage Server===================\n");
        fprintf(stderr, "storage server ip: %s\n", storageServerIp_.c_str());
        fprintf(stderr, "storage server port: %d\n", storageServerPort_);
        fprintf(stderr, "client ID: %d\n", clientID_);
        fprintf(stderr, "====================================================\n");
    }
}