//
// Copyright 2016 by Philipp Jarvers
//

#include "diagramScene.h"
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QDateTime>
#include <QDebug>

#include <math.h>

DiagramScene::DiagramScene() :
  QGraphicsScene(),
  m_spaceX(50),
  m_spaceY(50),
  m_xTopSpace(40),
  m_yTopSpace(40),
  m_labelHeartrate(0),
  m_curve(0),
  m_xFactor(0),
  m_yDelta(0),
  m_heartrateSpacing(10),
  m_minHeartrate(0),
  m_maxHeartrate(0)
{
  m_xAxis = new QGraphicsLineItem(0,0,200,0);
  m_yAxis = new QGraphicsLineItem(0,0,0,200);

  addItem(m_xAxis);
  addItem(m_yAxis);

  QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem("min/max:");
  textItem->setPos(-30, -50);
  textItem->setTransform(QTransform().scale(1, -1));
  addItem(textItem);
}

void DiagramScene::adjustDiagram(int width, int height)
{
  int maxX = width - m_spaceY;
  int maxY = height - m_spaceX;
  setSceneRect(-m_spaceY, -m_spaceX, maxX, maxY);

  m_xAxis->setLine(0,0, maxX - m_xTopSpace, 0);
  m_yAxis->setLine(0,0, 0, maxY - m_yTopSpace);
}

DiagramScene::~DiagramScene()
{
  delete m_xAxis;
  delete m_yAxis;
}

void DiagramScene::setXTicks()
{
  if (!m_xAxisTicks.isEmpty())
  {
    qDeleteAll(m_xAxisTicks);
    m_xAxisTicks.clear();
  }
  if (m_heartrateData.size() > 0)
  {
    int stepSize = 5; // heartrate
    int startHr = floor((m_minHeartrate - m_heartrateSpacing)/(double)stepSize) * stepSize;
    int stopHr = floor((m_maxHeartrate + m_heartrateSpacing)/(double)stepSize) * stepSize;

    int maxX = sceneRect().width();

    for (int i = startHr; i <= stopHr; i += stepSize)
    {
      int y = heartrateToY(i);
      if (y <= 0) continue;
      QGraphicsLineItem* line = new QGraphicsLineItem(-5, y, maxX - m_xTopSpace, y);
      line->setPen(QPen(Qt::gray));

      QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(QString::number(i), line);
      text->setPos(-30, y + 6);
      text->setTransform(QTransform().scale(1, -1));
      addItem(line);
      m_xAxisTicks.append(line);
    }
  }
}


void DiagramScene::setYTicks()
{
  if (!m_yAxisTicks.isEmpty())
  {
    qDeleteAll(m_yAxisTicks);
    m_yAxisTicks.clear();
  }
  if (m_heartrateData.size() > 0)
  {
    int stepSize = 300; // seconds
    int startSec = 0;
    int stopSec = m_heartrateData.lastKey()/(double)stepSize * stepSize;

    int maxY = sceneRect().height();

    for (int i = startSec; i <= stopSec; i += stepSize)
    {
      int x = secondToX(i);
      QGraphicsLineItem* line = new QGraphicsLineItem(x, -5, x, maxY - m_yTopSpace);
      line->setPen(QPen(Qt::gray));

      QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(QString::number(i/60), line);
      text->setPos(x - 5, -20);
      text->setTransform(QTransform().scale(1, -1));
      addItem(line);
      m_yAxisTicks.append(line);
    }
  }
}

