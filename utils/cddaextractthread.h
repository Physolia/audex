/* AUDEX CDDA EXTRACTOR
 * SPDX-FileCopyrightText: 2007-2015 Marco Nelles (audex@maniatek.com)
 * <https://userbase.kde.org/Audex>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CDDAEXTRACTTHREAD_HEADER
#define CDDAEXTRACTTHREAD_HEADER

#include <QString>
#include <QThread>

#include <KLocalizedString>

#include "cddaparanoia.h"

class CDDAExtractThread : public QThread
{
    Q_OBJECT
public:
    CDDAExtractThread(QObject *parent, CDDAParanoia *_paranoia);
    ~CDDAExtractThread() override;

public Q_SLOTS:
    void start();
    void cancel();
    void setParanoiaMode(int mode)
    {
        paranoia_mode = mode;
    }
    void setMaxRetries(int r)
    {
        paranoia_retries = r;
    }
    void setNeverSkip(bool b)
    {
        never_skip = b;
    }
    void setSampleOffset(const int offset)
    {
        sample_offset = offset;
    }
    void setTrackToRip(unsigned int t)
    {
        track = t;
    } // if t==0 rip whole cd
    void skipTrack(unsigned int t)
    {
        overall_sectors_read += paranoia->numOfFramesOfTrack(t);
    }

    bool isProcessing();

    const QStringList &protocol();

    CDDAParanoia *cddaParanoia()
    {
        return paranoia;
    }

private Q_SLOTS:
    void slot_error(const QString &message, const QString &details);

Q_SIGNALS:
    void output(const QByteArray &);

    void progress(const int percent, const int sector, const int sectorsOverall);

    void error(const QString &message, const QString &details = QString());
    void warning(const QString &message);
    void info(const QString &message);

protected:
    /** reimplemented from QThread. Does the work */
    void run() override;

private:
    CDDAParanoia *paranoia;

    long first_sector;
    long current_sector;
    long last_sector;
    unsigned long sectors_read;
    unsigned long overall_sectors_read;
    unsigned long sectors_all;

    int paranoia_mode;
    int paranoia_retries;
    bool never_skip;
    int sample_offset;
    bool sample_offset_done;

    unsigned int track;

    bool b_interrupt;
    bool b_error;

    QStringList extract_protocol;

    void createStatus(long, int);

    // status variables
    long last_read_sector;
    long overlap;
    long read_sectors;

    bool read_error;
    bool scratch_detected;

    // this friend function will call createStatus(long, int)
    friend void paranoiaCallback(long, int);
};

#endif
