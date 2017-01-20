//
// Copyright 2016 by Philipp Jarvers
//

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
extern "C"
{
  #include "ttbin/ttbin.h"
}

class FileHandler : public QObject
{
    Q_OBJECT

  public:
    FileHandler(QWidget* parent);
    virtual ~FileHandler() {};

    bool readFile(const QString& filename);
    bool saveFile();
    bool saveFileAs(const QString& filename);
    void clearFile();

  signals:
    void fileLoaded(const QString& filename);
    void fileSaved(const QString& filename);
    void loaded(const QMap<int, int>&, const QString&, const QColor&);

  public slots:
    void slotLoadFile();
    void slotSaveFile();
    void slotSaveFileAs();
    void slotSetHeartrateDataToTTBIN(const QMap<int, int>& heartrateData);

  protected:
    QMap<int, int> convertRecordsHeartrate(RECORD_ARRAY array);
    QMap<int, int> convertGpsRecordsToSpeed(RECORD_ARRAY array);
//    QMap<int, int> convertAltRecordsToElevation(RECORD_ARRAY array);

    QWidget* m_parent;

    TTBIN_FILE* m_ttbin;
    QString m_filename;
};

#endif
