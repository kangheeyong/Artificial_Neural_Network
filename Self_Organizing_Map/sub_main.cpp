#include "sub_main.hpp"
#include "my_data.hpp"
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


void traing_random_weight(char* input_name,char* output_name)
{
  printf("traing_random_weight in : %s\n",input_name);
  printf("traing_random_weight out : %s\n",output_name);

  MY_DATA origin;
  origin.fread(input_name);
  //origin.print();



  MY_DATA weight,setting;
  weight.init(cluster,origin.get_row());
  weight.random(weight_range_min, weight_range_max);
  weight.print();
  setting.init(4,1);

  char buff[255];
  strcpy(buff,output_name);
  strcat(buff,"weight_before.txt");
  weight.fwrite(buff);

  int epoch = 0;
  int winner_num;
  double temp;
  double min;

  cout<<endl;



  while(epoch < total)
  {
    origin.timer(epoch, total);

    if(epoch % 100 == 0)  origin.suffle();

    for(int k = 0 ; k < origin.get_column() ; k++)
    {
      for(int i = 0 ; i < cluster ; i++)
      {
        temp = 0.0;
        for(int j = 0 ; j < origin.get_row() ; j++)
        {
          temp = temp + (origin(k,j) - weight(i,j))*(origin(k,j) - weight(i,j));
        }
        if(i == 0 )
        {
          min = temp;
          winner_num = i;
        }
        else
        {
          if(min > temp)
          {
            min = temp;
            winner_num = i;
          }
        }
      }
      double distance_temp = distance_gain*(1.0 - (double)epoch/total);


      distance_temp = distance_temp*distance_temp;

      for(int i = 0 ; i < cluster ; i++)
      {
       
        if(i != winner_num)
        {
          temp = 0.0;
          for(int j = 0 ; j < origin.get_row() ; j++)
          {
            temp = temp + (weight(winner_num,j) - weight(i,j))*(weight(winner_num,j) - weight(i,j));
          }

          if(distance_temp >= temp )
          {
            for(int j = 0 ; j < origin.get_row() ; j++)
            {
              weight(i,j) = weight(i,j) + gain*(1 - (double)epoch/total)*(origin(k,j) - weight(i,j));
            }   
          }
        }
      }
      for(int j = 0 ; j < origin.get_row() ; j++)
      {
        weight(winner_num,j) = weight(winner_num,j) + gain*(1 - (double)epoch/total)*(origin(k,j) - weight(winner_num,j));
      } 
    }
    epoch++;
  }

  weight.print();
  strcpy(buff,output_name);
  strcat(buff,"weight_after.txt");
  weight.fwrite(buff);
  setting(0,0) = cluster;
  setting(1,0) = total;
  setting(2,0) = gain;
  setting(3,0) = distance_gain;
  strcpy(buff,output_name);
  strcat(buff,"setting.txt");
  setting.fwrite(buff);

}
void traing_sellected_weight(char* input_name,char* output_name,char* weight_name)
{
  printf("traing_sellected_weight in : %s\n",input_name);
  printf("traing_sellected_weight out : %s\n",output_name);
  printf("traing_sellected_weight weight : %s\n",weight_name);
}
void mapping(char* input_name,char* output_name,char* weight_name)
{
  printf("mapping in : %s\n",input_name);
  printf("mapping : %s\n",output_name);
  printf("mapping : %s\n",weight_name);
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
