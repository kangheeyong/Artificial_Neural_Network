#include "my_K_Means.hpp"
#include <iostream>
#include <math.h>


using namespace std;

void  K_MEANS ::init(int cluster)
{
  this->cluster = cluster;
}
void  K_MEANS ::status()
{
  cout<<"----- K Means status -----"<<endl;
  cout<<"cluster : "<<cluster<<endl;
  cout<<"min_error : "<<min_error<<endl;
  cout<<"epoch : "<<epoch<<endl;
  cout<<"MSE : "<<MSE<<endl;
  cout<<"weight before"<<endl;
  first_weight.print();

  cout<<"weight after"<<endl;
  weight.print();
  
  cout<<"----------------------"<<endl;
 
}
void  K_MEANS ::read_input(const char *file_name)
{
  origin.fread(file_name);
  origin_maching.init(origin.get_column());

}
void  K_MEANS ::read_weight(const char *file_name)
{
  weight.fread(file_name);
}
void  K_MEANS ::read_setting(const char *file_name)
{
  setting.fread(file_name);
}
void  K_MEANS ::write_weight(const char *file_name)
{
  weight.fwrite(file_name);
}
void  K_MEANS ::write_result(const char *file_name)
{ 
  MY_DATA result;

  int data_set = origin.get_column();
  int data_dimension = origin.get_row();

  result.init(data_set,data_dimension + 1);

  for(int i = 0 ; i < data_set ; i++)
  {
    for(int j = 0 ; j < data_dimension ;j++)
    {
      result(i,j) = origin(i,j);
    }
    result(i,data_dimension) = origin_maching(i);
  }

  //  origin.print();

  //  result.print();
  result.fwrite(file_name);
}
void  K_MEANS ::write_error(const char *file_name)
{
  errors.fwrite(file_name);
}

void  K_MEANS ::learning()
{

  cluster = setting(0);
  min_error = setting(1);

  int data_dimension = origin.get_row();
  int data_set = origin.get_column();


  weight.init(cluster,data_dimension);
  weight.random(setting(2),setting(3));
  
  first_weight = weight;

  MY_DATA before_weight;
  MY_DATA center,temp_weight;;

  center.init(cluster,2);

  errors.init(MAX_EPOCH);

  double temp = 0;
  double min;
  int winner;

  epoch = 0;

  while(1)
  {
    epoch++;
    temp_weight.init(cluster,data_dimension);
    center.init(cluster,2);

    for(int i = 0 ; i < data_set ; i++)
    {
      
      for(int j = 0 ; j < cluster ;j++)
      {
        temp = 0.0;
        for(int k = 0 ; k < data_dimension ; k++)
        {
          temp = temp + (origin(i,k)-weight(j,k))*(origin(i,k)-weight(j,k));
        }
        if(j == 0)
        {
          min = temp;
          winner = j;
        }
        else
        {
          if(min > temp)
          {
            min = temp;
            winner = j;
          }
        }
      }
      center(winner,0)++;
      center(winner,1) = center(winner,1) + min;
      origin_maching(i) = winner;
      for(int k = 0 ; k < data_dimension ; k++)
      {
        temp_weight(winner,k) = temp_weight(winner,k) + origin(i,k);
      }
    }

    for(int i = 0 ; i < cluster ; i++)
    {
      for(int k = 0 ; k < data_dimension ; k++)
      {
        weight(i,k) = temp_weight(i,k)/center(i,0);
      }
    }
    if(epoch != 1)
    {
      temp = 0;
      MSE = 0;
      for(int i = 0 ; i < cluster ; i++)
      {
        MSE = MSE + center(i,1);
        for(int k = 0 ; k < data_dimension ; k++)
        {
          temp = temp + (weight(i,k)-before_weight(i,k))*(weight(i,k)-before_weight(i,k));
        }
      }

      MSE = MSE/data_set;
      errors(epoch-1) = MSE;
      if(temp<min_error)
      {
        cout<<"error : "<<temp<<endl;
        break;
      }
      if(epoch > MAX_EPOCH) break;
    }
    else
    {
      MSE = 0;
      for(int i = 0 ; i < cluster ; i++)
      {
        MSE = MSE + center(i,1);
      }

      MSE = MSE/data_set;
      errors(epoch-1) = MSE;
    }

    before_weight = weight;
  }
}
void  K_MEANS ::testing()
{

}

