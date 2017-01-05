#ifndef __MY_CENTROID_NEURAL_NETWORK_CPP__
#define __MY_CENTROID_NEURAL_NETWORK_CPP__
#define PRINT     cout


#include "my_data.hpp"

class MY_CNN{

private :
bool read_status,learning_status;
int cluster;
int epoch;
double MSE;
MY_DATA weight, origin, setting;
MY_DATA origin_maching;



public :

void init(int cluster);
void status();
void read_input(char *file_name);
void read_weight(char *file_name);
void write_weight(char *file_name);
void write_setting(char *file_name);
void write_result(char *file_name);
void learning();
void testing();


MY_CNN();
~MY_CNN();


};



#endif
