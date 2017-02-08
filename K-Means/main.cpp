#include "my_K_Means.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int main(int argc, char **argv)
{
  K_MEANS a;

  if(argc != 4)
  {
    printf("Usage : %s [setting txt] [input text] [weight text]\n",argv[0]);
    return 0;
  }


  a.read_setting(argv[1]);
  a.read_input(argv[2]);
  a.learning();
  a.status();
  a.write_weight(argv[3]);
  a.write_result("result.txt");
  a.write_error("error.txt");
  return 0;
}
