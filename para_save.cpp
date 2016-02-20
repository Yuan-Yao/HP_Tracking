#include "para_save.h"

//constructor checks if a parameter file already exists, otherwise it creates one,
//and opens it
para_save::para_save()
{
  file = new QFile();
  filename = "parameter_init.txt";
  file->setFileName(filename);
}

void para_save::save(ParaGUI &paragui)
{
  file->open(QIODevice::WriteOnly);
  textStream = new QTextStream(file);
  *textStream << "1 Anzahl Umdrehungen\n"
              << paragui.get_anz_um() << "\n"
              << "2 Anzahl Teilchen\n"
              << paragui.get_anz_teil() << "\n"
              << "\t:3 Threads\n"
              << paragui.get_anz_threads() << "\n"
              << "\t:4 Anzahl Histogrammbalken\n"
              << paragui.get_hist_anz() << "\n"
              << "\t:5 Nukleonenenergie\n"
              << paragui.get_nukl_ener() << "\n"
              << "\t:6 Save_gap\n"
              << paragui.get_save_gap() << "\n"
              << "\t:7 Spannungstyp\n"
              << paragui.get_spannung() << "\n"
              << "\t:8 Ionensorte\n"
              << paragui.get_ion_sort() << "\n"
              << "\t:9 Reglertyp\n"
              << paragui.get_regler() << "\n"
              << "\t:10 Coasting\n"
              << paragui.get_coasting() << "\n"
              << "\t:11 Fuellgrad\n"
              << paragui.get_fuellgrad() << "\n"
              << "\t:12 Beschleunigung\n"
              << paragui.get_acc() << "\n"
              << "\t:13 Rauschen aktiv\n"
              << paragui.get_noise() << "\n"
              << "\t:14 Rauschstaerke\n"
              << paragui.get_noise_power() << "\n"
              << "\t:15 FIR-Gain\n"
              << paragui.get_FIR_gain() << "\n"
              << "\t:16 Grundspannung\n"
              << paragui.get_u_1() << "\n"
              << "\t:17 Anzahl Bunches im Ring\n"
              << paragui.get_harmonic_number() << "\n"
              << "\t:18 Frequenzmodifizierer\n"
              << paragui.get_freq_mod() << "\n"
              << "\t:19 Einteilchenmodus\n"
              << paragui.get_single_particle() << "\n"
              << "\t:20 Stoerungsauslenkung\n"
              << paragui.get_distortion_angle() << "\n"
              << "\t:21 Stoerung nach Coasting\n"
              << paragui.get_distortion_steps_coasting() << "\n"
              << "\t:22 Stoerung aktiv\n"
              << paragui.get_distortion_active() << "\n"
              << "\t:23 Wiederholung von Stoerungen nach Schritten\n"
              << paragui.get_distortion_time_repeat() << "\n"
              << "\t:24 Wiederholung der Stoerung.\n"
              << paragui.get_distortion_repeat_active() << "\n";
  file->close();
}

void para_save::load(ParaGUI &paragui)
{
  if (file->exists()){
    file->open(QIODevice::ReadOnly);
    textStream = new QTextStream(file);
    QString line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_anz_um(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_anz_teil(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_anz_threads(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_hist_anz(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_nukl_ener(line.toDouble(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_save_gap(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_spannung(line);
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_ion_sort(line);
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_regler(line);
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_coasting((line == "1")? true : false);
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_fuellgrad(line.toDouble(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_acc((line == "1")? true : false);
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_noise((line == "1")? true : false);
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_noise_power(line.toDouble(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_FIR_gain(line.toDouble(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_u_1(line.toDouble(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_harmonic_number(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_freq_mod(line.toDouble(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_single_particle((line == "1")? true : false);
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_distortion_angle(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_distortion_steps_coasting(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_distortion_active((line == "1") ? true : false);
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_distortion_time_repeat(line.toInt(0));
    line = textStream->readLine(); line = textStream->readLine();qDebug() << line.toInt(0); paragui.set_distortion_repeat_active((line == "1")? true : false);
  file->close();
  }
}

bool para_save::al_exist(){
  if (file->exists()) {return true;}
  else {return false;}
}
