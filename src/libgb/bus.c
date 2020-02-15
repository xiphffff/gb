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

#include <assert.h>
#include "bus.h"
#include "utility/memory.h"

// Pointer to the cartridge data. `libgb_bus` does not need to know about this;
// the caller knows about the data already.
static uint8_t* cart;

// Function to call when a read between addresses $4000 - $7FFF has taken
// place.
static uint8_t (*cart_banked_read)(const uint16_t address);

// Called when a cartridge read between addresses $4000 - $7FFF has taken
// place without a memory bank controller (MBC).
static uint8_t no_mbc_banked_read(const uint16_t address)
{
	return cart[address];
}

// Creates a Game Boy (DMG) system bus. This is really just a fancy way of
// saying "interconnect" because the system bus in reality is complex and
// multifaceted, apparently.
struct libgb_bus* libgb_bus_create(uint8_t* const rom_data)
{
	struct libgb_bus* bus = libgb_safe_malloc(sizeof(struct libgb_bus));

	switch (rom_data[0x0147])
	{
		// ROM ONLY
		case 0x00:
			cart_banked_read = &no_mbc_banked_read;
			break;

		default:
			return NULL;
	}
#ifdef LIBGB_DEBUG
	bus->user_data = NULL;
	bus->debug_unknown_memory_read = NULL;
#endif // LIBGB_DEBUG
	cart = rom_data;
	return bus;
}

// Destroys a Game Boy (DMG) system bus.
void libgb_bus_destroy(struct libgb_bus* bus)
{
	libgb_safe_free(bus);
}

// Resets the Game Boy (DMG) system bus to the startup state.
void libgb_bus_reset(struct libgb_bus* bus)
{
	assert(bus != NULL);

	bus->cycles_taken = 0;
}

// Steps the hardware by 4 T-cycles (1 m-cycle).
void libgb_bus_step(struct libgb_bus* bus)
{
	assert(bus != NULL);

	bus->cycles_taken += 4;
}

// Returns data from memory referenced by address `address`.
uint8_t libgb_bus_read_memory(struct libgb_bus* bus, const uint16_t address)
{
	assert(bus != NULL);

	// Each memory access is 1 m-cycle (or 4 T-cycles)
	libgb_bus_step(bus);

	switch ((address & 0xF000) >> 12)
	{
		case 0x0 ... 0x3:
			return cart[address];

		case 0x4 ... 0x7:
			return cart_banked_read(address);

		default:
#ifdef LIBGB_DEBUG
			if (bus->debug_unknown_memory_read)
			{
				bus->debug_unknown_memory_read(bus->user_data, address);
			}
#endif // LIBGB_DEBUG
			return 0x00;
	}
}