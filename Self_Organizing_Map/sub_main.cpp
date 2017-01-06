#include "sub_main.hpp"
#include "my_Self_Organizing_Map.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

static int cluster = 4;
static int  total = 50000;
static double gain = 0.2;
static double distance_gain = 0.02;
static double weight_range_min = 0.35;
static double weight_range_max = 0.45;
static string weight_path,setting_path,result_path;
static char key_buff[255];

void traing_random_weight(char* input_name,char* output_name)
{


    weight_path = output_name;
    setting_path = output_name;

    weight_path.append("weight.txt");
    setting_path.append("setting.txt");
    MY_SOM a;
    a.init(cluster,total,gain, distance_gain,weight_range_min,weight_range_max);
    a.read_input(input_name);
    a.learning();
    a.write_weight(weight_path.c_str());
    a.write_setting(setting_path.c_str());
    a.status();


    printf("press any key\n");
    scanf("%s",key_buff);



}
void traing_sellected_weight(char* input_name,char* output_name,char* weight_name)
{
  printf("traing_sellected_weight in : %s\n",input_name);
  printf("traing_sellected_weight out : %s\n",output_name);
  printf("traing_sellected_weight weight : %s\n",weight_name);
}
void mapping(char* input_name,char* output_name,char* weight_name)
{
    result_path = output_name;

    result_path.append("result.txt");
    printf("mapping input path : %s\n",input_name);
    printf("mapping in weigh path: %s\n",weight_name);
    printf("mapping out result path: ");

    MY_SOM a;
    cout<<result_path<<endl;
    a.read_input(input_name);
    a.read_weight(weight_name);
    a.testing();
    a.write_result(result_path.c_str());

    printf("press any key\n");
    scanf("%s",key_buff);
}
void setting()
{

  printf("setting\n");
  cout<<"cluster : "<<cluster<<endl;
  cout<<"max epoch : "<<total<<endl;
  cout<<"gain : "<<gain<<endl;
  cout<<"distance_gain : "<<distance_gain<<endl;
  cout<<"weight_range_min : "<<weight_range_min<<endl;
  cout<<"weight_range_max : "<<weight_range_max<<endl;
  int sel = 1;
  cout<<"change?? yes(1) or no(2) : ";
  cin>>sel;
  if(sel == 1)
  {
    cout<<"cluster : ";
    cin>>cluster;
    cout<<"max epoch : ";
    cin>>total;
    cout<<"gain : ";
    cin>>gain;
    cout<<"distance gain : ";
    cin>>distance_gain;
    cout<<"weight_range_min : ";
    cin>>weight_range_min;
    cout<<"weight_range_max : ";
    cin>>weight_range_max;
  }
}
