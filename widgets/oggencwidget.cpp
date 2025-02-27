/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: 2007-2015 Marco Nelles (audex@maniatek.com)
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "oggencwidget.h"

#include <QDebug>

oggencWidget::oggencWidget(Parameters *parameters, QWidget *parent)
    : oggencWidgetUI(parent)
{
    Q_UNUSED(parent);

    this->parameters = parameters;
    if (!parameters) {
        qDebug() << "ParameterString is NULL!";
        return;
    }

    horizontalSlider_quality->setValue((int)(parameters->valueToFloat(ENCODER_OGGENC_QUALITY_KEY, ENCODER_OGGENC_QUALITY) * 10));

    kdoublenuminput_quality->setValue(parameters->valueToFloat(ENCODER_OGGENC_QUALITY_KEY, ENCODER_OGGENC_QUALITY));

    set_bitrate(parameters->valueToInt(ENCODER_OGGENC_QUALITY_KEY, ENCODER_OGGENC_QUALITY));

    enable_min_bitrate(parameters->valueToBool(ENCODER_OGGENC_MINBITRATE_KEY));
    checkBox_minbitrate->setChecked(parameters->valueToBool(ENCODER_OGGENC_MINBITRATE_KEY));
    kintspinbox_minbitrate->setValue(parameters->valueToInt(ENCODER_OGGENC_MINBITRATE_VALUE_KEY, ENCODER_OGGENC_MINBITRATE_VALUE));
    set_minbitrate(parameters->valueToInt(ENCODER_OGGENC_MINBITRATE_VALUE_KEY, ENCODER_OGGENC_MINBITRATE_VALUE));
    enable_max_bitrate(parameters->valueToBool(ENCODER_OGGENC_MAXBITRATE_KEY));
    checkBox_maxbitrate->setChecked(parameters->valueToBool(ENCODER_OGGENC_MAXBITRATE_KEY));
    kintspinbox_maxbitrate->setValue(parameters->valueToInt(ENCODER_OGGENC_MAXBITRATE_VALUE_KEY, ENCODER_OGGENC_MAXBITRATE_VALUE));
    set_maxbitrate(parameters->valueToInt(ENCODER_OGGENC_MAXBITRATE_VALUE_KEY, ENCODER_OGGENC_MAXBITRATE_VALUE));
    qlineedit_suffix->setText(parameters->value(ENCODER_OGGENC_SUFFIX_KEY, ENCODER_OGGENC_SUFFIX));

    connect(horizontalSlider_quality, SIGNAL(valueChanged(int)), this, SLOT(quality_changed_by_slider(int)));
    connect(horizontalSlider_quality, SIGNAL(valueChanged(int)), this, SLOT(trigger_changed()));

    connect(kdoublenuminput_quality, SIGNAL(valueChanged(double)), this, SLOT(quality_changed_by_spinbox(double)));
    connect(kdoublenuminput_quality, SIGNAL(valueChanged(double)), this, SLOT(trigger_changed()));

    connect(checkBox_minbitrate, SIGNAL(toggled(bool)), this, SLOT(enable_min_bitrate(bool)));
    connect(checkBox_minbitrate, SIGNAL(toggled(bool)), this, SLOT(trigger_changed()));
    connect(checkBox_maxbitrate, SIGNAL(toggled(bool)), this, SLOT(enable_max_bitrate(bool)));
    connect(checkBox_maxbitrate, SIGNAL(toggled(bool)), this, SLOT(trigger_changed()));

    connect(kintspinbox_minbitrate, SIGNAL(valueChanged(int)), this, SLOT(trigger_changed()));
    connect(kintspinbox_minbitrate, SIGNAL(valueChanged(int)), this, SLOT(set_minbitrate(int)));
    connect(kintspinbox_maxbitrate, SIGNAL(valueChanged(int)), this, SLOT(trigger_changed()));
    connect(kintspinbox_maxbitrate, SIGNAL(valueChanged(int)), this, SLOT(set_maxbitrate(int)));

    connect(qlineedit_suffix, SIGNAL(textEdited(const QString &)), this, SLOT(trigger_changed()));

    changed = false;
}

oggencWidget::~oggencWidget()
{
}

