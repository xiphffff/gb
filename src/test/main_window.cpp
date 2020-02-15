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

#include "main_window.h"

MainWindow::MainWindow() noexcept
{
	emulation_menu = menuBar()->addMenu(tr("&Emulation"));

	start_emulation   = new QAction("&Start",   this);
	stop_emulation    = new QAction("&Stop",    this);
	pause_emulation   = new QAction("&Pause",   this);
	restart_emulation = new QAction("&Restart", this);

	emulation_menu->addAction(start_emulation);
	emulation_menu->addAction(stop_emulation);
	emulation_menu->addAction(pause_emulation);
	emulation_menu->addAction(restart_emulation);

	debug_menu = menuBar()->addMenu(tr("&Debug"));
	display_libgb_log = new QAction(tr("Display libgb log"), this);

	debug_menu->addAction(display_libgb_log);
}

MainWindow::~MainWindow() noexcept
{ }