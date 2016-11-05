//
// Copyright 2016 by Philipp Jarvers
//
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QMessageBox>

#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "diagramScene.h"
#include "fileHandler.h"

static QString VERSION = "0.1";

MainWindow::MainWindow() :
  QMainWindow(0),
  m_ui(0),
  m_scene(0),
  m_fileHandler(0)
{
  m_ui = new Ui::MainWindow();
  m_ui->setupUi(this);

  const int initialWidth = 1200;
  const int initialHeight = 600;

  m_scene = new DiagramScene();
  m_ui->graphicsView->setScene(m_scene);

  m_ui->graphicsView->setRenderHints( QPainter::Antialiasing );
  m_ui->graphicsView->scale(1, -1);
  m_ui->graphicsView->setMouseTracking(true);

  m_fileHandler = new FileHandler(this);

  connectMenu();

  m_ui->statusBar->showMessage("loaded", 5000);
}

void MainWindow::connectMenu()
{
  QObject::connect(m_ui->actionAbout, SIGNAL(triggered()), SLOT(showAbout()));
  QObject::connect(m_ui->actionLoadFile, SIGNAL(triggered()), m_fileHandler, SLOT(slotLoadFile()));
  QObject::connect(m_ui->actionSave, SIGNAL(triggered()), m_fileHandler, SLOT(slotSaveFile()));
  QObject::connect(m_ui->actionSave_as, SIGNAL(triggered()), m_fileHandler, SLOT(slotSaveFileAs()));
  QObject::connect(m_ui->actionQuit, SIGNAL(triggered()), SLOT(close()));
  QObject::connect(m_fileHandler, SIGNAL(fileLoaded(QString)), SLOT(setFilenameToStatusBar(QString)));
  QObject::connect(m_fileHandler, SIGNAL(heartrateLoaded(QMap<int, int>)), SLOT(setHeartrate(QMap<int, int>)));
  QObject::connect(m_scene, SIGNAL(signalHeartrateChanged(QMap<int, int>)), m_fileHandler, SLOT(slotSetHeartrateDataToTTBIN(QMap<int, int>)));
}

void MainWindow::showAbout()
{
  QMessageBox::about(this, "About ttbinEditor", "This is ttbinEditor version " + VERSION + ".\n\nCopyright 2016 by Philipp Jarvers.");
}

void MainWindow::setFilenameToStatusBar(const QString& filename)
{
  m_ui->statusBar->showMessage("File: " + filename);
}

void MainWindow::setHeartrate(const QMap<int, int>& values)
{
  qDebug() << "showing heartrate with " << values.size() << " entries.";
  //m_scene->setHeartrate(values);
  m_scene->calculateHeartrateData(values);
  m_scene->setXTicks();
  m_scene->setYTicks();
  m_scene->showHeartrate();
  m_scene->setLegend();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
  QMainWindow::resizeEvent(event);
  m_scene->adjustDiagram(m_ui->graphicsView->width(), m_ui->graphicsView->height());
  m_scene->reCalculateAndShowHeartrate();
  m_scene->setLegend();
}

void MainWindow::showEvent(QShowEvent* event)
{
  QMainWindow::showEvent(event);
  m_scene->adjustDiagram(m_ui->graphicsView->width(), m_ui->graphicsView->height());
}

MainWindow::~MainWindow()
{
  delete m_fileHandler;
  delete m_scene;
  delete m_ui;
}
