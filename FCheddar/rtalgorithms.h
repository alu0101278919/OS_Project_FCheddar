#ifndef RTALGORITHMS_H
#define RTALGORITHMS_H
#include <QVector>
#include <math.h>


class RTAlgorithms
{
public:
    RTAlgorithms();
    QVector<float> get_taskUF(void);
    float get_worst_case(void);
    float get_uf(void);
    QVector<int> get_time_demand(void);
    QVector<bool> get_is_valid(void);

    int rms_utilization_factor(int, QVector<int>, QVector<int>);
    bool rms_response_time(int, QVector<int>, QVector<int>);
private:
    QVector<float> taskUF;
    QVector<int> time_demand_vect;  // time_demand value for tasks
    QVector<bool> is_valid;  // Valid time_demand value for tasks
    float worst_case;
    float uf;
};

#endif // RTALGORITHMS_H
