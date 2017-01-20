//
// Copyright 2017 by Philipp Jarvers
//

#ifndef SRC_CURVEDATA_H
#define SRC_CURVEDATA_H

#include <QMap>
#include <QColor>

class CurveData
{
  public:
    CurveData(const QMap<int, int>& data, const QColor& color);

    QMap<int, int> m_values;
    QColor m_color;
};

#endif
