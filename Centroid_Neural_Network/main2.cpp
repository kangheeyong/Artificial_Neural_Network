#include "my_Centroid_Neural_Network.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int main(int argc, char **argv)
{
  MY_CNN a;

  if(argc != 4)
  {
    printf("Usage : %s [cluster number] [input text] [weight text]\n",argv[0]);
    return 0;
  }


  a.init(atoi(argv[1]));
  a.read_input(argv[2]);
  a.learning();
  a.status();
  a.write_weight(argv[3]);
  a.write_result("result.txt");
  a.write_setting("setting.txt");
  return 0;
}