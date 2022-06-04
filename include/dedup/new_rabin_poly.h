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
#define POL_SHIFT_ (POLYNOMIAL_DEGREE_-8)

typedef struct {
    uint32_t w_pos;
    uint8_t* window;
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

        uint64_t inline append_byte(uint64_t hash, uint8_t b, uint64_t pol) {
            hash <<= 8;
            hash |= (uint64_t)b;
            return mod(hash, pol);
        }

        uint64_t inline mod(uint64_t x, uint64_t p) {
            while (deg(x) >= deg(p)) {
                unsigned int shift = deg(x) - deg(p);
                x = x ^ (p << shift);
            }
            return x;
        }

        int inline deg(uint64_t p) {
            uint64_t mask = 0x8000000000000000LL;

            for (int i = 0; i < 64; i++) {
                if ((mask & p) > 0) {
                    return 63 - i;
                }
                mask >>= 1;
            }
            return -1;
        }

        void inline rabin_append(NewRabin_t& ctx, uint8_t b) {
            uint8_t index = (uint8_t)(ctx.fp >> POL_SHIFT_);
            ctx.fp <<= 8;
            ctx.fp |= (uint64_t)b;
            ctx.fp ^= mod_table_[index];
        }

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
        void NewCtx(NewRabin_t& ctx);

        /**
         * @brief reset the rabin fingerprint ctx
         * 
         * @param ctx the input ctx <ret>
         */
        void ResetCtx(NewRabin_t& ctx);

        /**
         * @brief slide one byte in current ctx
         * 
         * @param ctx current ctx
         * @param input_byte the input byte
         * 
         * @return the new value of the Rabin fingerprint
         */
        uint64_t SlideOneByte(NewRabin_t& ctx, uint8_t input_byte);

        /**
         * @brief free the rabin fingerprint ctx
         * 
         * @param ctx the input ctx
         */
        void FreeCtx(NewRabin_t& ctx);
};

#endif