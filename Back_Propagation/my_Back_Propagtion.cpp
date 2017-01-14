#include "my_Back_Propagtion.hpp"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>


using namespace std;

MY_BP :: MY_BP()
{
  w = u = delta = NULL;  
  this->init();
}

MY_BP :: ~MY_BP()
{
  this->init();
}

void MY_BP :: init()
{
  origin.init();
  setting.init();
  result.init();
  errors.init();
  target.init();
  input.init();
  if(w != NULL) delete [] w; 
  w = NULL;
  if(u != NULL) delete [] u;
  u = NULL;
  if(delta != NULL) delete [] delta;
  delta = NULL;
  neurals.init();
  iteration = 0;
  epoch = 0;
  max_epoch = 0;
  total_neural_layers = 0;
  error = 0.0;
  target_error = 0.0;
  gain = 0.0;
  total_set = 0;
  input_dimension = 0;
  output_dimension = 0;
  setting_status = 0;
  weight_status = 0;
  input_status = 0;

}

void MY_BP :: status()
{
  if(setting_status == 1)
  {
    cout<<"total_neural_layers : "<<total_neural_layers<<endl;;
    for(int i = 0 ; i < total_neural_layers ; i++)
    {
      cout<<neurals(i)<<" ";
    }
    cout<<endl;
    cout<<"max_epoch : "<<max_epoch<<endl;;
    cout<<"target_error : "<<target_error<<endl;;
    cout<<"gain : "<<gain<<endl;
    cout<<"input_dimension : "<<input_dimension<<endl;
    cout<<"output_dimension : "<<output_dimension<<endl;
    cout<<"error : "<<error<<endl;
    cout<<"epoch : "<<epoch<<endl;
    /*      for(int i = 0 ; i < total_neural_layers ; i++)
            {
            cout<<neurals(i)<<" neural"<<endl<<"u"<<endl;;

            u[i].print();
            cout<<"delta"<<endl;
            delta[i].print();
            if(i != 0)
            {
            cout<<"w"<<endl;
            w[i - 1].print();
            }
            }*/
  }
  else cout<<"no setting"<<endl;
}

void MY_BP :: read_input(const char *file_name)
{
  input_status = 1;
  origin.fread(file_name);
  if(origin.get_row() != input_dimension + output_dimension && origin.get_row() != input_dimension)
  {
    cout<<"the dimensions don't match"<<endl;
    exit(1);
  }
  total_set = origin.get_column();

  errors.init(max_epoch);
  input.init(total_set,input_dimension);
  target.init(total_set,output_dimension);

  for(int i = 0 ; i < total_set ; i++)
  {
    for(int j = 0 ; j < input_dimension ; j++)
    {
      input(i,j) = origin(i,j);
    }
    for(int j = 0 ; j <output_dimension ; j++)
    {
      target(i,j) = origin(i , input_dimension + j);
    }
  }

}

void MY_BP :: read_weight(const char *file_name)
{
  weight_status = 1;

  char buffer[1024];
  sprintf(buffer,"%s_0.txt",file_name);
  this->read_setting(buffer);
  
  for(int i = 0 ; i < total_neural_layers - 1 ; i++)
  {
    sprintf(buffer,"%s_%d.txt",file_name,i+1);
    w[i].fread(buffer);
  }

  if(setting_status == 0)
  {
    cout<<"setting is first"<<endl;
    exit(1);
  }

}

void MY_BP :: rand_weight()
{
  if(setting_status == 0)
  {
    cout<<"setting is first"<<endl;
    exit(1);
  }
  for(int i = 0 ; i < total_neural_layers -1 ; i ++)
  {
    w[i].random(-1,1);
    for(int j = 0 ; j < w[i].get_row() ; j++)
    {
      w[i](0,j) = 0,0;
    }
  }

}

void MY_BP :: mkdir_learning_result(const char* file_name)
{
  mkdir(file_name,0755);

  char temp[1024];

  sprintf(temp,"%s/%s",file_name,"error.txt");
  this->write_error(temp);
  sprintf(temp,"%s/%s",file_name,"weight");
  this->write_weight(temp);
}

