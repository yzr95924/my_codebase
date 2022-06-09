/**
 * @file finesse_util.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief 
 * @version 0.1
 * @date 2022-06-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/dedup/finesse_util.h"

/**
 * @brief Construct a new FinesseUtil object
 * 
 * @param super_feature_per_chunk super feature per chunk 
 * @param feature_per_chunk feature per chunk 
 * @param feature_per_super_feature feature per super feature
 */
FinesseUtil::FinesseUtil(uint64_t super_feature_per_chunk, uint64_t feature_per_chunk,
    uint64_t feature_per_super_feature) {
    super_feature_per_chunk_ = super_feature_per_chunk;
    feature_per_chunk_ = feature_per_chunk;
    feature_per_super_feature_ = feature_per_super_feature;

    rabin_util_ = new RabinFPUtil(config.GetSimilarSlidingWinSize());
}

/**
 * @brief Destroy the FinesseUtil object
 * 
 */
FinesseUtil::~FinesseUtil() {
    delete rabin_util_;
}

/**
 * @brief compute the features from the chunk
 *
 * @param ctx the rabin ctx
 * @param data the chunk data
 * @param size the chunk size
 * @param features the chunk features
 */
void FinesseUtil::ExtractFeature(RabinCtx_t& ctx, uint8_t* data, uint32_t size,
    uint64_t* features) {
    rabin_util_->ResetCtx(ctx);
    uint64_t sub_chunk_size;
    if ((size % feature_per_chunk_) != 0) {
        sub_chunk_size = size / feature_per_chunk_ + 1;
    } else {
        sub_chunk_size = size / feature_per_chunk_;
    }

    uint64_t tmp_rabin_fp = 0;
    uint64_t max_rabin_fp = 0;
    vector<vector<uint64_t>> group_features;
    group_features.resize(feature_per_chunk_);
    uint32_t cur_offset = 0;
    uint32_t cur_group_id = 0;

    for (uint32_t i = 0; i < feature_per_chunk_; i++) {
        uint8_t* sub_chunk_start = data + i * sub_chunk_size;
        max_rabin_fp = 0;
        for (uint32_t j = 0; j < sub_chunk_size; j++) {
            uint8_t* cur_data = sub_chunk_start + j;
            tmp_rabin_fp = rabin_util_->SlideOneByte(ctx, *cur_data);
            if (tmp_rabin_fp > max_rabin_fp) {
                max_rabin_fp = tmp_rabin_fp;
            }
            cur_offset++;

            if (cur_offset == size) {
                break;
            }
        }
        group_features[cur_group_id].push_back(max_rabin_fp);
        if (group_features[cur_group_id].size() == super_feature_per_chunk_) {
            cur_group_id++;
        }
        rabin_util_->ResetCtx(ctx);
    }
    rabin_util_->ResetCtx(ctx);

    // sort the features of each group
    for (uint32_t i = 0; i < feature_per_super_feature_; i++) {
        // max->min
        sort(group_features[i].begin(), group_features[i].end(),
            greater<uint64_t>());
    }

    // group features into super features
    vector<vector<uint64_t>> super_features;
    super_features.resize(super_feature_per_chunk_);
    for (uint32_t i = 0; i < super_feature_per_chunk_; i++) {
        for (uint32_t j = 0; j < feature_per_super_feature_; j++) {
            super_features[i].push_back(group_features[j][i]);
        }
    }

    // compute the hash of the super features
    for (uint32_t i = 0; i < super_feature_per_chunk_; i++) {
        features[i] = XXHash64::hash((uint8_t*)super_features[i].data(),
            sizeof(uint64_t) * feature_per_super_feature_, 0);
    }

    return ;
}