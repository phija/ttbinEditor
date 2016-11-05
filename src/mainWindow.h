//
// Copyright 2016 by Philipp Jarvers
//

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class DiagramScene;
class FileHandler;

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
    void setFilenameToStatusBar(const QString& filename);
    void setHeartrate(const QMap<int, int>& values);

  protected:
    void connectMenu();

    virtual void resizeEvent(QResizeEvent* event);
    virtual void showEvent(QShowEvent* event);
    Ui::MainWindow* m_ui;
    DiagramScene* m_scene;
    FileHandler* m_fileHandler;
};

#endif
