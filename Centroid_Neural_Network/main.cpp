#include "my_interface.hpp"
#include "sub_main.hpp"
#include <stdio.h>
#include <stdlib.h>

int input_num()
{
  cout<<endl<<"choose the number"<<endl;
  int num;
  cin>>num;
  return num;
}

MY_INTERFACE my_ui;

int main()
{

  int sel = 0;


  while(sel < 4)
  {
    system("clear");
    my_ui.print_menu();
    sel = input_num();

    switch(sel){
      case 1 :

      my_ui.search_file("input_data");
      my_ui.print_file();
      printf("choose input data\n");
      my_ui.sellect_file(input_num());
      my_ui.print_training();
      sel = input_num();
      switch(sel){
        case 1 :
          traing_random_weight(my_ui.get_recent_name(),my_ui.get_recent_dir());
        break;
        case 2 :
        my_ui.search_file("output_data");
        my_ui.print_file();
        printf("choose weight data\n");
        my_ui.sellect_dir(input_num());
        traing_sellected_weight(my_ui.get_recent_name(),my_ui.get_recent_dir(),my_ui.get_recent_weight());
        break;

        default :
        break;
      }
      break;
      case 2 :

      my_ui.search_file("input_data");
      my_ui.print_file();
      printf("choose input data\n");
      my_ui.sellect_file(input_num());

      my_ui.search_file("output_data");
      my_ui.print_file();
      printf("choose weight data\n");
      my_ui.sellect_dir(input_num());
      mapping(my_ui.get_recent_name(),my_ui.get_recent_dir(),my_ui.get_recent_weight());

      break;

      case 3 :
      setting();
      break;

      default :
      printf("good bey~~\n");
      break;
    }
  }

  return 0;
}
