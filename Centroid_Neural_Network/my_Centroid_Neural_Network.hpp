#ifndef __MY_CENTROID_NEURAL_NETWORK_CPP__
#define __MY_CENTROID_NEURAL_NETWORK_CPP__
#define MAX_EPOCH       500000


#include "my_data.hpp"

class MY_CNN{

private :
bool read_status,learning_status;
int cluster;
int epoch;
double MSE;
MY_DATA weight, origin, setting;
MY_DATA origin_maching;
MY_DATA error;
int centroid_k;
MY_DATA variance,wcnn_weight;
MY_DATA mean;


public :

void init(int cluster);
void status();
void read_input(const char *file_name);
void read_weight(const char *file_name);
void write_weight(const char *file_name);
void write_error(const char *file_name);
void write_setting(const char *file_name);
void write_result(const char *file_name);
void learning();
void WCNN_learning();
void DCNN_learning();
void DCNN_pibonachi_learning();
void pibonachi_learning();
void pibonachi2_learning();
void temp_learning();
void testing();
double get_percent();

MY_CNN();
~MY_CNN();


};



#endif