K_MEANS ::K_MEANS()
{
  cluster = 2;
  epoch = 0;
  MSE = 0;
  min_error = 0.1;

}
K_MEANS ::~K_MEANS()
{

}

void  K_MEANS ::learning_for_comp_CNN()
{

  cluster = setting(0);
  min_error = setting(1);

  int data_dimension = origin.get_row();
  int data_set = origin.get_column();
  MY_DATA centroid,small;


  centroid.init(data_dimension);
  small.init(data_dimension);

  //----------------- calculate centroid ----------------
  for(int i = 0 ;i < data_set ; i++)
  {
    for(int j = 0 ; j < data_dimension ; j++)
    {
      centroid(j) = centroid(j) + origin(i,j);
    }
  }

  for(int j = 0; j < data_dimension ; j++)
  {
    centroid(j) = centroid(j)/data_set;
  }
  //----------------------------------------------------



  weight.init(cluster,data_dimension);
  weight.random(setting(2),setting(3));

  for(int i = 0 ; i < cluster ; i++)
  {
    small.random(-0.01,0.01);
    for(int j = 0 ; j < data_dimension ; j++)
    {
      weight(i,j) = centroid(j) + small(j);
    }
  }


  first_weight = weight;

  MY_DATA before_weight;
  MY_DATA center,temp_weight;;

  center.init(cluster,2);

  errors.init(MAX_EPOCH);

  double temp = 0;
  double min;
  int winner;

  epoch = 0;

  while(1)
  {
    epoch++;
    temp_weight.init(cluster,data_dimension);
    center.init(cluster,2);

    for(int i = 0 ; i < data_set ; i++)
    {
      
      for(int j = 0 ; j < cluster ;j++)
      {
        temp = 0.0;
        for(int k = 0 ; k < data_dimension ; k++)
        {
          temp = temp + (origin(i,k)-weight(j,k))*(origin(i,k)-weight(j,k));
        }
        if(j == 0)
        {
          min = temp;
          winner = j;
        }
        else
        {
          if(min > temp)
          {
            min = temp;
            winner = j;
          }
        }
      }
      center(winner,0)++;
      center(winner,1) = center(winner,1) + min;
      origin_maching(i) = winner;
      for(int k = 0 ; k < data_dimension ; k++)
      {
        temp_weight(winner,k) = temp_weight(winner,k) + origin(i,k);
      }
    }

    for(int i = 0 ; i < cluster ; i++)
    {
      for(int k = 0 ; k < data_dimension ; k++)
      {
        weight(i,k) = temp_weight(i,k)/center(i,0);
      }
    }
    if(epoch != 1)
    {
      temp = 0;
      MSE = 0;
      for(int i = 0 ; i < cluster ; i++)
      {
        MSE = MSE + center(i,1);
        for(int k = 0 ; k < data_dimension ; k++)
        {
          temp = temp + (weight(i,k)-before_weight(i,k))*(weight(i,k)-before_weight(i,k));
        }
      }

      MSE = MSE/data_set;
      errors(epoch-1) = MSE;
      if(temp<min_error)
      {
        cout<<"error : "<<temp<<endl;
        break;
      }
      if(epoch > MAX_EPOCH) break;
    }
    else
    {
      MSE = 0;
      for(int i = 0 ; i < cluster ; i++)
      {
        MSE = MSE + center(i,1);
      }

      MSE = MSE/data_set;
      errors(epoch-1) = MSE;
    }

    before_weight = weight;
  }
}












