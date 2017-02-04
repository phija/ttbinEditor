//
// Copyright 2017 by Philipp Jarvers
//

#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <QMap>

class DataProcessor
{
  public:
    DataProcessor(double filter, double weight, int averageAmount);
    virtual ~DataProcessor() {};

    QMap<int, int> processData(const QMap<int, int>&) const;

    void setWeight(double weight) { m_weight = weight; }
    void setAverage(int average) { m_averageAmount = average; }
    void setFilter(int filter) { m_filter = filter; }

  private:
    double average(QList<int> data) const;

    double m_filter;
    double m_weight;
    double m_averageAmount;
};

#endif
