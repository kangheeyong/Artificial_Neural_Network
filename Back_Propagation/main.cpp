#include "my_Back_Propagtion.hpp"
#include <iostream>



using namespace std;

int main(int argc, char **argv)
{
  MY_BP a;

  a.read_setting("setting.txt");
  a.rand_weight();
//  a.status();
//  cout<<"-----------------"<<endl;
  a.read_input("train_test.txt");
  a.learning();
  a.status();
  a.write_error("error.txt");
   return 0;
}
