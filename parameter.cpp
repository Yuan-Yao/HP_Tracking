#include "parameter.h"

//The following reads parameter values from the ParaGUI
Parameter::Parameter(ParaGUI & paraGUI) :
  anz_Umdrehungen(paraGUI.get_anz_um()),
  save_Gap(paraGUI.get_save_gap()),
  save_Slots(anz_Umdrehungen/save_Gap),
  histo_anz(paraGUI.get_hist_anz()),
  threads(paraGUI.get_anz_threads()),
  spannungs_Typ((paraGUI.get_spannung() == "einfachharmonisch") ? "eh" : "dh"),
  ionen_Sorte(paraGUI.get_ion_sort()),
  anz_Teilchen(paraGUI.get_anz_teil()),
  filter_Typ(paraGUI.get_regler()),
  nukleonen_Energie(paraGUI.get_nukl_ener()),
  beschleunigung(paraGUI.get_acc()),
  coasting(paraGUI.get_coasting()),
  c(299792458.0),
  e(1.602176487e-19),
  pi(3.1415926535),
  m_amu(1.660538921e-27),
  h(paraGUI.get_harmonic_number()),
  freq_mod(paraGUI.get_freq_mod()),
  W_amu(m_amu*pow(c,2.0)),
  N_ionen(2.5*pow(10,11)),
  u_1base(paraGUI.get_u_1()),
  u_2base(-500),
  n_Ionen(2.5e11),
  fuellgrad(paraGUI.get_fuellgrad()),
  gamma_t(5.45),
  L(216.72),
  R(L/(2*pi)),
  r(10),
  ramp_coast(0.04),
  ramp_static(0.1),
  ramp_round1_raw(0.05),
  ramp_round2_raw(0.05),
  ramp_round1(ramp_static + ramp_round1_raw),
  ramp_raise(ramp_round1 +0.65), // nachjustieren
  ramp_round2(ramp_raise + ramp_round2_raw), //auch nachjustieren, wenn der Beam von Coasting in Bunch übergegangen ist
  BDot_max(4),
  noise(paraGUI.get_noise()),
  noise_power(paraGUI.get_noise_power()),
  reglertyp(paraGUI.get_regler()),
  gain(paraGUI.get_FIR_gain()),
  distortion_angle(paraGUI.get_distortion_angle()),
  distortion_steps_coasting(paraGUI.get_distortion_steps_coasting()),
  distortion_time_repeat(paraGUI.get_distortion_time_repeat()),
  distortion_active(paraGUI.get_distortion_active()),
  distortion_repeat_active(paraGUI.get_distortion_repeat_active()),
  single_particle(paraGUI.get_single_particle())
{
  if (ionen_Sorte == "Uran 73+"){
    m_Ion = 238.02891;
    Q_ion_rel = 73;
  }
  else if (ionen_Sorte == "Uran 28+"){
    m_Ion = 238.02891;
    Q_ion_rel = 28;
  }
  else if (ionen_Sorte == "Proton 1+"){
    m_Ion = 1.0079;
    Q_ion_rel = 1;
  }
  else if (ionen_Sorte == "Argon 18+"){
    m_Ion = 39.948;
    Q_ion_rel = 18;
  }
  else if (ionen_Sorte == "Neon 10+"){
    m_Ion = 20.1797;
    Q_ion_rel = 10;
  }
  else if (ionen_Sorte == "Nickel 26+"){
    m_Ion = 58.6934;
    Q_ion_rel = 26;
  }
  else if (ionen_Sorte == "86Krypton 33+"){
    m_Ion = 85.91061073;
    Q_ion_rel = 33;
  }
  else if (ionen_Sorte == "86Krypton 34+"){
    m_Ion = 85.91061073;
    Q_ion_rel = 34;
  }
  else if (ionen_Sorte == "Bismut 67+"){
    m_Ion = 208.98038;
    Q_ion_rel = 67;
  }
  q = Q_ion_rel*e;
  m_0 = m_amu*m_Ion;

  if (single_particle == true){
    threads = 1;
    anz_Teilchen = 1;
  }
}

void Parameter::ausgabe_debug(){
  qDebug() << "anz_Umdrehungen, " << anz_Umdrehungen
                  << "\n spannungs_Typ, " << spannungs_Typ
                  << "\n ionen_Sorte, " << ionen_Sorte
                  << "\n anz_Teilchen, " << anz_Teilchen
                  << "\n filter_Typ, " << filter_Typ
                  << "\n nukleonen_Energie, " << nukleonen_Energie
                  << "\n save_Gap, " << save_Gap
                  << "\n beschleunigung, " << beschleunigung
                  << "\n coasting, " << coasting
        << "\n c, " << c
        << "\n e, " << e
        << "\n pi, " << pi
        << "\n m_amu, " << m_amu
        << "\n h, " << h
        << "\n W_amu, " << W_amu
        << "\n m_Ion, " << m_Ion
        << "\n Q_ion_rel, " << Q_ion_rel
        << "\n q, " << q
        << "\n m_0, " << m_0
        << "\n u_1base, " << u_1base
        << "\n u_2base, " << u_2base
        << "\n n_Ionen, " << n_Ionen
        << "\n gamma_t, " << gamma_t
        << "\n L, " << L
        << "\n R, " << R
        << "\n r, " << r
        << "\n threads" << threads   ;

}
