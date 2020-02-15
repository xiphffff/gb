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

#include <QtCore>

class Emulator;
class MainWindow;
class MessageLogger;

class GBTest : public QObject
{
	Q_OBJECT

public:
	GBTest() noexcept;
	~GBTest() noexcept;

private:
	// Called when the user triggers "Emulation -> Start" on the main window.
	void start_emulation() noexcept;

	// Called when the user triggers "Emulation -> Stop" on the main window.
	void stop_emulation() noexcept;

	// Called when the user triggers "Emulation -> Pause" on the main window.
	void pause_emulation() noexcept;

	// Called when the user triggers "Emulation -> Restart" on the main window.
	void restart_emulation() noexcept;

	// Called when the user triggers "Debug -> Display libgb log" on the main
	// window.
	void display_libgb_log() noexcept;

#ifdef LIBGB_DEBUG
	// Called when the emulator reports an unknown memory read has been trapped
	void debug_unknown_memory_read(const uint16_t address) noexcept;

	// Called when the emulator reports that an illegal instruction has been trapped
	void debug_illegal_instruction() noexcept;
#endif // LIBGB_DEBUG

	MessageLogger* message_logger;

	MainWindow* main_window;
	Emulator* emulator;

	uint8_t* cart_data;
};