#include "my_Back_Propagtion.hpp"
#include <iostream>
#include <stdio.h>


using namespace std;

int main(int argc, char **argv)
{
  MY_BP a;

  if(argc != 4)
  {
    printf("Usage : %s [setting text] [input text] [weight dir]\n",argv[0]);
    return 0;
  }


  a.read_setting(argv[1]);
  //a.read_weight(argv[1]);
  a.rand_weight();
  //   a.read_weight("weight");
  //  a.status();
  //  cout<<"-----------------"<<endl;
  a.read_input(argv[2]);
  a.learning();
  a.status();
  printf("success\n");
  a.mkdir_learning_result(argv[3]);
  // a.write_weight("weight");
  return 0;
}
