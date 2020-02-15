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

#include <stdint.h>

struct libgb_bus
{
	// Number of T-cycles taken by the current step
	unsigned int cycles_taken;

#ifdef LIBGB_DEBUG
	void* user_data;
	void (*debug_unknown_memory_read)(void* user_data, const uint16_t address);
#endif // LIBGB_DEBUG
};

// Creates a Game Boy (DMG) system bus. This is really just a fancy way of
// saying "interconnect" because the system bus in reality is complex and
// multifaceted, apparently.
struct libgb_bus* libgb_bus_create(uint8_t* const rom_data);

// Destroys a Game Boy (DMG) system bus.
void libgb_bus_destroy(struct libgb_bus* bus);

// Resets the Game Boy (DMG) system bus to the startup state.
void libgb_bus_reset(struct libgb_bus* bus);

// Steps the hardware by 1 T-cycle (4 M-cycles).
void libgb_bus_step(struct libgb_bus* bus);

// Returns data from memory referenced by address `address`.
uint8_t libgb_bus_read_memory(struct libgb_bus* bus, const uint16_t address);

#ifdef __cplusplus
}
#endif // __cplusplus