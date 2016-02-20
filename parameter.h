#ifndef PARAMETER_H
#define PARAMETER_H
#include <QString>
#include "math.h"
#include "qdebug.h"
#include "paragui.h"

struct Parameter
{
    int anz_Umdrehungen;
    int save_Gap;
    int save_Slots;
    int histo_anz;
    int threads;
    QString spannungs_Typ;
    QString ionen_Sorte;
    int anz_Teilchen;
    QString filter_Typ;
    double nukleonen_Energie;
    bool beschleunigung;
    bool coasting;
    double c;
    double e;
    double pi;
    double m_amu;
    int h;
    double freq_mod;
    double W_amu;
    double N_ionen;
    double m_Ion;
    double Q_ion_rel;
    double q;
    double m_0;

    //bunch-Anfangswerte
    float u_1base;
    float u_2base;
    float n_Ionen;
    double fuellgrad;

    //Ringparameter
    float gamma_t;
    float L;
    float R;
    int r;

    //Magnetfeldrampenzeiten
    float ramp_coast;
    float ramp_static;
    float ramp_round1_raw;
    float ramp_round2_raw;
    float ramp_round1;
    float ramp_raise;
    float ramp_round2;
    float BDot_max;

    //Rauschen
    bool noise;
    double noise_power;

    //Regler
    QString reglertyp;
    double gain;

    Parameter(ParaGUI &);
    void ausgabe_debug();

    //Störung
    int distortion_angle;
    int distortion_steps_coasting;
    int distortion_time_repeat;
    bool distortion_active;
    bool distortion_repeat_active;

    //Special Case: One particle
    bool single_particle;
};

#endif // PARAMETER_H
