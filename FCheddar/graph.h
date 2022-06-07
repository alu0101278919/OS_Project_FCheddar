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

    bool rms(void);

private:
    int task_num_;
    bool schedulable_;
    QVector<QLineSeries*> taskLines_;
    QVector<QVector<QLineSeries*> > activeTaskLines_;
    int hyperperiod_;
    QVector<QString> name_vect_;
    QVector<int> arrivalT_vect_;
    QVector<int> period_vect_;
    QVector<int> execT_vect_;
    QVector<QVector<int> > arrival_period_vect;

    //Graph Attributes
    QChart *chart;
    QPen taskPen;
    QPen activeTaskPen;
    QValueAxis *axisX;
    QCategoryAxis *axisY;
};

#endif // GRAPH_H
