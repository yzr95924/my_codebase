/**
 * @file fastChunkingTest.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface defined in the FastCDC.h 
 * @version 0.1
 * @date 2020-09-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */


#include "../../include/fastChunkingTest.h"

struct timeval sFastCDCTime;
struct timeval eFastCDCTime;
/**
 * @brief Construct a new Fast C D C object
 * 
 * @param filePath 
 * @param minSize 
 * @param maxSize 
 * @param avgSize 
 */
FastCDC::FastCDC(string filePath, int minSize, int maxSize, int avgSize) {
    this->minChunkSize_ = tool::CompareLimit(minSize, MINIMUM_MIN, MINIMUM_MAX);
    this->avgChunkSize_ = tool::CompareLimit(avgSize, AVERAGE_MIN, AVERAGE_MAX);
    this->maxChunkSize_ = tool::CompareLimit(maxSize, MAXIMUM_MIN, MAXIMUM_MAX);
    this->pos_ = 0;
    this->normalSize_ = CalNormalSize(minSize, avgSize, maxSize);

    uint32_t bits = (uint32_t) round(log2(static_cast<double>(avgSize))); 
    this->maskS_ = GenerateFastCDCMask(bits + 1);
    this->maskL_ = GenerateFastCDCMask(bits - 1);

    fprintf(stderr, "---------FastCDC Configuration-----------\n");
    fprintf(stderr, "minChunkSize_: %u\n", this->minChunkSize_);
    fprintf(stderr, "avgChunkSize_: %u\n", this->avgChunkSize_);
    fprintf(stderr, "maxChunkSize_: %u\n", this->maxChunkSize_);
    fprintf(stderr, "normalSize_: %u\n", this->normalSize_);
    fprintf(stderr, "Mask-S: %u\n", this->maskS_);
    fprintf(stderr, "Mask-L: %u\n", this->maskL_);
    fprintf(stderr, "-----------------------------------------\n");

    readSize_ = 128 * 1024 * 1024; // 128MB
    waitingForChunkingBuffer_ = new uint8_t[readSize_];
    this->LoadChunkFile(filePath);
}

/**
 * @brief generate the mask according to the given bits
 * 
 * @param bits the number of '1' + 1
 * @return uint32_t the returned mask
 */
uint32_t FastCDC::GenerateFastCDCMask(uint32_t bits) {
    uint32_t tmp;
    tmp = (1 << tool::CompareLimit(bits, 1, 31)) - 1;
    return tmp;
}

/**
 * @brief compute the normal size 
 * 
 * @param min 
 * @param av 
 * @param max 
 * @return uint32_t 
 */
uint32_t FastCDC::CalNormalSize(const uint32_t min, const uint32_t av,
    const uint32_t max) {
    uint32_t off = min + tool::DivCeil(min, 2);
    if (off > av) {
        off = av;
    } 
    uint32_t diff = av - off;
    if (diff > max) {
        return max;
    }
    return diff;
}

/**
 * @brief Destroy the Fast C D C:: Fast C D C object
 * 
 */
FastCDC::~FastCDC() {
    fprintf(stderr, "FastCDC: Destory the object.\n");

    if (waitingForChunkingBuffer_) {
        delete[] waitingForChunkingBuffer_;
    } 
}

/**
 * @brief open the chunking file according to the path 
 * 
 * @param path the path of the chunking file  
 */
void FastCDC::LoadChunkFile(std::string path) {
    if (chunkingFile_.is_open()) {
        chunkingFile_.close();
    }
    chunkingFile_.open(path, std::ios::binary | std::ios::in);

    if (!chunkingFile_.is_open()) {
        fprintf(stderr, "FastCDC: open file %s error, exit now.\n", path.c_str());
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief run the chunking process  
 * 
 */
void FastCDC::Chunking() {
    ifstream& fin = GetChunkingFile();
    uint64_t fileSize = 0;
    uint64_t chunkIDCnt = 0;
    double totalTime = 0;

#if CHUNKING_BREAKDOWN == 1
    gettimeofday(&sFastCDCTime, NULL);
#endif
    size_t totalOffset = 0;
    memset((char*)waitingForChunkingBuffer_, 0, sizeof(uint8_t) * readSize_);
    bool end = false;
    while (!end) {
        fin.read((char*)waitingForChunkingBuffer_, sizeof(uint8_t) * readSize_);
        end = fin.eof(); 
        size_t len = fin.gcount();
        size_t localOffset = 0;
        while (((len - localOffset) >= maxChunkSize_) || (end && (localOffset < len))) {
            uint32_t cp = CutPoint(waitingForChunkingBuffer_ + localOffset, len - localOffset);
            Data_t tempChunk;
            tempChunk.chunk.chunkSize = cp;
            memcpy(tempChunk.chunk.logicData, waitingForChunkingBuffer_ + localOffset, cp);
            tempChunk.dataType = DATA_CHUNK;
            localOffset += cp;
            fileSize += cp;
            chunkIDCnt++;
        }
        pos_ += localOffset;
        totalOffset += localOffset;
    
        fin.seekg(totalOffset, std::ios_base::beg);
    }
    fileRecipe_.recipeEnd.totalChunkNumber = chunkIDCnt;
    fileRecipe_.recipeEnd.fileSize = fileSize;
    fileRecipe_.dataType = RECIPE_END;

#if CHUNKING_BREAKDOWN == 1
    gettimeofday(&eFastCDCTime, NULL);
    totalTime = tool::GetTimeDiff(sFastCDCTime, eFastCDCTime);
    fprintf(stderr, "FastCDC: total chunking time = %.4lf\n", totalTime);
    fprintf(stderr, "FastCDC: total offset = %lu\n", totalOffset);
    fprintf(stderr, "Total chunk number: %lu\n", chunkIDCnt);
    fprintf(stderr, "Chunking file size (MB): %.4lf\n", (fileSize / 1024.0 / 1024.0));
    fprintf(stderr, "Throughput (MB/s): %.4lf\n", (fileSize / 1024.0 / 1024.0) / totalTime);
#endif

    fin.close();
}

/**
 * @brief Get the Chunking File object
 * 
 * @return std::ifstream& the reference to the chunking file  
 */
std::ifstream& FastCDC::GetChunkingFile() {
    if (!chunkingFile_.is_open()) {
        fprintf(stderr, "FastCDC: chunking file open failed.\n");
        exit(EXIT_FAILURE);
    }
    return chunkingFile_;
}

/**
 * @brief To get the offset of chunks for a given buffer  
 * 
 * @param src the input buffer  
 * @param len the length of this buffer
 * @return uint32_t length of this chunk.
 */
uint32_t FastCDC::CutPoint(const uint8_t* src, const uint32_t len) {
    uint32_t n;
    uint32_t fp = 0;
    uint32_t i;
    i = std::min(len, static_cast<uint32_t>(minChunkSize_)); 
    n = std::min(normalSize_, len);
    for (; i < n; i++) {
        fp = (fp >> 1) + GEAR[src[i]];
        if (!(fp & maskS_)) {
            return (i + 1);
        }
    }

    n = std::min(static_cast<uint32_t>(maxChunkSize_), len);
    for (; i < n; i++) {
        fp = (fp >> 1) + GEAR[src[i]];
        if (!(fp & maskL_)) {
            return (i + 1);
        }
    } 
    return i;
}