void DiagramScene::calculateHeartrateData(const QMap<int, int>& data)
{
  m_heartrateData = data;
  int heartrateSpacing = 10;

  if (m_heartrateData.size() > 0)
  {
    m_minHeartrate = INT_MAX;
    m_maxHeartrate = 0;

    for (QMap<int, int>::const_iterator it = m_heartrateData.begin(); it != m_heartrateData.end(); ++it)
    {
      if (it.value() < m_minHeartrate)
      {
        m_minHeartrate = it.value();
      }
      if (it.value() > m_maxHeartrate)
      {
        m_maxHeartrate = it.value();
      }
    }
  }

  int secsDelta = m_heartrateData.size() > 0 ? (m_heartrateData.lastKey() - m_heartrateData.firstKey()) : 0;
  m_xFactor = secsDelta / m_xAxis->line().length();

  m_yDelta = (m_maxHeartrate - m_minHeartrate + 2 * m_heartrateSpacing)/m_yAxis->line().length();
}

void DiagramScene::showHeartrate()
{
  if (m_curve != 0)
  {
    delete m_curve;
  }
  if (m_heartrateData.size() > 0)
  {
    m_curve = new QGraphicsPathItem();

    int minHeartrate = m_minHeartrate - m_heartrateSpacing;
    int xStart = m_heartrateData.firstKey();

    QPainterPath path(QPointF(0, (m_heartrateData.begin().value() - minHeartrate)/m_yDelta));

    for (QMap<int, int>::const_iterator it = m_heartrateData.begin(); it != m_heartrateData.end(); ++it)
    {
      int secs = it.key() - xStart;
      QPointF p(secs/m_xFactor, (it.value() - minHeartrate)/m_yDelta);
      path.lineTo(p);
    }

    m_curve->setPath(path);
    m_curve->setPen(QPen(Qt::red, 1));
    m_curve->setVisible(true);
    addItem(m_curve);
  }
}

void DiagramScene::reCalculateAndShowHeartrate()
{
  calculateHeartrateData(m_heartrateData);
  setXTicks();
  setYTicks();
  showHeartrate();
}

void DiagramScene::setLegend()
{
  QString hrText = QString::number(m_minHeartrate) + "/" + QString::number(m_maxHeartrate);
  if (m_labelHeartrate == 0)
  {
    m_labelHeartrate = new QGraphicsSimpleTextItem();
    m_labelHeartrate->setPos(40, -50);
    m_labelHeartrate->setTransform(QTransform().scale(1, -1));
    QFont font;
    font.setBold(true);
    m_labelHeartrate->setFont(font);
    addItem(m_labelHeartrate);
  }

  m_labelHeartrate->setText(hrText);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  if (mouseEvent->buttons()  == Qt::LeftButton)
  {
    int minSecs = qMin(xToSecond(mouseEvent->lastScenePos().x()), xToSecond(mouseEvent->scenePos().x()));
    int maxSecs = qMax(xToSecond(mouseEvent->lastScenePos().x()), xToSecond(mouseEvent->scenePos().x()));
    int heartrate = yToHeartrate(mouseEvent->scenePos().y());
    int start = qMax(0, minSecs - (int)ceil(m_xFactor));

    QMap<int, int>::iterator i = m_heartrateData.find(start);
    while (i != m_heartrateData.end() && i.key() <= maxSecs)
    {
      i.value() = heartrate;
      ++i;
    }

    showHeartrate();
  }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  adjustDiagram(views().first()->width(), views().first()->height());
  reCalculateAndShowHeartrate();
  setLegend();
  emit signalHeartrateChanged(m_heartrateData);
}

int DiagramScene::xToSecond(int x) const
{
  //x = sec/factor
  //<=> sec = x * factor
  return qRound(x * m_xFactor);
}

int DiagramScene::yToHeartrate(int y) const
{
  // y = (heartrate - minHeartrate)/deltaY
  // <=> heartrate = y * deltaY + minHeartrate
  return qRound(y * m_yDelta + (m_minHeartrate - m_heartrateSpacing));
}

int DiagramScene::heartrateToY(int hr) const
{
  // y = (heartrate - minHeartrate)/deltaY
  // <=> heartrate = y * deltaY + minHeartrate
  return qRound((hr - (m_minHeartrate - m_heartrateSpacing))/m_yDelta);
}

int DiagramScene::secondToX(int second) const
{
  return qRound(second/m_xFactor);
}
