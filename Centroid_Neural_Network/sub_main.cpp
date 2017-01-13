#include "sub_main.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include "my_Centroid_Neural_Network.hpp"


using namespace std;

static char key_buff[255];
static int cluster = 4;
static string weight_path,setting_path,result_path;

void traing_random_weight(char* input_name,char* output_name)
{
  

    weight_path = output_name;
    setting_path = output_name;

    weight_path.append("weight.txt");
    setting_path.append("setting.txt");
    MY_CNN a;

    a.init(cluster);
    a.read_input(input_name);

    printf("1. CNN\n2.WCNN\n");
    int num;
    cin>>num;

    if(num  == 1) a.learning();
    else a.WCNN_learning();
    
    
    a.write_weight(weight_path.c_str());
    a.write_setting(setting_path.c_str());
    a.status();




    printf("press any key\n");
    scanf("%s",key_buff);


}
void traing_sellected_weight(char* input_name,char* output_name,char* weight_name)
{
  printf("default\n");
  printf("press any key\n");
  scanf("%s",key_buff);
}
void mapping(char* input_name,char* output_name,char* weight_name)
{ 
    result_path = output_name;

    result_path.append("result.txt");
    printf("mapping input path : %s\n",input_name);
    printf("mapping in weigh path: %s\n",weight_name);
    printf("mapping out result path: ");

    MY_CNN a;
    cout<<result_path<<endl;
    a.read_input(input_name);
    a.read_weight(weight_name);
    a.testing();

    printf("press any key\n");
    scanf("%s",key_buff);
}
void setting()
{
  cout<<"cluster : ";
  cin>>cluster;

}
