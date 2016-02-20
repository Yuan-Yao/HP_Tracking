//Fitter function comes from https://www.gnu.org/software/gsl/manual/html_node/
// Example-programs-for-Nonlinear-Least_002dSquares-Fitting.html
//Least-square fitting
#ifndef EXPONENTIAL_FITTER_H
#define EXPONENTIAL_FITTER_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>
#include <vector>
#include <qvector.h>
#include <qdebug.h>

struct data{
    size_t n;
    double * y;
    double * sigma;
};

class Exponential_Fitter
{
  private:
    const gsl_multifit_fdfsolver_type *T;
    gsl_multifit_fdfsolver *s;
    int status;
    unsigned int i, iter;
    const size_t n;
    const size_t p;
    gsl_matrix *covar;
    double *y, *sigma;
    data d;
    double *x_init;
    gsl_multifit_function_fdf f;
    gsl_vector_view x;
    const gsl_rng_type * type;
    gsl_rng * r;
    double A;
    double lambda;
    double b;
    void alternative_fit(QVector<double> time,QVector<double> amplitude);

  public:
    Exponential_Fitter(QVector<double> time, QVector<double> amplitude);
    double get_A() {return A;}
    double get_b() {return b;}
    double get_lambda() {return lambda;}

};

#endif // EXPONENTIAL_FITTER_H
