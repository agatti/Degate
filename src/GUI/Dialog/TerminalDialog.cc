/**
 * This file is part of the IC reverse engineering tool Degate.
 *
 * Copyright 2008, 2009, 2010 by Martin Schobert
 * Copyright 2019-2020 Dorian Bachelot
 *
 * Degate is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Degate is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with degate. If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "TerminalDialog.h"

namespace degate
{

    // Widget

    TerminalWidget::TerminalWidget(QWidget* parent, TerminalCommands& commands)
            : commands(commands), process(parent)
    {
        terminal.setReadOnly(true);
        layout.addWidget(&terminal);

        QObject::connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(write_output()));
        QObject::connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(write_error()));
        QObject::connect(&process, SIGNAL(finished(int)), this, SLOT(process_finished()));

        setLayout(&layout);
    }

    TerminalWidget::~TerminalWidget()
    {
        terminal.close();
    }

    void TerminalWidget::write_output()
    {
        terminal.append(process.readAllStandardOutput());
    }

    void TerminalWidget::write_error()
    {
        terminal.append(process.readAllStandardError());
    }

    void TerminalWidget::process_finished()
    {
        if (commands.empty())
            return;

        start();
    }

    void TerminalWidget::start()
    {
        auto pop = commands.begin();
        TerminalCommand command = *pop;
        commands.erase(pop);

        // Print command.
        QString string_command = "> " + command.program;
        for (auto& item : command.arguments)
            string_command += " " + item;
        terminal.append(string_command + "\n");

        process.start(command.program, command.arguments);
    }

    std::string TerminalWidget::get_output()
    {
        return terminal.toPlainText().toStdString();
    }


    // Dialog

    TerminalDialog::TerminalDialog(QWidget* parent, TerminalCommands& commands)
            : terminal(parent, commands), button_box(QDialogButtonBox::Ok)
    {
        setWindowTitle(tr("Degate terminal"));
        resize(500, 400);

        layout.addWidget(&terminal);
        layout.addWidget(&button_box);

        setLayout(&layout);

        QObject::connect(&button_box, SIGNAL(accepted()), this, SLOT(finish()));
    }

    void TerminalDialog::start()
    {
        if (!isVisible())
            open();

        terminal.start();
    }

    void TerminalDialog::finish()
    {
        close();
    }

    std::string TerminalDialog::get_output()
    {
        return terminal.get_output();
    }
}
