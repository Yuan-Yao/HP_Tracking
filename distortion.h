#ifndef DISTORTION_H
#define DISTORTION_H
#include "bunch.h"
#include <vector>
#include <qdebug.h>
#include "parameter.h"
#include <random>
#include <log_ref_i.h>

class Distortion
{
  private:
    const double distortion_strength; //distortion-angle in rad
    const double distortion_time_intervall; //distortion in turns, maybe better if it would be in time intervals
    const bool homogen; //homogen, or random distortions on all buckets?
    const bool fda; //frequency damp analyser configuration?
    double distortion_offset; //offset, when first distortion should take effect, there should be no distortion during coasting
    double distortion_end; //end, when distortion should be ended.
    std::vector<double> distortion_vector; //when there are more bucket, every bucket can be adressed individually
    std::vector<double> random_distortion; //when there are more buckets, and each bucket is to be randomly distorted
    bool active; //distortion generally on?
    bool active_repeat; //repeated distortion?

    void distort_homogen(Parameter & para, log_ref_i & lri, std::vector<Bunch> & bunch); //maximum distortion in all buckets
    void distort_random(Parameter & para, log_ref_i & lri, std::vector<Bunch> & bunch); //random distortion in each bucket
    void distort_homogen_fda(log_ref_i & lri, std::vector<Bunch> & bunch); //distortion only frequency damp analyzer, single bunch only, otherwise error_message

  public:
    Distortion(Parameter & para, log_ref_i & lri,bool homogen, bool fda); //constructor
    void process_distortion(Parameter & para, log_ref_i & lri, std::vector<Bunch> & bunch); //decides which function to use
    };

#endif // DISTORTION_H
