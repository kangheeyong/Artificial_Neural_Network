#include "my_Fuzzy_C_Means.hpp" 
#include <stdio.h>
#include <math.h>

MY_FCM :: MY_FCM()
{

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
  printf("step : %d\n",step);

  printf("----- center -----");
  center.print();
  printf("----- weight -----");
  weight.print();
  printf("----- errors -----");
  errors.print();

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
}

void MY_FCM ::  write_weight(const char *file_name)
{
  weight.fwrite(file_name);
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





















