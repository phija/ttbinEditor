//
// Copyright 2016 by Philipp Jarvers
//

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QDateTime>

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

  public:
    DiagramScene();
    virtual ~DiagramScene();

    void adjustDiagram(int width, int height);

    void setAndCalculateData(const QMap<int, int>& data, const QColor& color);
    void showCurve();
    void reCalculateAndShowHeartrate();

    void setXTicks();
    void setYTicks();

    void setLegend();

  signals:
    void signalHeartrateChanged(const QMap<int, int>&);

  protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);

    int xToSecond(int x) const;
    int yToHeartrate(int y) const;
    int heartrateToY(int hr) const;
    int secondToX(int second) const;

    int m_spaceX;
    int m_spaceY;
    int m_xTopSpace;
    int m_yTopSpace;

    QGraphicsLineItem* m_xAxis;
    QGraphicsLineItem* m_yAxis;
    QList<QGraphicsLineItem*> m_xAxisTicks;
    QList<QGraphicsLineItem*> m_yAxisTicks;

    QGraphicsSimpleTextItem* m_labelHeartrate;

    QGraphicsPathItem* m_curve;
    QMap<int, int> m_data;
    QColor m_color;

    double m_xFactor;
    double m_yDelta;
    int m_heartrateSpacing;
    int m_minHeartrate;
    int m_maxHeartrate;
    double m_averageHeartrate;
};

#endif
