// Copyright 2020 Michael Rodriguez
//
// Permission to use, copy, modify, and /or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright noticeand this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
// OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
// CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <assert.h>
#include "gb.h"
#include "utility/memory.h"

// Creates a Game Boy (DMG) system.
struct libgb_system* libgb_system_create(uint8_t* const rom_data)
{
	struct libgb_system* gb = libgb_safe_malloc(sizeof(struct libgb_system));

	gb->bus = libgb_bus_create(rom_data);
	gb->cpu = libgb_cpu_create(gb->bus);

	libgb_system_reset(gb);
	return gb;
}

// Destroys a Game Boy (DMG) system.
void libgb_system_destroy(struct libgb_system* gb)
{
	assert(gb != NULL);

	libgb_cpu_destroy(gb->cpu);
	libgb_bus_destroy(gb->bus);
}

// Resets a Game Boy (DMG) system to the startup state.
void libgb_system_reset(struct libgb_system* gb)
{
	assert(gb != NULL);

	libgb_bus_reset(gb->bus);
	libgb_cpu_reset(gb->cpu);
}

// Executes one full system step and returns the number of T-cycles taken.
unsigned int libgb_system_step(struct libgb_system* gb)
{
	assert(gb != NULL);

	libgb_cpu_step(gb->cpu);

	const unsigned int cycles_taken = gb->bus->cycles_taken;
	gb->bus->cycles_taken = 0;

	return cycles_taken;
}