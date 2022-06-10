#include "rtalgorithms.h"
#include <iostream>

RTAlgorithms::RTAlgorithms() {}

QVector<float> RTAlgorithms::get_taskUF(void) {
    return taskUF;
}

float RTAlgorithms::get_worst_case(void) {
    return worst_case;
}

float RTAlgorithms::get_uf(void) {
    return uf;
}

QVector<int> RTAlgorithms::get_time_demand(void) {
    return time_demand_vect;
}

QVector<bool> RTAlgorithms::get_is_valid(void) {
    return is_valid;
}

int RTAlgorithms::rms_utilization_factor(int n, QVector<int> execT_vect, QVector<int> period_vect) {
    worst_case = ((float)n * (std::pow(2.0, 1.0/(float)n) - 1.0));
    uf = 0.0;
    for(int i = 0; i < n; i++) {
        uf += ((float)execT_vect[i] / (float)period_vect[i]);
        taskUF.push_back(((float)execT_vect[i] / (float)period_vect[i]));
    }

    if(uf > 0 && uf < worst_case) {
        return 1;
    } else if(uf > worst_case && uf < 1) {
        return -1;
    } else {
        return 0;
    }
}

// Response time calculation
// w(t) = execT[i] + summatory((w(t)[i - 1] / period[j]) * execT[j])
// if (w <= period[i]) then w is_valid for i task
bool RTAlgorithms::rms_response_time(int num_tasks, QVector<int> execT_vect, QVector<int> period_vect) {
    int time_demand;  // w(t)[i]
    int iterator;
    int tmp_acc = 0;  // Temporary accumulator
    int previous_time_demand = 0;  // w(t)[i - 1]
    int task = 0;  // Actual task

    while(task < num_tasks) {
        iterator = 0;
        while(true) {
            time_demand = 0;
            tmp_acc = 0;

            if(!iterator) {
                for(int j = task - 1; j >= 0; j--) {
                    time_demand += execT_vect[j];
                }
                time_demand += execT_vect[task];
            } else {
                for(int j = task - 1; j >= 0; j--) {
                    tmp_acc += (std::ceil(((float)previous_time_demand / (float)period_vect[j])) * execT_vect[j]);
                }
                time_demand = execT_vect[task] + tmp_acc;
            }

            if(time_demand > period_vect[task]) {
                is_valid.push_back(false);
                time_demand_vect.push_back(time_demand);
                return false;
            } else if(time_demand == previous_time_demand) {
                is_valid.push_back(true);
                time_demand_vect.push_back(time_demand);
                break;
            }
            previous_time_demand = time_demand;
            iterator++;
        }
        task++;
    }
    return true;
}
