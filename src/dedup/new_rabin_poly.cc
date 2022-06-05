/**
 * @file new_rabin_poly.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface of NewRabinUtil
 * @version 0.1
 * @date 2022-06-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/dedup/new_rabin_poly.h"

#define POL_SHIFT_ (POLYNOMIAL_DEGREE_-8)

static int deg(uint64_t p) {
    uint64_t mask = 0x8000000000000000LL;

    for (int i = 0; i < 64; i++) {
        if ((mask & p) > 0) {
            return 63 - i;
        }

        mask >>= 1;
    }

    return -1;
}

// Mod calculates the remainder of x divided by p.
static uint64_t mod(uint64_t x, uint64_t p) {
    while (deg(x) >= deg(p)) {
        unsigned int shift = deg(x) - deg(p);

        x = x ^ (p << shift);
    }

    return x;
}

static uint64_t append_byte(uint64_t hash, uint8_t b, uint64_t pol) {
    hash <<= 8;
    hash |= (uint64_t)b;

    return mod(hash, pol);
}

/**
 * @brief Construct a new Rabin FP Util object
 * 
 * @param window_size the sliding window size
 */
NewRabinUtil::NewRabinUtil(uint64_t window_size) {
    window_size_ = 128;
    // POLYNOMIAL_ = 0x3DA3358B4DC173LL;
    // POLYNOMIAL_DEGREE_ = 53;
    // POL_SHIFT_ = (POLYNOMIAL_DEGREE_ - 8);

    // calculate table for sliding out bytes. The byte to slide out is used as
    // the index for the table, the value contains the following:
    // out_table[b] = Hash(b || 0 ||        ...        || 0)
    //                          \ windowsize-1 zero bytes /
    // To slide out byte b_0 for window size w with known hash
    // H := H(b_0 || ... || b_w), it is sufficient to add out_table[b_0]:
    //    H(b_0 || ... || b_w) + H(b_0 || 0 || ... || 0)
    //  = H(b_0 + b_0 || b_1 + 0 || ... || b_w + 0)
    //  = H(    0     || b_1 || ...     || b_w)
    //
    // Afterwards a new byte can be shifted in.
    for (int b = 0; b < 256; b++) {
        uint64_t hash = 0;

        hash = append_byte(hash, (uint8_t)b, POLYNOMIAL_);
        for (int i = 0; i < window_size_ -1; i++) {
            hash = append_byte(hash, 0, POLYNOMIAL_);
        }
        out_table_[b] = hash;
    }

    // calculate table for reduction mod Polynomial
    int k = deg(POLYNOMIAL_);
    for (int b = 0; b < 256; b++) {
        // mod_table[b] = A | B, where A = (b(x) * x^k mod pol) and  B = b(x) * x^k
        //
        // The 8 bits above deg(Polynomial) determine what happens next and so
        // these bits are used as a lookup to this table. The value is split in
        // two parts: Part A contains the result of the modulus operation, part
        // B is used to cancel out the 8 top bits so that one XOR operation is
        // enough to reduce modulo Polynomial
        mod_table_[b] = mod(((uint64_t)b) << k, POLYNOMIAL_) | ((uint64_t)b) << k;
    }

    // for (int i = 0; i < 256; i++) {
    //     cout << out_table_[i] << ", " << mod_table_[i] << endl;
    // }
}

/**
 * @brief Destroy the Rabin FP Util object
 * 
 */
NewRabinUtil::~NewRabinUtil() {
    ;
}

/**
 * @brief slide one byte in current ctx
 * 
 * @param ctx current ctx
 * @param input_byte the input byte
 * 
 * @return the new value of the Rabin fingerprint
 */
// void NewRabinUtil::SlideOneByte(NewRabin_t* ctx, uint8_t input_byte) {
//     uint8_t out = ctx->window[ctx->w_pos];
//     ctx->window[ctx->w_pos] = input_byte;
//     ctx->fp = (ctx->fp ^ out_table_[out]);
//     ctx->w_pos = (ctx->w_pos + 1) % window_size_;
//     rabin_append(ctx, input_byte);
// }

/**
 * @brief reset the rabin fingerprint ctx
 * 
 * @param ctx the input ctx <ret>
 */
void NewRabinUtil::ResetCtx(NewRabin_t* ctx) {
    for (size_t i = 0; i < window_size_; i++) {
        ctx->window[i] = 0;
    }
    ctx->w_pos = 0;
    ctx->fp = 0;
    SlideOneByte(ctx, 1);
    return ; 
}

/**
 * @brief generate a new rabin fingerprint ctx
 * 
 * @param ctx the input ctx <ret>
 */
void NewRabinUtil::NewCtx(NewRabin_t* ctx) {
    // ctx->window = (uint8_t*) malloc(window_size_ * sizeof(uint8_t));
    memset(ctx->window, 0, window_size_);
    ctx->w_pos = 0;
    ctx->fp = 0;
    SlideOneByte(ctx, 1);
    return ;
}

/**
 * @brief free the rabin fingerprint ctx
 * 
 * @param ctx the input ctx
 */
void NewRabinUtil::FreeCtx(NewRabin_t& ctx) {
    // free(ctx.window);
    ctx.w_pos = 0;
    ctx.fp = 0;
    return ;
}

void NewRabinUtil::rabin_append(NewRabin_t* ctx, uint8_t b) {
    uint8_t index = (uint8_t)(ctx->fp >> POL_SHIFT_);
    ctx->fp <<= 8;
    ctx->fp |= (uint64_t)b;
    ctx->fp ^= mod_table_[index];
    return ;
}