bool oggencWidget::save()
{
    bool success = true;

    parameters->setValue(ENCODER_OGGENC_QUALITY_KEY, kdoublenuminput_quality->value());
    parameters->setValue(ENCODER_OGGENC_MINBITRATE_KEY, checkBox_minbitrate->isChecked());
    parameters->setValue(ENCODER_OGGENC_MINBITRATE_VALUE_KEY, kintspinbox_minbitrate->value());
    parameters->setValue(ENCODER_OGGENC_MAXBITRATE_KEY, checkBox_maxbitrate->isChecked());
    parameters->setValue(ENCODER_OGGENC_MAXBITRATE_VALUE_KEY, kintspinbox_maxbitrate->value());
    parameters->setValue(ENCODER_OGGENC_SUFFIX_KEY, qlineedit_suffix->text());

    changed = false;

    return success;
}

void oggencWidget::quality_changed_by_slider(int quality)
{
    auto q = (float)((float)quality / 10.0f);

    kdoublenuminput_quality->blockSignals(true);
    kdoublenuminput_quality->setValue(q);
    kdoublenuminput_quality->blockSignals(false);

    set_bitrate(q);
}

void oggencWidget::quality_changed_by_spinbox(double quality)
{
    int q = (int)((float)quality * 10.0f);

    horizontalSlider_quality->blockSignals(true);
    horizontalSlider_quality->setValue(q);
    horizontalSlider_quality->blockSignals(false);

    set_bitrate(quality);
}

void oggencWidget::set_bitrate(float quality)
{
    int q = (int)quality;

    switch (q) {
    case -1:
        lineEdit_kbps->setText("48");
        break;
    case 0:
        lineEdit_kbps->setText("64");
        break;
    case 1:
        lineEdit_kbps->setText("80");
        break;
    case 2:
        lineEdit_kbps->setText("96");
        break;
    case 3:
        lineEdit_kbps->setText("112");
        break;
    case 4:
        lineEdit_kbps->setText("128");
        break;
    case 5:
        lineEdit_kbps->setText("160");
        break;
    case 6:
        lineEdit_kbps->setText("192");
        break;
    case 7:
        lineEdit_kbps->setText("224");
        break;
    case 8:
        lineEdit_kbps->setText("256");
        break;
    case 9:
        lineEdit_kbps->setText("320");
        break;
    case 10:
        lineEdit_kbps->setText("500");
        break;
    }
}

void oggencWidget::enable_min_bitrate(bool enable)
{
    kintspinbox_minbitrate->setEnabled(enable);
    label_minbitrate_kbps->setEnabled(enable);
}

void oggencWidget::set_minbitrate(int bitrate)
{
    kintspinbox_maxbitrate->blockSignals(true);
    kintspinbox_maxbitrate->setMinimum(bitrate);
    kintspinbox_maxbitrate->blockSignals(false);
}

void oggencWidget::enable_max_bitrate(bool enable)
{
    kintspinbox_maxbitrate->setEnabled(enable);
    label_maxbitrate_kbps->setEnabled(enable);
}

void oggencWidget::set_maxbitrate(int bitrate)
{
    kintspinbox_minbitrate->blockSignals(true);
    kintspinbox_minbitrate->setMaximum(bitrate);
    kintspinbox_minbitrate->blockSignals(false);
}

void oggencWidget::trigger_changed()
{
    changed = (kdoublenuminput_quality->value() != parameters->valueToFloat(ENCODER_OGGENC_QUALITY_KEY, ENCODER_OGGENC_QUALITY) ||
               kintspinbox_minbitrate->value() != parameters->valueToInt(ENCODER_OGGENC_MINBITRATE_VALUE_KEY, ENCODER_OGGENC_MINBITRATE_VALUE) || checkBox_minbitrate->isChecked() != parameters->valueToBool(ENCODER_OGGENC_MINBITRATE_KEY) ||
               kintspinbox_maxbitrate->value() != parameters->valueToInt(ENCODER_OGGENC_MAXBITRATE_VALUE_KEY, ENCODER_OGGENC_MAXBITRATE_VALUE) || checkBox_maxbitrate->isChecked() != parameters->valueToBool(ENCODER_OGGENC_MAXBITRATE_KEY) ||
               qlineedit_suffix->text() != parameters->value(ENCODER_OGGENC_SUFFIX_KEY, ENCODER_OGGENC_SUFFIX));

    Q_EMIT triggerChanged();
}
