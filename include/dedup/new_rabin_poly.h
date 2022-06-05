/**
 * @file new_rabin_poly.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement a new rabin poly (https://github.com/fd0/rabin-cdc)
 * @version 0.1
 * @date 2022-06-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_CODEBASE_NEW_RABIN_POLY
#define MY_CODEBASE_NEW_RABIN_POLY

#include "../define.h"

using namespace std;

#define POLYNOMIAL_ 0x3DA3358B4DC173LL
#define POLYNOMIAL_DEGREE_ 53

typedef struct {
    uint32_t w_pos;
    uint8_t window[128];
    uint64_t fp;
} NewRabin_t;

class NewRabinUtil {
    private:
        string my_name_ = "NewRabinUtil";
        
        uint64_t mod_table_[256];
        uint64_t out_table_[256];
        uint64_t window_size_;

        // uint64_t POLYNOMIAL_;
        // uint64_t POLYNOMIAL_DEGREE_;
        // uint64_t POL_SHIFT_;

        void rabin_append(NewRabin_t* ctx, uint8_t b);

    public:
        /**
         * @brief Construct a new Rabin FP Util object
         * 
         * @param window_size the sliding window size
         */
        NewRabinUtil(uint64_t window_size);

        /**
         * @brief Destroy the Rabin FP Util object
         * 
         */
        ~NewRabinUtil();

        /**
         * @brief generate a new rabin fingerprint ctx
         * 
         * @param ctx the input ctx <ret>
         */
        void NewCtx(NewRabin_t* ctx);

        /**
         * @brief reset the rabin fingerprint ctx
         * 
         * @param ctx the input ctx <ret>
         */
        void ResetCtx(NewRabin_t* ctx);

        /**
         * @brief slide one byte in current ctx
         * 
         * @param ctx current ctx
         * @param input_byte the input byte
         * 
         * @return the new value of the Rabin fingerprint
         */
        inline void SlideOneByte(NewRabin_t* ctx, uint8_t input_byte) {
            uint8_t out = ctx->window[ctx->w_pos];
            ctx->window[ctx->w_pos] = input_byte;
            ctx->fp = (ctx->fp ^ out_table_[out]);
            ctx->w_pos = (ctx->w_pos + 1) % window_size_;
            rabin_append(ctx, input_byte);
        }

        /**
         * @brief free the rabin fingerprint ctx
         * 
         * @param ctx the input ctx
         */
        void FreeCtx(NewRabin_t& ctx);
};

#endif