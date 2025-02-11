// This file is dual-licensed.  Choose whichever licence you want from
// the two licences listed below.
//
// The first licence is a regular 2-clause BSD licence.  The second licence
// is the CC-0 from Creative Commons. It is intended to release Monocypher
// to the public domain.  The BSD licence serves as a fallback option.
//
// SPDX-License-Identifier: BSD-2-Clause OR CC0-1.0
//
// ------------------------------------------------------------------------
//
// Copyright (c) 2017-2019, Loup Vaillant
// All rights reserved.
//
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ------------------------------------------------------------------------
//
// Written in 2017-2019 by Loup Vaillant
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related neighboring rights to this software to the public domain
// worldwide.  This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software.  If not, see
// <https://creativecommons.org/publicdomain/zero/1.0/>

#include "speed.h"
#include "utils.h"
#include "tweetnacl.h"

// TweetNaCl needs to link with this
// Not really random, but we don't care for those benchmarks.
void randombytes(u8 *stream, u64 size)
{
    p_random(stream, (size_t)size);
}

static u64 salsa20(void)
{
    u8 out[SIZE];
    RANDOM_INPUT(in   , SIZE);
    RANDOM_INPUT(key  ,   32);
    RANDOM_INPUT(nonce,    8);

    TIMING_START {
        crypto_stream_salsa20_xor(out, in, SIZE, nonce, key);
    }
    TIMING_END;
}

static u64 poly1305(void)
{
    u8 out[16];
    RANDOM_INPUT(in , SIZE);
    RANDOM_INPUT(key,   32);

    TIMING_START {
        crypto_onetimeauth(out, in, SIZE, key);
    }
    TIMING_END;
}

static u64 authenticated(void)
{
    u8 out[SIZE + 32];
    RANDOM_INPUT(in   , SIZE + 32);
    RANDOM_INPUT(key  ,        32);
    RANDOM_INPUT(nonce,        24);

    TIMING_START {
        crypto_secretbox(out, in, SIZE + 32, nonce, key);
    }
    TIMING_END;
}

static u64 sha512(void)
{
    u8 hash[64];
    RANDOM_INPUT(in, SIZE);

    TIMING_START {
        crypto_hash(hash, in, SIZE);
    }
    TIMING_END;
}

static u64 x25519(void)
{
    u8 in [32] = {9};
    u8 out[32] = {9};

    TIMING_START {
        crypto_scalarmult(out, out, in);
    }
    TIMING_END;
}

static u64 edDSA_sign(void)
{
    u8 sk        [ 64];
    u8 pk        [ 32];
    u8 signed_msg[128];
    unsigned long long sig_size;
    RANDOM_INPUT(message, 64);
    crypto_sign_keypair(pk, sk);

    TIMING_START {
        crypto_sign(signed_msg, &sig_size, message, 64, sk);
    }
    TIMING_END;
}

static u64 edDSA_check(void)
{
    u8 sk        [ 64];
    u8 pk        [ 32];
    u8 signed_msg[128];
    u8 out_msg   [128];
    unsigned long long sig_size;
    unsigned long long msg_size;
    RANDOM_INPUT(message, 64);
    crypto_sign_keypair(pk, sk);
    crypto_sign(signed_msg, &sig_size, message, 64, sk);

    TIMING_START {
        if (crypto_sign_open(out_msg, &msg_size, signed_msg, sig_size, pk)) {
            printf("TweetNaCl verification failed\n");
        }
    }
    TIMING_END;
}

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;
    print("Salsa20          ",salsa20()      *MUL,"megabytes  per second");
    print("Poly1305         ",poly1305()     *MUL,"megabytes  per second");
    print("Auth'd encryption",authenticated()*MUL,"megabytes  per second");
    print("SHA-512          ",sha512()       *MUL,"megabytes  per second");
    print("x25519           ",x25519()           ,"exchanges  per second");
    print("EdDSA(sign)      ",edDSA_sign()       ,"signatures per second");
    print("EdDSA(check)     ",edDSA_check()      ,"checks     per second");
    printf("\n");
    return 0;
}
