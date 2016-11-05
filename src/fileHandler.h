//
// Copyright 2016 by Philipp Jarvers
//

#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

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
    void heartrateLoaded(const QMap<int, int>&);

  public slots:
    void slotLoadFile();
    void slotSaveFile();
    void slotSaveFileAs();
    void slotSetHeartrateDataToTTBIN(const QMap<int, int>& heartrateData);

  protected:
    QMap<int, int> convertRecordsHeartrate(RECORD_ARRAY array);

    QWidget* m_parent;

    TTBIN_FILE* m_ttbin;
    QString m_filename;
};

#endif
