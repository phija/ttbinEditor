//
// Copyright 2017 by Philipp Jarvers
//

#include "dataProcessor.h"

#include <QDebug>
#include <math.h>


DataProcessor::DataProcessor(double filter, double weight, int averageAmount) :
  m_filter(filter),
  m_weight(weight),
  m_averageAmount(averageAmount)
{
}


QMap<int, int> DataProcessor::processData(const QMap<int, int>& origin) const
{
  QMap<int, int> returnMap;
  QList<int> averageList;

  double averageOfLastN = -1.0;
  bool firstAmountReached = false;

  int j = 0;
  QMapIterator<int, int> mapIterator(origin);
  while (mapIterator.hasNext())
  {
    const int& value = mapIterator.next().value();

    averageList.push_back(value);

    if (averageList.size() >= m_averageAmount)
    {
      double averageValue = average(averageList);
      if (!firstAmountReached)
      {
        for (int i = 0; i < averageList.size(); ++i)
        {
          returnMap.insert(i, qRound(averageList[i] * (1.0 - m_weight) + averageValue * m_weight));
        }
        firstAmountReached = true;
      }
      else
      {
        if (fabs(value - averageValue) > m_filter)
        {
          returnMap.insert(j, qRound(averageValue));
        }
        else
        {
          returnMap.insert(j, qRound(value * (1.0 - m_weight) + averageValue * m_weight));
        }
      }
      averageList.pop_front();
    }
    j++;
  }

  return returnMap;
}

double DataProcessor::average(QList<int> data) const
{
  double sum = 0;
  for (int i = 0; i < data.size(); ++i)
  {
    sum += data[i];
  }
  return sum / (double)data.size();
}

