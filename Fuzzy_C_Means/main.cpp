#include "my_Fuzzy_C_Means.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int main(int argc, char **argv)
{
  MY_FCM a;

  if(argc != 4)
  {
    printf("Usage : %s [setting txt] [input text] [weight text]\n",argv[0]);
    return 0;
  }


  a.read_setting(argv[1]);
  a.read_input(argv[2]);
  a.GBFCM_learning();
  a.status();
  a.write_error("error.txt");
  a.write_weight(argv[3]);
  return 0;
}
