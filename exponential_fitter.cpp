//Fitter function comes from https://www.gnu.org/software/gsl/manual/html_node/
// Example-programs-for-Nonlinear-Least_002dSquares-Fitting.html
//Least-square fitting
#include "exponential_fitter.h"

//
//expb_f
//
int expb_f(const gsl_vector * x, void * data, gsl_vector * f){
  size_t n = ((struct data *) data)->n;
  double *y = ((struct data *) data)->y;
  double *sigma = ((struct data *) data)->sigma;

  double A = gsl_vector_get (x,0);
  double lambda = gsl_vector_get (x,1);
  double b = gsl_vector_get (x,2);

  size_t i;

  for (i = 0; i<n; i++){
    double t = i;
    double Yi = A * exp (-lambda * t) + b;
    gsl_vector_set (f,i,(Yi-y[i])/sigma[i]);
  }

  return GSL_SUCCESS;
}

//
//expb_df
//
int expb_df(const gsl_vector * x, void * data, gsl_matrix * J){
  size_t n = ((struct data*) data)->n;
  double *sigma = ((struct data *) data)->sigma;

  double A = gsl_vector_get (x, 0);
  double lambda = gsl_vector_get (x, 1);

  size_t i;

  for (i = 0; i < n; i++){
    double t = i;
    double s = sigma[i];
    double e = exp(-lambda * t);
    gsl_matrix_set(J,i,0,e/s);
    gsl_matrix_set(J,i,1,-t * A * e/s);
    gsl_matrix_set(J,i,2,1/s);
  }
  return GSL_SUCCESS;
}

//
//expb_fdf
//
int expb_fdf(const gsl_vector * x, void *data, gsl_vector * f, gsl_matrix * J){
  expb_f (x,data,f);
  expb_df (x,data,J);

  return GSL_SUCCESS;
}

void print_state(size_t iter, gsl_multifit_fdfsolver * s);

Exponential_Fitter::Exponential_Fitter(QVector<double> time, QVector<double> amplitude):
  n(amplitude.size()),
  p(3),
  type(gsl_rng_default),
  A(0),
  b(0),
  lambda(0)
{
  iter = 0;
  covar = gsl_matrix_alloc (p, p);
  y = new double[n];
  sigma = new double[n];
  d = {n,y,sigma};
  x_init = new double[3];
  x_init[0] = amplitude[0];
  x_init[1] = 0.1;
  x_init[2] = 0.01;
  x = gsl_vector_view_array(x_init,p);
  gsl_rng_env_setup();
  r = gsl_rng_alloc (type);

  f.f = &expb_f;
  f.df = &expb_df;
  f.fdf = &expb_fdf;
  f.n = n;
  f.p = p;
  f.params = &d;

  //insert data
  for (i = 0; i < n; i++){
    y[i] = amplitude[i];
    sigma[i] = 0.0001;
  }
  T = gsl_multifit_fdfsolver_lmsder;
  s = gsl_multifit_fdfsolver_alloc(T,n,p);
  gsl_multifit_fdfsolver_set(s, &f, &x.vector);

  do{
    iter ++;
    status = gsl_multifit_fdfsolver_iterate(s);

    if (status)
      break;

    status = gsl_multifit_test_delta (s->dx, s->x,1e-8, 1e-8);
  }
  while (status == GSL_CONTINUE && iter < 500);

  qDebug() << "Schritt 1";
  gsl_multifit_covar (s->J, 0.0, covar);

#define FIT(i) gsl_vector_get(s->x, i)
#define ERR(i) sqrt(gsl_matrix_get(covar,i,i))

  {
    double chi = gsl_blas_dnrm2(s->f);
    double dof = n - p;
    double c = GSL_MAX_DBL(1, chi / sqrt(dof));

    qDebug() << "A" <<  FIT(0) << c*ERR(0);
    qDebug() << "lambda" <<  FIT(1) << c*ERR(1);
    qDebug() << "b" <<  FIT(2) << c*ERR(2);
    A = FIT(0);
    b = FIT(2);
    lambda = FIT(1);
  }
  gsl_multifit_fdfsolver_free (s);
  gsl_matrix_free (covar);
  gsl_rng_free (r);
}

void Exponential_Fitter::alternative_fit(QVector<double> time_, QVector<double> amplitude_){
  QVector<double> time_to0 = QVector<double>(0);
  QVector<double> amplitude_log = QVector<double>(0);
  for (int i = 0; i < time_.size(); i++){
    time_to0.push_back(time_[i]-time_[0]);
    amplitude_log.push_back(1);
  }
}
