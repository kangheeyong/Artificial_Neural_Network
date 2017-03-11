#ifndef __MY_FUZZY_C_MEANS_CPP__
#define __MY_FUZZY_C_MEANS_CPP__

#define MAX_STEP    500

#include "my_data.hpp"

class MY_FCM{
private :
  MY_DATA origin,errors,weight,setting,center;
  int step;
  int cluster;
  double min_error;
  double gain;
  double standard_deviation;
  int input_dimension, input_set;
  int m;

public :
  MY_FCM();
  ~MY_FCM();
  void init();
  void status();
  void read_input(const char *file_name);
  void read_setting(const char *file_name);
  void write_weight(const char *file_name);
  void write_center(const char *file_name);
  void write_error(const char *file_name);
  void learning();
  void GBFCM_learning();
  void GBFCM_MK_learning();
};



#endif
