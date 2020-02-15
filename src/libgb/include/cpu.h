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

struct libgb_bus;

struct libgb_cpu
{
	struct
	{
		uint16_t pc;
	} reg;

	uint8_t instruction;

#ifdef LIBGB_DEBUG
	void* user_data;
	void (*debug_illegal_instruction)(void* user_data);
#endif // LIBGB_DEBUG
};

// Creates a Sharp SM83 CPU interpreter.
struct libgb_cpu* libgb_cpu_create(struct libgb_bus* b);

// Destroys a Sharp SM83 CPU interpreter.
void libgb_cpu_destroy(struct libgb_cpu* cpu);

// Resets a Sharp SM83 CPU interpreter to the startup state.
void libgb_cpu_reset(struct libgb_cpu* cpu);

// Executes one instruction.
void libgb_cpu_step(struct libgb_cpu* cpu);

#ifdef __cplusplus
}
#endif // __cplusplus