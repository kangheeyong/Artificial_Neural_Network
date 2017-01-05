#include "my_Centroid_Neural_Network.hpp"
#include <iostream>
#include <stdlib.h>


using namespace std;


void MY_CNN :: status()
{
  cout<<"----- CNN status -----"<<endl;
  cout<<"cluster : "<<cluster<<endl;
  cout<<"epoch : "<<epoch<<endl;
  cout<<"MSE : "<<MSE<<endl;
  cout<<"weight after"<<endl;
  weight.print();
  
  cout<<"----------------------"<<endl;
    
}


void MY_CNN :: init(int cluster)
{
  this->cluster = cluster;
}

void MY_CNN :: read_input(char *file_name)
{
    read_status = 1;
    origin.fread(file_name);
    origin_maching.init(origin.get_column(),1);


}
void MY_CNN :: read_weight(char *file_name)
{ 
    weight.fread(file_name); 
}
void MY_CNN :: write_weight(char *file_name)
{
    if(learning_status == 0)
    {
        cout<<"you need learning"<<endl;
        exit(1);
    }
   weight.fwrite(file_name);
 

}
void MY_CNN :: write_setting(char *file_name)
{
    setting.init(3,1);
    setting(0,0) = cluster;
    setting(1,0) = epoch;
    setting(2,0) = MSE;
    setting.fwrite(file_name);
}
void MY_CNN :: write_result(char *file_name)
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
    
  //  origin.print();

  //  result.print();
    result.fwrite(file_name);
}
void MY_CNN :: learning()
{
    if(read_status == 0)
    {
        cout<<"there is no input data"<<endl;
        exit(1);
    }
    learning_status = 1;

    MY_DATA centroid,small;

    int data_dimension = origin.get_row();
    int data_set = origin.get_column();

    centroid.init(1,data_dimension);
    small.init(1,data_dimension);

    small.random(-0.01,0.01);



    //----------------- calculate centroid ----------------
    for(int i = 0 ;i < data_set ; i++)
    {
        for(int j = 0 ; j < data_dimension ; j++)
        {
            centroid(0,j) = centroid(0,j) + origin(i,j);
        }
    }

    for(int j = 0; j < data_dimension ; j++)
    {
        centroid(0,j) = centroid(0,j)/data_set;
    }
    //----------------------------------------------------

//    centroid.print();
//    small.print();

    MY_DATA cluster_data;

    weight.init(cluster,data_dimension);
    cluster_data.init(cluster,2); //클러스터에 포함된 데이터 수, 에러^2의 합들


    int centroid_k = 2;

    for(int i = 0 ; i < data_dimension ; i++)
    {
        weight(0,i) = centroid(0,i) + small(0,i);
        weight(1,i) = centroid(0,i) - small(0,i);
    }

    epoch = 0;
    double temp = 0.0;
    double min = 0.0;
    int winner = 0;
    int loser = 0;
    while(1)
    {
        do
        {
            loser = 0;
            for(int i = 0 ; i < data_set ; i++)
            {
                for(int j = 0 ; j < centroid_k ; j++)
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
                if(epoch == 0)
                {
                    loser++;
                    cluster_data(winner,0) = cluster_data(winner,0) + 1;
                    origin_maching(i,0) = winner;
                    for(int k = 0 ; k < data_dimension ; k++)
                    {
                        weight(winner,k) = weight(winner,k)+(origin(i,k)-weight(winner,k))/cluster_data(winner,0);
                    }    
                }
                else
                {
                    int before =  (int)origin_maching(i,0); 
                    if(winner != before)
                    {
                        loser++;
                        origin_maching(i,0) = winner;
                        cluster_data(winner,0) = cluster_data(winner,0) + 1;
                        cluster_data(before,0) = cluster_data(before,0) - 1;

                        for(int k = 0 ; k < data_dimension ; k++)
                        { 
                            weight(winner,k) = weight(winner,k)+(origin(i,k)-weight(winner,k))/cluster_data(winner,0);
                            weight(before,k) = weight(before,k)-(origin(i,k)-weight(before,k))/cluster_data(before,0);

                        }

                    }
                }
             //   weight.print(); 만약 실시간 그래프를 하려면 이 타이밍에 출력
            }
            epoch++; 
        }while(loser != 0);
       
        for(int i = 0; i < centroid_k ;i++) cluster_data(i,1) = 0.0; 
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
        // centroid_k가 한개 증가하는 시점의 MSE

        centroid_k++;
        if(centroid_k > cluster) break;
        
        
        double max;
        int max_number;
        

        for(int i = 0 ; i < centroid_k ; i++)
        {
            if(i == 0)
            {
                max =  cluster_data(i,1);
                max_number = i;
            }
            else
            {
                if(max < cluster_data(i,1))
                {
                    max = cluster_data(i,1);
                    max_number = i;
                }
            }
        }
        for(int i = 0 ; i < data_dimension ; i++)
        {
            weight(centroid_k-1,i) = weight(max_number,i) + small(0,i);
        }

    }

 }
void MY_CNN :: testing()
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


MY_CNN :: MY_CNN()
{
  this->read_status = 0;
  this->learning_status = 0;
  this->epoch = 0;
  this->MSE = 0.0;
  this->cluster = 4;
}
MY_CNN ::~ MY_CNN()
{

}




