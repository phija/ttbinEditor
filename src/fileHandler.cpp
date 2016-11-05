
#include "fileHandler.h"
#include <QDateTime>

extern "C"
{
  #include "ttbin/ttbin.h"
}

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

typedef struct _IO_FILE FILE;

FileHandler::FileHandler(QWidget* parent) :
  QObject(),
  m_parent(parent),
  m_ttbin(0)
{
}

FileHandler::~FileHandler()
{
}

void FileHandler::slotLoadFile()
{
  QString fileName = QFileDialog::getOpenFileName(m_parent, tr("Open File"), "", tr("Files (*.ttbin)"));
  qDebug() << "opening " << fileName;
  if (!fileName.isEmpty())
  {
    readFile(fileName);
  }
}

void FileHandler::slotSaveFile()
{
  if (QMessageBox::question(m_parent, "Confirm", "Confirm to overwrite current file?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok)
  {
    saveFile();
  }
}

void FileHandler::slotSaveFileAs()
{
  QString fileName = QFileDialog::getSaveFileName(m_parent, tr("Save File As"), "", tr("Files (*.ttbin)"));
  saveFileAs(fileName);
}

void FileHandler::slotSetHeartrateDataToTTBIN(const QMap<int, int>& heartrateData)
{
  qDebug() << "set to TTBIN!";

  RECORD_ARRAY& array = m_ttbin->heart_rate_records;
  if (array.records != 0 && array.count > 0)
  {
    qDebug() << "record has" << array.count << "entries.";
    int i = 0;
    int tag = -1;
    QDateTime startTime;
    for (TTBIN_RECORD* record = *array.records; i < array.count; record = record->next)
    {
      if (tag < 0)
      {
        tag = record->tag;
        startTime = QDateTime::fromTime_t(record->heart_rate.timestamp);
      }
      if (record != 0 && tag == record->tag)
      {
        int pos = startTime.secsTo(QDateTime::fromTime_t(record->heart_rate.timestamp));
        QMap<int, int>::const_iterator data = heartrateData.find(pos);
        if (data != heartrateData.end())
        {
          record->heart_rate.heart_rate = data.value();
        }
        ++i;
      }
    }
  }
}

bool FileHandler::readFile(const QString& filename)
{
  FILE* input_file = fopen(filename.toLatin1(), "r");
  if (!input_file)
  {
    qDebug() << "Unable to open input file: " << filename;
    return false;
  }

  clearFile();
  /* read the ttbin data file */
  m_ttbin = read_ttbin_file(input_file);
  if (input_file != stdin)
  {
    fclose(input_file);
  }
  if (!m_ttbin)
  {
    qDebug() << "Unable to read and parse TTBIN file";
    return false;
  }

  m_filename = filename;
  emit fileLoaded(filename);
  emit heartrateLoaded(convertRecordsHeartrate(m_ttbin->heart_rate_records));
  return true;
}

bool FileHandler::saveFile()
{
  return saveFileAs(m_filename);
}

bool FileHandler::saveFileAs(const QString& outFile)
{
  FILE* output_file = fopen(outFile.toLatin1(), "w");
  qDebug() << (long)(output_file);
  if (!output_file)
  {
    qDebug() << "Unable to open output file: " << outFile;
    return false;
  }

  if (!m_ttbin)
  {
    qDebug() << "Nothing to write out to a file!";
    return false;
  }
  /* write the output file */
  write_ttbin_file(m_ttbin, output_file);
  if (output_file != stdout)
  {
    fclose(output_file);
  }
  return true;
}

void FileHandler::clearFile()
{
  free_ttbin(m_ttbin);
  m_ttbin = 0;
}

QMap<int, int> FileHandler::convertRecordsHeartrate(RECORD_ARRAY array)
{
  QMap<int, int> values;
  if (array.records != 0 && array.count > 0)
  {
    qDebug() << "record has" << array.count << "entries.";
    int i = 0;
    int tag = -1;
    QDateTime startTime;
    for (TTBIN_RECORD* record = *array.records; i < array.count; record = record->next)
    {
      if (tag < 0)
      {
        tag = record->tag;
        startTime = QDateTime::fromTime_t(record->heart_rate.timestamp);
      }
      if (record != 0 && tag == record->tag)
      {
        QDateTime dateTime = QDateTime::fromTime_t(record->heart_rate.timestamp);
        //qDebug() << "  adding:" << dateTime.toString("dd HH:mm:ss") << " hr:" << record->heart_rate.heart_rate << " ptr:" << (long)record << " length:" << record->length << record->tag;
        values.insert(startTime.secsTo(dateTime), record->heart_rate.heart_rate);
        ++i;
      }
    }
  }
  return values;
}

