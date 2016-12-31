#ifndef __MY_INTERFACE_CPP__
#define __MY_INTERFACE_CPP__

#include <iostream>
#include <time.h>

#define PRINT_PER_EPOCH   5000
#define INPUT_DATA_PATH   "input_data\\"
#define OUTPUT_PATH   "output_data/"
#define OUTPUT_DATA_PATH   "output_data\\"

using namespace std;

class MY_INTERFACE
{
private :
  char fname[50][250];
  int fname_limit;
  char recent_name[100];
  char recent_dir[100];
  char buff[100];
  char weight_name[100];


  clock_t start_point, end_point;

public :


  void search_file(char* path);
  char* sellect_file(int num);
  char* sellect_dir(int num);
  char* get_recent_name();
  char* get_recent_dir();
  char* get_recent_weight();

  void print_menu();
  void print_training();
  void print_mapping();
  void print_setting();
  void print_file();

  void timer(int now, int total); //data의 row단위로 섞는다.
  MY_INTERFACE();
  ~MY_INTERFACE();
};



#endif
