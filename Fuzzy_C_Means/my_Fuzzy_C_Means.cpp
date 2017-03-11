#include "my_Fuzzy_C_Means.hpp" 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


MY_FCM :: MY_FCM()
{
  gain = 0.4;
}

MY_FCM :: ~MY_FCM()
{

}

void MY_FCM :: init()
{
}

void MY_FCM :: status()
{
  printf("cluster : %d\n",cluster);
  printf("min_error : %lf\n",min_error);
  printf("m : %d\n",m);
  if(setting.get_row() >= 4 )printf("gain : %lf\n",gain);
  if(setting.get_row() >= 5 )printf("standard_deviation : %lf\n",standard_deviation);
  printf("step : %d\n",step);
  if(step > 1 )printf("final errer : %lf\n",errors(step-1));
  printf("----- center -----");
  center.print();
  printf("----- weight -----");
  weight.print();
//  printf("----- errors -----");
//  errors.print();i
  printf("----- origin -----");
  origin.print();

}

void MY_FCM :: read_input(const char *file_name)
{
  origin.fread(file_name);
}

void MY_FCM :: read_setting(const char *file_name)
{
  setting.fread(file_name);

 
  cluster = setting(0);
  min_error =setting(1);
  m = setting(2);

  if(setting.get_row() >= 4) gain = setting(3);
  if(setting.get_row() >= 5) standard_deviation = setting(4);
}

void MY_FCM ::  write_weight(const char *file_name)
{
  weight.fwrite(file_name);
}
void MY_FCM :: write_center(const char *file_name)
{
  center.fwrite(file_name);
}
void MY_FCM ::  write_error(const char *file_name)
{
  errors.fwrite(file_name);
}

void MY_FCM ::  learning()
{

  input_dimension = origin.get_row();
  input_set = origin.get_column();

  weight.init(input_set,cluster);
  weight.random(0,1);

  center.init(cluster,input_dimension);

  MY_DATA e;

  e.init(MAX_STEP);

  step = 0;

  MY_DATA weight_before;

  while(1)
  {
    step++;
    for(int i = 0 ; i < cluster ; i++)
    {
      double temp1 = 0.0;


      for(int k = 0 ; k< input_dimension ; k++) center(i,k) = 0.0;

      for(int j = 0 ; j < input_set ; j++)
      {
        for(int k = 0 ; k < input_dimension ; k++)
        {
          center(i,k) = center(i,k) + pow(weight(j,i),m)*origin(j,k);
        }
        temp1 = temp1 + pow(weight(j,i),m);

      }

      for(int k = 0 ; k< input_dimension ; k++) center(i,k) = center(i,k)/temp1;

    }

    for(int i = 0 ; i < cluster ; i++)
    {
      for(int j = 0 ; j < input_set ; j++)
      {
        weight(j,i) = 0.0;

        for(int a = 0 ; a < cluster ; a++)
        {
          double d1,d2;
          d1 = d2 = 0.0;
          for(int b = 0 ; b < input_dimension ; b++)
          {
            d1 = d1 + (origin(j,b) - center(i,b))*(origin(j,b) - center(i,b));
            d2 = d2 + (origin(j,b) - center(a,b))*(origin(j,b) - center(a,b));
          }
          weight(j,i) = weight(j,i) + pow(sqrt(d1)/sqrt(d2),2.0/(m-1.0));
        }
        weight(j,i) = 1/weight(j,i);
      }
    }


    if(step > 1)
    {
      double temp1 = 0;
      for(int i = 0 ; i< cluster ; i++)
      {
        for(int j = 0 ; j <input_set ; j++)
        {
          temp1 = temp1 + (weight_before(j,i) - weight(j,i))*(weight_before(j,i) - weight(j,i));
        }
      }

      e(step-1) = sqrt(temp1);
      if(e(step - 1) < min_error) break;
    }
    weight_before = weight;
    if(step > MAX_STEP) break;
  }
  errors.init(step);

  for(int i = 0 ; i < step ; i++)
  {
    errors(i) = e(i);
  }

}
void MY_FCM :: GBFCM_learning()
{
  if(setting.get_row() < 4)
  {
    printf("setting is incorrect %d\n",setting.get_row());
    exit(1);
  }

  input_dimension = origin.get_row();
  input_set = origin.get_column();


  weight.init(input_set,cluster);
  weight.random(0,1);

  MY_DATA *center_before = new MY_DATA[input_set];


  center.init(cluster,input_dimension);
  center.random(0,1);

  MY_DATA e;

  double gain_temp = gain;
  e.init(MAX_STEP);

  step = 0;
  double temp2 = 0;

  origin.suffle();// recomend!

  while(1)
  {

    step++;
    temp2 = 0;



    for(int i = 0 ; i < input_set ; i++)
    {

   
      for(int j = 0 ; j < cluster ; j++)
      {
        weight(i,j) = 0.0;

        for(int a = 0 ; a < cluster ; a++)
        {
          double d1,d2;
          d1 = d2 = 0.0;
          for(int b = 0 ; b < input_dimension ; b++)
          {
            d1 = d1 + (origin(i,b) - center(j,b))*(origin(i,b) - center(j,b));
            d2 = d2 + (origin(i,b) - center(a,b))*(origin(i,b) - center(a,b));
          }
          weight(i,j) = weight(i,j) + pow(sqrt(d1)/sqrt(d2),2.0/(m-1.0));
        }
        weight(i,j) = 1/weight(i,j);
      }


      for(int j = 0 ; j < cluster ; j++)
      {
        for(int k = 0 ; k < input_dimension ; k++)
        {
          center(j,k) = center(j,k) - gain*pow(weight(i,j),m)*(center(j,k) - origin(i,k));
        }
      }

      if(step > 1)
      {
        for(int k = 0 ; k < cluster ; k++){
          for(int j = 0 ; j < input_dimension ; j++)
          {
            temp2 =temp2 + sqrt(( center(k,j) - center_before[i](k,j))*(center(k,j) - center_before[i](k,j)));
          }
        }
      }
      center_before[i] = center;
    }

    if(step > 1)
    {
      e(step-1) = temp2/input_set;
      if(e(step - 1) < min_error) break;
    }

    if(step > MAX_STEP) break;
  }
  errors.init(step);

  for(int i = 0 ; i < step ; i++)
  {
    errors(i) = e(i);

  }

  delete [] center_before;

}

