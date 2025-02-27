/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: 2007-2015 Marco Nelles (audex@maniatek.com)
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "wavewidget.h"

#include <QDebug>

waveWidget::waveWidget(Parameters *parameters, QWidget *parent)
    : waveWidgetUI(parent)
{
    Q_UNUSED(parent);

    this->parameters = parameters;
    if (!parameters) {
        qDebug() << "ParameterString is NULL!";
        return;
    }

    qlineedit_suffix->setText(parameters->value(ENCODER_WAVE_SUFFIX_KEY, ENCODER_WAVE_SUFFIX));

    connect(qlineedit_suffix, SIGNAL(textEdited(const QString &)), this, SLOT(trigger_changed()));

    changed = false;
}

waveWidget::~waveWidget()
{
}

bool waveWidget::save()
{
    bool success = true;

    parameters->setValue(ENCODER_WAVE_SUFFIX_KEY, qlineedit_suffix->text());

    changed = false;

    return success;
}

void waveWidget::trigger_changed()
{
    changed = (qlineedit_suffix->text() != parameters->value(ENCODER_WAVE_SUFFIX_KEY, ENCODER_WAVE_SUFFIX));

    Q_EMIT triggerChanged();
}
