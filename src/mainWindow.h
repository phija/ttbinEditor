//
// Copyright 2016 by Philipp Jarvers
//

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "curveData.h"
#include "dataProcessor.h"

class DiagramScene;
class FileHandler;
class QListWidgetItem;
class QLabel;

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow();
    virtual ~MainWindow();

  protected slots:
    void showAbout();
    void showHelp();
    void setLoadedFilenameToStatusBar(const QString& filename);
    void setSavedFilenameToStatusBar(const QString& filename);
    void setModifiedFilenameToStatusBar();
    void addEntry(const QMap<int, int>& values, const QString& title, const QColor& color);
    void selectionChanged();
    void cleanData();
    void slotApplyFilter(bool useFilter);
    void slotWeightChanged(int);
    void slotFilterChanged(int);
    void slotAverageChanged(int);

  protected:
    void connectMenu();
    void setSelectedData(bool useFilter, bool resetAxes);
    void resetFilter();

    virtual void resizeEvent(QResizeEvent* event);
    virtual void showEvent(QShowEvent* event);

    Ui::MainWindow* m_ui;
    DiagramScene* m_scene;
    FileHandler* m_fileHandler;
    DataProcessor* m_dataProcessor;

    QHash<QListWidgetItem*, CurveData> m_diagramData;

    QListWidgetItem* m_listEntryHeartrate;
    QListWidgetItem* m_listEntrySpeed;
    QLabel* m_statusWidget;
};

#endif