void MY_FCM :: GBFCM_MK_learning()
{
  if(setting.get_row() < 5)
  {
    printf("setting is incorrect\n");
    exit(1);
  }

  input_dimension = origin.get_row();
  input_set = origin.get_column();


  weight.init(input_set,cluster);
  weight.random(0,1);

  MY_DATA *center_before = new MY_DATA[input_set];


  center.init(cluster,input_dimension);
  center.random(0,1);

  MY_DATA e;

  double gain_temp = gain;
  e.init(MAX_STEP);
  double variance = standard_deviation*standard_deviation;
  step = 0;





  gain_temp = gain_temp/variance;

  double temp2 = 0;
  double temp3 = 0;
  origin.suffle();// recomend!

  while(1)
  {

    step++;
    temp2 = 0;



    for(int i = 0 ; i < input_set ; i++)
    {

      double d1,d2;
      for(int j = 0 ; j < cluster ; j++)
      {
        weight(i,j) = 0.0;

        for(int a = 0 ; a < cluster ; a++)
        {
          
          d1 = d2 = 0.0;
          for(int b = 0 ; b < input_dimension ; b++)
          {
            d1 = d1 + (origin(i,b) - center(j,b))*(origin(i,b) - center(j,b));
            d2 = d2 + (origin(i,b) - center(a,b))*(origin(i,b) - center(a,b));
          }
          weight(i,j) = weight(i,j) + pow((1-exp(d1*(-1)/variance))/(1-exp(d2*(-1)/variance)),1.0/(m-1.0));
        }
        weight(i,j) = 1/weight(i,j);
      }


      for(int j = 0 ; j < cluster ; j++)
      {
        d1 = 0;
        for(int k = 0 ; k < input_dimension ; k++) d1 = d1 + (center(j,k) - origin(i,k))*(center(j,k) - origin(i,k));
        for(int k = 0 ; k < input_dimension ; k++)
        {
          center(j,k) = center(j,k) - gain_temp*pow(weight(i,j),m)*(center(j,k) - origin(i,k))*exp(d1*(-1)/variance);
        }
      }

      if(step > 1)
      {
        for(int k = 0 ; k < cluster ; k++){
          for(int j = 0 ; j < input_dimension ; j++)
          {
            temp2 =temp2 + sqrt(( center(k,j) - center_before[i](k,j))*(center(k,j) - center_before[i](k,j)));
          }
        }
      }
      center_before[i] = center;
    }

    if(step > 1)
    {
      e(step-1) = temp2/input_set;
      if(e(step - 1) < min_error) break;
    }

    if(step > MAX_STEP) break;
  }
  errors.init(step);

  for(int i = 0 ; i < step ; i++)
  {
    errors(i) = e(i);

  }
  origin.fwrite("input.txt");
  delete [] center_before;

}





















