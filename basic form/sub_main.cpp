#include "sub_main.hpp"
#include <stdio.h>

static char key_buff[255];

void traing_random_weight(char* input_name,char* output_name)
{
  printf("traing_random_weight input path : %s\n",input_name);
  printf("traing_random_weight out weight path : %s\n",output_name);
  
  printf("press any key\n");
  scanf("%s",key_buff);
    

}
void traing_sellected_weight(char* input_name,char* output_name,char* weight_name)
{
  printf("traing_sellected_weight input path : %s\n",input_name);
  printf("traing_sellected_weight out weight path : %s\n",output_name);
  printf("traing_sellected_weight in weight path: %s\n",weight_name);

  printf("press any key\n");
  scanf("%s",key_buff);
}
void mapping(char* input_name,char* output_name,char* weight_name)
{
  printf("mapping input path : %s\n",input_name);
  printf("mapping out result path: %s\n",output_name);
  printf("mapping in weigh path: %s\n",weight_name);

  printf("press any key\n");
  scanf("%s",key_buff);
}
void setting()
{
  printf("setting\n");


  printf("press any key\n");
  scanf("%s",key_buff);
}
