//
// Copyright 2017 by Philipp Jarvers
//

#include "curveData.h"

CurveData::CurveData(const QMap<int, int>& data, const QColor& color) :
  m_values(data),
  m_color(color)
{
}
