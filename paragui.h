#ifndef PARAGUI_H
#define PARAGUI_H

#include <QWidget>
#include <QString>

namespace Ui {
class ParaGUI;
}

class ParaGUI : public QWidget
{
    Q_OBJECT

  public:
    explicit ParaGUI(QWidget *parent = 0);
    ~ParaGUI();
    int get_anz_um(); //number of turns
    int get_anz_teil(); //number of particles per thread
    int get_anz_threads(); //number of threads, depends on the maximum virtual threads a machine hast
    int get_save_gap(); //every save_gaps turn is a save
    int get_hist_anz(); //length of a histogramm, preferably 2^n, because its best for fft
    double get_nukl_ener(); //initial energy of every nucleon
    double get_fuellgrad(); //filling size of bucket, 3rd standard deviation is at the value of the bucket, 1 is 100%
    QString get_spannung(); //initial static voltage
    QString get_regler(); //type of controller
    QString get_ion_sort(); //ion sort
    bool get_coasting(); //with coasting?
    bool get_acc(); //with acceleration?
    bool get_noise(); //with noise on histogram?
    double get_noise_power(); //noise power on histogram. percentage compared to equally distributed over histogramm
    double get_FIR_gain(); //FIR-gain, only makes sense for stationary case, how to calculate
    double get_u_1(); //get initial voltage in stationary case
    int get_distortion_angle(); //distortion in °
    int get_distortion_steps_coasting(); //first distortion starts after amount of steps
    bool get_distortion_active(); //with distortion?
    bool get_distortion_repeat_active(); //with repeated distortion?
    int get_distortion_time_repeat(); //steps between repeated distortion
    int get_harmonic_number(); //harmonic number
    double get_freq_mod(); //frequency_modifier
    bool get_single_particle(); //check if single particle mode

    void set_anz_um(int i);
    void set_anz_teil(int i);
    void set_anz_threads(int i);
    void set_save_gap(int i);
    void set_hist_anz(int i);
    void set_nukl_ener(double d);
    void set_fuellgrad(double d);
    void set_spannung(QString s);
    void set_regler(QString s);
    void set_ion_sort(QString s);
    void set_coasting(bool b);
    void set_acc(bool b);
    void set_noise(bool b);
    void set_noise_power(double d);
    void set_FIR_gain(double d);
    void set_u_1(double d);
    void set_distortion_angle(int i);
    void set_distortion_steps_coasting(int i);
    void set_distortion_active(bool b);
    void set_distortion_repeat_active(bool b);
    void set_distortion_time_repeat(int i);
    void set_harmonic_number(int i);
    void set_freq_mod(double d);
    void set_single_particle(bool b);

  private:
    Ui::ParaGUI *ui;
};

#endif // PARAGUI_H
