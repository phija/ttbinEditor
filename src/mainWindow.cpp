//
// Copyright 2016 by Philipp Jarvers
//

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QMessageBox>
#include <climits>

char static_assert_float32[1 - (2 * ((sizeof(float) * CHAR_BIT) != 32))];

#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "diagramScene.h"
#include "fileHandler.h"

static QString VERSION = "0.2";

MainWindow::MainWindow() :
  QMainWindow(0),
  m_ui(0),
  m_scene(0),
  m_fileHandler(0),
  m_listEntryHeartrate(0),
  m_statusWidget(0)
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

  m_ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

  connectMenu();

  m_statusWidget = new QLabel("Load a ttbin file to start.");
  m_ui->statusBar->addWidget(m_statusWidget);
}

void MainWindow::connectMenu()
{
  QObject::connect(m_ui->actionLoadFile, SIGNAL(triggered()), m_fileHandler, SLOT(slotLoadFile()));
  QObject::connect(m_ui->actionSave, SIGNAL(triggered()), m_fileHandler, SLOT(slotSaveFile()));
  QObject::connect(m_ui->actionSave_as, SIGNAL(triggered()), m_fileHandler, SLOT(slotSaveFileAs()));
  QObject::connect(m_ui->actionQuit, SIGNAL(triggered()), SLOT(close()));
  QObject::connect(m_ui->actionHelp, SIGNAL(triggered()), SLOT(showHelp()));
  QObject::connect(m_ui->actionAbout, SIGNAL(triggered()), SLOT(showAbout()));
  QObject::connect(m_fileHandler, SIGNAL(fileLoaded(QString)), SLOT(cleanData()));
  QObject::connect(m_fileHandler, SIGNAL(fileLoaded(QString)), SLOT(setLoadedFilenameToStatusBar(QString)));
  QObject::connect(m_fileHandler, SIGNAL(fileSaved(QString)), SLOT(setSavedFilenameToStatusBar(QString)));
  QObject::connect(m_fileHandler, SIGNAL(loaded(QMap<int, int>,QString,QColor)), SLOT(addEntry(QMap<int, int>,QString,QColor)));
  QObject::connect(m_scene, SIGNAL(signalHeartrateChanged(QMap<int, int>)), m_fileHandler, SLOT(slotSetHeartrateDataToTTBIN(QMap<int, int>)));
  QObject::connect(m_scene, SIGNAL(signalHeartrateChanged(QMap<int, int>)), SLOT(setModifiedFilenameToStatusBar()));
  QObject::connect(m_ui->listWidget, SIGNAL(itemSelectionChanged()), SLOT(selectionChanged()));
}

void MainWindow::showAbout()
{
  QMessageBox::about(this, "About ttbinEditor", "This is ttbinEditor version " + VERSION + ".\n\nCopyright 2016 by Philipp Jarvers.");
}

void MainWindow::showHelp()
{
  QMessageBox::information(this, "Help", "\
1. Load and save files via the menu\n\
2. Edit the data in the diagram by pressing the left mouse button and moving the mouse.\n\
\n\
If you do not have ttbin files, go and get a TOMTOM sports watch and start using it! ;)");
}

void MainWindow::setLoadedFilenameToStatusBar(const QString& filename)
{
  m_statusWidget->setText("Loaded File: " + filename);
}

void MainWindow::setSavedFilenameToStatusBar(const QString& filename)
{
  m_statusWidget->setText("Saved File: " + filename);
}

void MainWindow::setModifiedFilenameToStatusBar()
{
  QStringList texts = m_statusWidget->text().split(": ");
  if (texts.size() > 1)
  {
    m_statusWidget->setText("Modified: " + texts[1]);
  }
}

void MainWindow::cleanData()
{
  qDeleteAll(m_diagramData.keys());
  m_diagramData.clear();
  m_ui->listWidget->setItemSelected(0, false);
}

void MainWindow::selectionChanged()
{
  if (m_ui->listWidget->selectedItems().size() == 0) return;

  QListWidgetItem* item = m_ui->listWidget->selectedItems().first();
  if (item != 0)
  {
    QHash<QListWidgetItem*, CurveData>::iterator iterator = m_diagramData.find(item);
    if (iterator != m_diagramData.end())
    {
      CurveData& data = iterator.value();
      m_scene->setAndCalculateData(data.m_values, data.m_color);
      m_scene->setXTicks();
      m_scene->setYTicks();
      m_scene->showCurve();
      m_scene->setLegend();
    }
  }
}

void MainWindow::addEntry(const QMap<int, int>& values, const QString& title, const QColor& color)
{
  QListWidgetItem* listEntry = new QListWidgetItem(title + " (" + QString::number(values.count()) + " entries)");
  m_ui->listWidget->addItem(listEntry);

  m_diagramData.insert(listEntry, CurveData(values, color));

  // select item if nothing is yet selected
  if (m_ui->listWidget->selectedItems().isEmpty())
  {
    m_ui->listWidget->setItemSelected(listEntry, true);
  }
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
