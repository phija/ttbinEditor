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
  m_labelValue(0),
  m_curve(0),
  m_xFactor(0),
  m_yDelta(0),
  m_TopBottomSpacing(10),
  m_minValue(0),
  m_maxValue(0),
  m_averageValue(0)
{
  m_xAxis = new QGraphicsLineItem(0,0,200,0);
  m_yAxis = new QGraphicsLineItem(0,0,0,200);

  addItem(m_xAxis);
  addItem(m_yAxis);

  QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem("min/max (avg):");
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
  if (m_data.size() > 0)
  {
    int stepSize = 50.0 * m_yDelta;
    int startValue = floor((m_minValue - m_TopBottomSpacing)/(double)stepSize) * stepSize;
    int stopValue = floor((m_maxValue + m_TopBottomSpacing)/(double)stepSize) * stepSize;

    int maxX = sceneRect().width();

    for (int i = startValue; i <= stopValue; i += stepSize)
    {
      int y = valueToY(i);
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
  if (m_data.size() > 0)
  {
    int stepSize = 300; // seconds
    int startSec = 0;
    int stopSec = m_data.lastKey()/(double)stepSize * stepSize;

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

void DiagramScene::setAndCalculateData(const QMap<int, int>& data, const QColor& color, bool keepOldMinMax)
{
  m_data = data;
  m_color = color;

  if (m_data.size() > 0)
  {
    if (!keepOldMinMax)
    {
      m_minValue = INT_MAX;
      m_maxValue = 0;
    }

    m_averageValue = 0;
    for (QMap<int, int>::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
    {
      m_averageValue += it.value();
      if (!keepOldMinMax)
      {
        if (it.value() < m_minValue)
        {
          m_minValue = it.value();
        }
        if (it.value() > m_maxValue)
        {
          m_maxValue = it.value();
        }
      }
    }
    m_averageValue /= m_data.size();
  }

  int secsDelta = m_data.size() > 0 ? (m_data.lastKey() - m_data.firstKey()) : 0;
  m_xFactor = secsDelta / m_xAxis->line().length();

  if (!keepOldMinMax)
  {
    m_yDelta = (m_maxValue - m_minValue + 2 * m_TopBottomSpacing)/m_yAxis->line().length();
  }
}

void DiagramScene::showCurve()
{
  if (m_curve != 0)
  {
    delete m_curve;
    m_curve = 0;
  }
  if (m_data.size() > 0)
  {
    m_curve = new QGraphicsPathItem();

    int minHeartrate = m_minValue - m_TopBottomSpacing;
    int xStart = m_data.firstKey();

    QPainterPath path(QPointF(0, (m_data.begin().value() - minHeartrate)/m_yDelta));

    for (QMap<int, int>::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
    {
      int secs = it.key() - xStart;
      QPointF p(secs/m_xFactor, (it.value() - minHeartrate)/m_yDelta);
      path.lineTo(p);
    }

    m_curve->setPath(path);
    m_curve->setPen(QPen(m_color, 1));
    m_curve->setVisible(true);
    addItem(m_curve);
  }
}

void DiagramScene::reCalculateAndShowValues()
{
  setAndCalculateData(m_data, m_color);
  setXTicks();
  setYTicks();
  showCurve();
}

void DiagramScene::setLegend()
{
  QString hrText = QString::number(m_minValue) + "/" + QString::number(m_maxValue) +
                   " (" + QString::number(qRound(m_averageValue))+ ")";
  if (m_labelValue == 0)
  {
    m_labelValue = new QGraphicsSimpleTextItem();
    m_labelValue->setPos(80, -50);
    m_labelValue->setTransform(QTransform().scale(1, -1));
    QFont font;
    font.setBold(true);
    m_labelValue->setFont(font);
    addItem(m_labelValue);
  }

  m_labelValue->setText(hrText);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  if (mouseEvent->buttons()  == Qt::LeftButton)
  {
    int minSecs = qMin(xToSecond(mouseEvent->lastScenePos().x()), xToSecond(mouseEvent->scenePos().x()));
    int maxSecs = qMax(xToSecond(mouseEvent->lastScenePos().x()), xToSecond(mouseEvent->scenePos().x()));
    int heartrate = yToValue(mouseEvent->scenePos().y());
    int start = qMax(0, minSecs - (int)ceil(m_xFactor));

    QMap<int, int>::iterator i = m_data.find(start);
    while (i != m_data.end() && i.key() <= maxSecs)
    {
      i.value() = heartrate;
      ++i;
    }

    showCurve();
  }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  adjustDiagram(views().first()->width(), views().first()->height());
  reCalculateAndShowValues();
  setLegend();
  emit signalValuesChanged(m_data);
}

int DiagramScene::xToSecond(int x) const
{
  //x = sec/factor
  //<=> sec = x * factor
  return qRound(x * m_xFactor);
}

int DiagramScene::secondToX(int second) const
{
  return qRound(second/m_xFactor);
}

int DiagramScene::yToValue(int y) const
{
  // e.g. for heartrates:
  // y = (heartrate - minHeartrate)/deltaY
  // <=> heartrate = y * deltaY + minHeartrate
  return qRound(y * m_yDelta + (m_minValue - m_TopBottomSpacing));
}

int DiagramScene::valueToY(int value) const
{
  // e.g. for heartrates:
  // y = (heartrate - minHeartrate)/deltaY
  // <=> heartrate = y * deltaY + minHeartrate
  return qRound((value - (m_minValue - m_TopBottomSpacing))/m_yDelta);
}

