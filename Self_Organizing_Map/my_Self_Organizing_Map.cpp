#include "my_Self_Organizing_Map.hpp"
#include <iostream>
#include <stdlib.h>


using namespace std;


void MY_SOM :: status()
{
  cout<<"----- SOM status -----"<<endl;
  cout<<"cluster : "<<cluster<<endl;
  cout<<"max epoch : "<<total<<endl;
  cout<<"gain : "<<gain<<endl;
  cout<<"distance_gain : "<<distance_gain<<endl;
  cout<<"MSE : "<<MSE<<endl;
  cout<<"weight_range_min : "<<weight_range_min<<endl;
  cout<<"weight_range_max : "<<weight_range_max<<endl;
  cout<<"weight before";
  weight_before.print();
  cout<<"weight";
  
  weight.print();

  cout<<"----------------------"<<endl;
    
}

double MY_SOM :: get_percent()
{
  return (double)epoch/(double)total;    
}
void MY_SOM :: init(int cluster,int total,double gain, double distance_gain, double weight_range_min, double weight_range_max)
{
  this->cluster = cluster;
  this->total = total;
  this->gain = gain;
  this->distance_gain = distance_gain;
  this->weight_range_min = weight_range_min;
  this->weight_range_max = weight_range_max;
  this->epoch = 0;
}

void MY_SOM :: read_input(const char *file_name)
{
    read_status = 1;
    origin.fread(file_name);
    origin_maching.init(origin.get_column(),1);


}
void MY_SOM :: read_weight(const char *file_name)
{ 
    weight.fread(file_name); 
}
void MY_SOM :: write_weight(const char *file_name)
{
    if(learning_status == 0)
    {
        cout<<"you need learning"<<endl;
        exit(1);
    }
   weight.fwrite(file_name);
 

}
void MY_SOM :: write_setting(const char *file_name)
{
    setting.init(5,1);

    setting(0,0) = cluster;
    setting(1,0) = total;
    setting(2,0) = gain;
    setting(3,0) = distance_gain;
    setting(4,0) = MSE;



    setting.fwrite(file_name);
}
void MY_SOM :: write_result(const char *file_name)
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
        result(i,data_dimension) = origin_maching(i,0);
    }
    
    //origin.print();

    //result.print();
    result.fwrite(file_name);
}
void MY_SOM :: learning()
{
    if(read_status == 0)
    {
        cout<<"there is no input data"<<endl;
        exit(1);
    }
    learning_status = 1;


    if(weight.get_row() == 0)
    {
        weight.init(cluster,origin.get_row());
        weight.random(weight_range_min, weight_range_max);
    }
    else
    {
        if(origin.get_row() == weight.get_row())
        {
            cout<<"the dimensions of data don't match"<<endl;
            exit(1);
        }
    }
    weight_before = weight;


    epoch = 0;
    int winner_num;
    double temp;
    double min;




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
            origin_maching(k,0) = winner_num;
        }
        epoch++;
    }

    int data_dimension = origin.get_row();
    int data_set = origin.get_column();


    MY_DATA cluster_data;

    cluster_data.init(cluster,2); //클러스터에 포함된 데이터 수, 에러^2의 합들


    for(int i = 0; i < weight.get_column() ;i++) cluster_data(i,1) = 0.0; 
    for(int i = 0 ; i < data_set ; i++)
    {
        int cluster_num = origin_maching(i,0);
        for(int j = 0 ; j < data_dimension ; j++)
        {
            cluster_data(cluster_num,1) = cluster_data(cluster_num,1) + ((origin(i,j)-weight(cluster_num,j))*((origin(i,j)-weight(cluster_num,j))));
        }
    }
    MSE = 0.0;
    for(int i = 0 ; i < cluster ;i++)
    {
        MSE = MSE + cluster_data(i,1);
        //  cout<<i<<" : "<<cluster_data(i,1)<<endl;
    }
    MSE = MSE/data_set;


}
void MY_SOM :: testing()
{
    if(read_status == 0)
    {
        cout<<"no read data"<<endl;
        exit(1);
    }
    int data_dimension = origin.get_row();
    int data_set = origin.get_column();
    if(data_dimension != weight.get_row())
    {
        cout<<"the dimensions don't match"<<endl;
        exit(1);
    }
    MY_DATA cluster_data;

    cluster_data.init(cluster,2); //클러스터에 포함된 데이터 수, 에러^2의 합들




    double temp = 0.0;
    double min = 0.0;
    int winner = 0;
    for(int i = 0 ; i < data_set ; i++)
    {
        for(int j = 0 ; j < weight.get_column() ; j++)
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
        origin_maching(i,0) = winner;
    }

    for(int i = 0; i < weight.get_column() ;i++) cluster_data(i,1) = 0.0; 
    for(int i = 0 ; i < data_set ; i++)
    {
        int cluster_num = origin_maching(i,0);
        for(int j = 0 ; j < data_dimension ; j++)
        {
            cluster_data(cluster_num,1) = cluster_data(cluster_num,1) + ((origin(i,j)-weight(cluster_num,j))*((origin(i,j)-weight(cluster_num,j))));
        }
    }
    MSE = 0.0;
    for(int i = 0 ; i < cluster ;i++)
    {
        MSE = MSE + cluster_data(i,1);
        //  cout<<i<<" : "<<cluster_data(i,1)<<endl;
    }
    MSE = MSE/data_set;







}


MY_SOM :: MY_SOM()
{
  this->read_status = 0;
  this->learning_status = 0;
  this->cluster = 4;
  this->total = 50000;
  this->gain = 0.2;
  this->distance_gain = 0.02;
  this->weight_range_min = 0.35;
  this->weight_range_max = 0.45;
  this->epoch = 0;



}
MY_SOM ::~ MY_SOM()
{

}