void MY_BP :: read_setting(const char *file_name)
{
  setting_status = 1;
  setting.fread(file_name);

  if(setting.get_row() < 4 || setting.get_column() < 2 || setting(0,3) < 2)
  {
    cout<<"setting file fomat dismached"<<endl;
    exit(1);
  }

  max_epoch = setting(0,0);
  gain = setting(0,1);
  target_error = setting(0,2);
  total_neural_layers =setting(0,3);
  neurals.init(total_neural_layers);
  u = new MY_DATA[total_neural_layers];
  w = new MY_DATA[total_neural_layers - 1];
  delta = new MY_DATA[total_neural_layers];
  for(int i = 0 ; i < total_neural_layers ; i++)
  {
    neurals(i) = setting(1,i);
    u[i].init(neurals(i) + 1);
    delta[i].init(neurals(i) + 1);
    if(i != 0)
    {
      w[i - 1].init(neurals(i)+1,neurals(i - 1)+1);
    }
  }
  input_dimension = neurals(0);
  output_dimension = neurals(total_neural_layers - 1);


}

void MY_BP :: write_weight(const char *file_name)
{
  char buffer[1024];
  sprintf(buffer,"%s_0.txt",file_name);

  setting.fwrite(buffer);
  for(int i = 0 ; i < total_neural_layers - 1 ; i++)
  {
    sprintf(buffer,"%s_%d.txt",file_name,i+1);
    w[i].fwrite(buffer);
  }

}

void MY_BP :: write_setting(const char *file_name)
{

}

void MY_BP :: write_result(const char *file_name)
{
  result.fwrite(file_name);
}

void MY_BP :: write_error(const char *file_name)
{
  errors.fwrite(file_name);
}

void MY_BP :: gradient_descent_algorithm()
{
  for(int k = total_neural_layers - 2 ; k > 0 ; k--)
  {
    for(int i = 1 ; i <= neurals(k) ; i++)
    {
      delta[k](i) = 0.0;
      for(int j = 1; j <= neurals(k+1) ; j++)
      {
        delta[k](i) = delta[k](i) + u[k](i)*(1- u[k](i))*w[k](j,i)*delta[k+1](j);
      }
    }
    for(int i = 1 ; i <= neurals(k+1) ; i++)
    {
      for(int j = 0 ; j <= neurals(k) ; j++)
      {
        w[k](i,j) = w[k](i,j) + gain*u[k](j)*delta[k+1](i);
      }
    }
  }

  for(int i = 1 ; i <= neurals(1) ; i++)
  {
    for(int j = 0 ; j <= neurals(0) ; j++)
    {
      w[0](i,j) = w[0](i,j) + gain*u[0](j)*delta[1](i);
    }
  }

}
void MY_BP :: activation()
{
  double temp;
  for(int k = 0 ; k <total_neural_layers -1  ; k++)
  {
    u[k](0) = 1;
    for(int i = 1 ; i <= neurals(k+1) ; i++)
    {
      temp = 0.0;
      for(int j = 0 ; j <= neurals(k) ; j++)
      {
        temp = temp + w[k](i,j)*u[k](j);

      }
      u[k+1](i) = F(temp);
    }
  }
}
void MY_BP :: learning()
{
  if(input_status == 0 || setting_status == 0)
  {
    cout<<"no input or setting "<<endl;
    exit(1);
  }

  while(epoch < max_epoch)
  {
    origin.timer(epoch, max_epoch);
    error = 0.0;
    for(int k = 0 ; k < total_set ; k++)
    {
      for(int i = 0 ; i < input_dimension ; i++) u[0](i+1) = input(k,i);
      activation();
      for(int i = 1 ; i <= output_dimension ; i++)
      {
        delta[total_neural_layers -1](i) = u[total_neural_layers -1](i)*(1 - u[total_neural_layers -1](i))*(target(k,i-1) - u[total_neural_layers -1](i));
        error = error + (target(k,i-1) - u[total_neural_layers -1](i))* (target(k,i-1) - u[total_neural_layers -1](i));
      }

      gradient_descent_algorithm();
    }

    error = sqrt(error/(total_set*output_dimension)); 
    errors(epoch) = error;
    if(target_error > error) break;
    epoch++;
  }
}

void MY_BP :: testing()
{
  if(input_status == 0 || setting_status == 0 || weight_status == 0)
  {
    cout<<"no input, weight or setting "<<endl;
    exit(1);
  }
  result.init(total_set,input_dimension + output_dimension);

  for(int k = 0 ; k < total_set ; k++)
  {
    for(int i = 0 ; i < input_dimension ; i++)
    {
      u[0](i+1) = input(k,i);
      result(k,i) = input(k,i);
    }
    activation();
    for(int i = 1 ; i <= output_dimension ; i++)
    {
      result(k,input_dimension + i - 1) = u[total_neural_layers -1](i);
    }
  }
}






