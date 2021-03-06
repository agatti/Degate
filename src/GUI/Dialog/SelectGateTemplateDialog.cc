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

#include "SelectGateTemplateDialog.h"

#include <utility>

namespace degate
{
    SelectGateTemplateDialog::SelectGateTemplateDialog(QWidget* parent, Project_shptr project, bool unique_selection)
            : QDialog(parent), list(this, std::move(project), unique_selection)
    {
        validate_button.setText(tr("Ok"));
        layout.addWidget(&list);
        layout.addWidget(&validate_button);

        setLayout(&layout);

        QObject::connect(&validate_button, SIGNAL(clicked()), this, SLOT(accept()));
    }

    GateTemplate_shptr SelectGateTemplateDialog::get_selected_gate()
    {
        return list.get_selected_gate();
    }

    std::vector<GateTemplate_shptr> SelectGateTemplateDialog::get_selected_gates()
    {
        return list.get_selected_gates();
    }
}
