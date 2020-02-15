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
#include "cpu.h"
#include "bus.h"
#include "utility/memory.h"

// `libgb_cpu` does not need to know about this.
static struct libgb_bus* bus;

// Creates a Sharp SM83 CPU interpreter.
struct libgb_cpu* libgb_cpu_create(struct libgb_bus* b)
{
	struct libgb_cpu* cpu = libgb_safe_malloc(sizeof(struct libgb_cpu));

#ifdef LIBGB_DEBUG
	cpu->debug_illegal_instruction = NULL;
#endif // LIBGB_DEBUG

	bus = b;
	return cpu;
}

// Destroys a Sharp SM83 CPU interpreter.
void libgb_cpu_destroy(struct libgb_cpu* cpu)
{
	libgb_safe_free(cpu);
}

// Resets a Sharp SM83 CPU interpreter to the startup state.
void libgb_cpu_reset(struct libgb_cpu* cpu)
{
	assert(cpu != NULL);

	cpu->reg.pc = 0x0100;
}

// Execute one instruction.
void libgb_cpu_step(struct libgb_cpu* cpu)
{
	assert(cpu != NULL);

	cpu->instruction = libgb_bus_read_memory(bus, cpu->reg.pc);

	switch (cpu->instruction)
	{
		default:
#ifdef LIBGB_DEBUG
			if (cpu->debug_illegal_instruction)
			{
				cpu->debug_illegal_instruction(cpu->user_data);
			}
#endif
			break;
	}
}