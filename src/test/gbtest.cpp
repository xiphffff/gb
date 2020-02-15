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

#include <QtWidgets>
#include "emulator.h"
#include "gbtest.h"
#include "main_window.h"
#include "debug/message_logger.h"

GBTest::GBTest() noexcept : message_logger(nullptr)
{
    goto staging;

staging:
    QString file_name{ QFileDialog::getOpenFileName(nullptr,
                                                    tr("Open Game Boy ROM"),
                                                    "",
                                                 tr("Game Boy ROMs (*.gb)")) };

    if (file_name.isEmpty())
    {
        const auto result{ QMessageBox::warning(nullptr,
                                                tr("No ROM selected"),
                                                tr("libgb debugging station "
                                                   "requires that you select "
                                                   "a Game Boy ROM."),
                                                QMessageBox::Retry,
                                                QMessageBox::Cancel) };

        switch (result)
        {
            case QMessageBox::Retry:
                goto staging;

            case QMessageBox::Cancel:
                exit(EXIT_FAILURE);
        }
    }
    else
    {
        QFile m_rom_file(file_name);
        const auto file_size{ m_rom_file.size() };

        FILE* rom_file = fopen(qPrintable(file_name), "rb");
        cart_data = new uint8_t[file_size];
        fread(cart_data, 1, file_size, rom_file);
        fclose(rom_file);
#if 0

        if (!rom_file.open(QIODevice::ReadOnly))
        {
            __debugbreak();
        }

        const char* m_rom_data{ rom_file.readAll().constData() };
        const auto file_size{ rom_file.size() };

        cart_data = new uint8_t[file_size];

        memcpy(cart_data, m_rom_data, file_size);

        rom_file.close();
#endif // Qt sucks or maybe I do

        emulator = new Emulator(this, cart_data);

#ifdef LIBGB_DEBUG
        connect(emulator,
                &Emulator::debug_unknown_memory_read,
                this,
                &GBTest::debug_unknown_memory_read);

        connect(emulator,
                &Emulator::debug_illegal_instruction,
                this,
                &GBTest::debug_illegal_instruction);
#endif // LIBGB_DEBUG

        main_window = new MainWindow();

        connect(main_window->start_emulation,
                &QAction::triggered,
                this,
                &GBTest::start_emulation);

        connect(main_window->stop_emulation,
                &QAction::triggered,
                this,
                &GBTest::stop_emulation);

        connect(main_window->pause_emulation,
                &QAction::triggered,
                this,
                &GBTest::pause_emulation);

        connect(main_window->restart_emulation,
                &QAction::triggered,
                this,
                &GBTest::restart_emulation);

        connect(main_window->display_libgb_log,
                &QAction::triggered,
                this,
                &GBTest::display_libgb_log);

        main_window->setWindowTitle("libgb debugging station");
        main_window->resize(1280, 720);

        main_window->show();
    }
}

GBTest::~GBTest() noexcept
{ }

// Called when the user triggers "Debug -> Display libgb log" on the main
// window.
void GBTest::display_libgb_log() noexcept
{
    message_logger = new MessageLogger(main_window);
    message_logger->setAttribute(Qt::WA_DeleteOnClose);

    message_logger->setWindowTitle("libgb log");
    message_logger->resize(500, 500);

    message_logger->show();
}

// Called when the user triggers "Emulation -> Start" on the main window.
void GBTest::start_emulation() noexcept
{
    main_window->start_emulation->setDisabled(true);
    main_window->stop_emulation->setEnabled(true);
    main_window->pause_emulation->setEnabled(true);
    main_window->restart_emulation->setEnabled(true);

    emulator->start();
}

// Called when the user triggers "Emulation -> Stop" on the main window.
void GBTest::stop_emulation() noexcept
{
    main_window->start_emulation->setEnabled(true);
    main_window->stop_emulation->setDisabled(true);
    main_window->pause_emulation->setDisabled(true);
    main_window->restart_emulation->setDisabled(true);

    emulator->stop();
}

// Called when the user triggers "Emulation -> Pause" on the main window.
void GBTest::pause_emulation() noexcept
{
    main_window->start_emulation->setEnabled(true);
    main_window->stop_emulation->setEnabled(true);
    main_window->pause_emulation->setDisabled(true);
    main_window->restart_emulation->setEnabled(true);

    emulator->pause();
}

// Called when the user triggers "Emulation -> Restart" on the main window.
void GBTest::restart_emulation() noexcept
{
    stop_emulation();
    start_emulation();
}

#ifdef LIBGB_DEBUG
// Called when the emulator reports an unknown memory read has been trapped
void GBTest::debug_unknown_memory_read(const uint16_t address) noexcept
{
    if (message_logger && message_logger->unknown_memory_read->isChecked())
    {
        message_logger->append(QString("Unknown memory read: %1")
                               .arg(QString::number(address, 16).toUpper()));
    }
}

// Called when the emulator reports that an illegal instruction has been trapped
void GBTest::debug_illegal_instruction() noexcept
{
    stop_emulation();

    QString error_string;

    const auto instruction = QString("%1").arg(emulator->gb_emu->cpu->instruction, 2, 16, QChar('0')).toUpper();
    const auto pc          = QString("%1").arg(emulator->gb_emu->cpu->reg.pc, 4, 16, QChar('0')).toUpper();

    error_string = QString("Illegal instruction $%1 trapped at address $%2. "
                           "Emulation halted.")
                           .arg(instruction).arg(pc);

    QMessageBox::critical(main_window, tr("$8BADF00D"), error_string);

    if (message_logger)
    {
        message_logger->append(error_string);
    }
}
#endif // LIBGB_DEBUG