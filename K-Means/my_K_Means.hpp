#ifndef __MY_K_MEANS_CPP__
#define __MY_K_MEANS_CPP__

#define MAX_EPOCH     500


#include "my_data.hpp"

class K_MEANS{

private :
int cluster;
int epoch;
double MSE;
double min_error;

MY_DATA origin_maching;

MY_DATA weight, origin, setting;
MY_DATA first_weight,errors;


public :

void init(int cluster);
void status();
void read_input(const char *file_name);
void read_weight(const char *file_name);
void read_setting(const char *file_name);
void write_weight(const char *file_name);
void write_result(const char *file_name);
void write_error(const char *file_name);
void learning();
void testing();
void learning_for_comp_CNN();
K_MEANS();
~K_MEANS();


};




#endif
