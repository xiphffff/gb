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

#include <QApplication>
#include "emulator.h"

Emulator::Emulator(QObject* parent, uint8_t* const rom_data) : QObject(parent)
{
	gb_emu = libgb_system_create(rom_data);

#ifdef LIBGB_DEBUG
	gb_emu->bus->user_data = this;

	gb_emu->bus->debug_unknown_memory_read =
	[](void* user_data, const uint16_t address)
	{
		Emulator* emu = reinterpret_cast<Emulator*>(user_data);
		emit emu->debug_unknown_memory_read(address);
	};

	gb_emu->cpu->user_data = this;

	gb_emu->cpu->debug_illegal_instruction = [](void* user_data)
	{
		Emulator* emu = reinterpret_cast<Emulator*>(user_data);
		emit emu->debug_illegal_instruction();
	};
#endif // LIBGB_DEBUG

	running = false;
}

Emulator::~Emulator()
{
	libgb_system_destroy(gb_emu);
}

void Emulator::start() noexcept
{
	if (running)
	{
		return;
	}

	running = true;

	while (running)
	{
		const auto max_cycles{ 4194304 / 60 };
		auto cycles{ 0 };

		QElapsedTimer timer;
		timer.start();

		while (cycles < max_cycles)
		{
			if (!running)
			{
				break;
			}
			cycles += libgb_system_step(gb_emu);
		}
		QApplication::processEvents();

		const auto elapsed{ timer.elapsed() };

		if (elapsed < (1000 / 60))
		{
			QThread::msleep((1000 / 60) - elapsed);
		}
	}
}

void Emulator::stop() noexcept
{
	if (running)
	{
		running = false;
	}
}

void Emulator::pause() noexcept
{ }

void Emulator::restart() noexcept
{ }