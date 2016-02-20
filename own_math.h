#ifndef OWN_MATH_H
#define OWN_MATH_H
#include <vector>
#include <math.h>
#include <qdebug.h>

namespace own_math2 {
    std::vector<double> reduc_vec(std::vector<double> & iv,int begin,int end);
    double mean(std::vector<double> & input_vector);
    double standard_deviation(std::vector<double> & input_vector);
    double DFT_1_rad(std::vector<double> & input_vector);
    double DFT_1_phase(std::vector<double> & input_vector);

    std::vector<double> energy(std::vector<double> & input_vector, double frequency, double Ts);
}

#endif // OWN_MATH_H
