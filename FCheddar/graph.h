#ifndef GRAPH_H
#define GRAPH_H

#include <QtCharts>

class Graph
{
public:
    Graph(int, int, QVector<QString>, QVector<int>, QVector<int>, QVector<int>);
    ~Graph();

    void setTaskPen(QColor, int);
    void setActiveTaskPen(QColor, int);

    QChart* get_chart(void);

private:
    int task_num_;
    bool schedulable_;
    QVector<QLineSeries*> taskLines_;
    QVector<QLineSeries*> activeTaskLines_;
    int hyperperiod_;

    //Graph Attributes
    QChart *chart;
    QPen taskPen;
    QPen activeTaskPen;
    QValueAxis *axisX;
    QCategoryAxis *axisY;
};

#endif // GRAPH_H
