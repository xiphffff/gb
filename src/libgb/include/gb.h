// Copyright 2020 Michael Rodriguez
//
// Permission to use, copy, modify, and /or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
// OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
// CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "bus.h"
#include "cpu.h"

struct libgb_system
{
	struct libgb_bus* bus;
	struct libgb_cpu* cpu;
};

// Creates a Game Boy (DMG) system.
struct libgb_system* libgb_system_create(uint8_t* const rom_data);

// Destroys a Game Boy (DMG) system.
void libgb_system_destroy(struct libgb_system* gb);

// Resets a Game Boy (DMG) system to the startup state.
void libgb_system_reset(struct libgb_system* gb);

// Executes one full system step and returns the number of T-cycles taken.
unsigned int libgb_system_step(struct libgb_system* gb);

#ifdef __cplusplus
}
#endif // __cplusplus