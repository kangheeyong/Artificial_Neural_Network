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

void MY_CNN :: read_input(const char *file_name)
{
  read_status = 1;
  origin.fread(file_name);
  origin_maching.init(origin.get_column(),1);
  int data_dimension = origin.get_row();
  int data_set = origin.get_column();



  mean.init(1,data_dimension);
  variance.init(1,data_dimension);
  wcnn_weight.init(data_set,data_dimension);

  for(int i = 0 ; i < data_set ; i++)
  {
    for(int j = 0 ; j < data_dimension ; j++)
    {
      mean(0,j) = mean(0,j) + origin(i,j);
    }
  }
  for(int j = 0 ; j < data_dimension ; j++)
  {
    mean(0,j) = mean(0,j)/data_set;
  }

  for(int i = 0 ; i < data_set ; i++)
  {
    for(int j = 0 ; j < data_dimension ; j++)
    {
      variance(0,j) = variance(0,j) + (mean(0,j) - origin(i,j))*(mean(0,j) - origin(i,j));
      wcnn_weight(i,j) = wcnn_weight(i,j) +  (mean(0,j) - origin(i,j))*(mean(0,j) - origin(i,j));

    }
  }

  for(int j = 0 ; j < data_dimension ; j++)
  {
    variance(0,j) = variance(0,j)/data_set;
  }


  for(int i = 0 ; i < data_set ; i++)
  {
    for(int j = 0 ; j < data_dimension ; j++)
    {
      //이부분을 수정하여 wcnn의 weight를 바꾼다
      if(wcnn_weight(i,j) < variance(0,j))  wcnn_weight(i,j) = 0.682;
      else if(wcnn_weight(i,j) < 4*variance(0,j))  wcnn_weight(i,j) = 0.272;
      else wcnn_weight(i,j) = 0.044;
    }
  }



}
void MY_CNN :: read_weight(const char *file_name)
{ 
  weight.fread(file_name); 
}
void MY_CNN :: write_weight(const char *file_name)
{
  if(learning_status == 0)
  {
    cout<<"you need learning"<<endl;
    exit(1);
  }
  weight.fwrite(file_name);


}
void MY_CNN :: write_setting(const char *file_name)
{
  setting.init(3,1);
  setting(0,0) = cluster;
  setting(1,0) = epoch;
  setting(2,0) = MSE;
  setting.fwrite(file_name);
}
void MY_CNN :: write_error(const char *file_name)
{
  error.fwrite(file_name);
}
void MY_CNN :: write_result(const char *file_name)
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

double MY_CNN :: get_percent()
{
  return (double)centroid_k/(double)cluster;
}

void MY_CNN :: learning()
{
  if(read_status == 0)
  {
    cout<<"there is no input data"<<endl;
    exit(1);
  }
  learning_status = 1;
  origin.suffle();

  MY_DATA centroid,small;

  MY_DATA e;
  e.init(MAX_EPOCH,3);
 
  int data_dimension = origin.get_row();
  int data_set = origin.get_column();

  int pibonachi = 1;

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


  centroid_k = 2;

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
            /*divergence  */
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

      /*end point epoch*/



      for(int i = 0; i < centroid_k ;i++) cluster_data(i,1) = 0.0; 
      for(int i = 0 ; i < data_set ; i++)
      {
        int cluster_num = origin_maching(i,0);
        for(int j = 0 ; j < data_dimension ; j++)
        {/*divergence*/
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
      e(epoch,0) = MSE;
      e(epoch,1) = centroid_k;
      e(epoch,2) = loser;
      epoch++;

      //if(loser < data_set*0.1 && centroid_k != cluster) break;
  
    }while(loser != 0);

    //-----------------------------------------------------------
    system("clear"); 
    cout<<"----------------------"<<endl;
    cout<<"percent : "<<this->get_percent()*100<<"%"<<endl;
    cout<<"k : "<<centroid_k<<endl;
    cout<<"epoch : "<<epoch<<endl;
    cout<<"----------------------"<<endl;
    //-----------------------------------------------------------

/*
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
*/

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
    small.random(-0.01,0.01);

    for(int i = 0 ; i < data_dimension ; i++)
    {
      weight(centroid_k-1,i) = weight(max_number,i) + small(0,i);
    }

  }
  error.init(epoch,3);
  for(int i = 0 ; i < epoch ; i++)
  {
    error(i,0) = e(i,0);
    error(i,1) = e(i,1);
    error(i,2) = e(i,2);
  }
//---------------temp expoeriment---------------
 /* MY_DATA b;
  b.init(cluster,2);
  for(int i = 0 ; i < cluster ; i++)
  {
    b(i,0) = weight(i,0);
    b(i,1) = cluster_data(i,0);
  }
  b.fwrite("temp_experiment.txt");
*/
}

void MY_CNN :: DCNN_learning()
{
  if(read_status == 0)
  {
    cout<<"there is no input data"<<endl;
    exit(1);
  }


  learning_status = 1;
  origin.suffle();

  MY_DATA centroid,small,variance;

  MY_DATA e;
  e.init(MAX_EPOCH,3);
 
  int data_dimension = origin.get_row();
  int data_set = origin.get_column();

  if(data_dimension % 2 == 1)
  if(data_dimension != 4)
  {
    cout<<"no 2-dimensions GPFD"<<endl;
    exit(1);
  }
  data_dimension = data_dimension/2;

  centroid.init(1,data_dimension);
  small.init(1,data_dimension);
  variance.init(data_dimension);
  small.random(-0.01,0.01);


  //----------------- calculate centroid ----------------

  for(int i = 0 ;i < data_set ; i++)
  {
    for(int j = 0 ; j < data_dimension ; j++)
    {
      centroid(0,j) = centroid(0,j) + origin(i,j);
      variance(j) = variance(j) + origin(i,data_dimension+j);
    }
  }

  for(int j = 0; j < data_dimension ; j++)
  {
    centroid(0,j) = centroid(0,j)/data_set;
    variance(j) = variance(j)/data_set;
  }


  //----------------------------------------------------


  MY_DATA cluster_data;
   MY_DATA temp_weight;
  weight.init(cluster,data_dimension*2);
  cluster_data.init(cluster,2); //클러스터에 포함된 데이터 수, 에러^2의 합들


  centroid_k = 2;

  for(int i = 0 ; i < data_dimension ; i++)
  {
    weight(0,i) = centroid(0,i) + small(0,i);
    weight(1,i) = centroid(0,i) - small(0,i);
    weight(0,data_dimension+i) = variance(i);
    weight(1,data_dimension+i) = variance(i);
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
            /*divergence  */
            temp = temp + (origin(i,data_dimension + k)+(origin(i,k)-weight(j,k))*(origin(i,k)-weight(j,k)))/weight(j,data_dimension + k)+(weight(j,data_dimension + k)+(origin(i,k)-weight(j,k))*(origin(i,k)-weight(j,k)))/origin(i,data_dimension + k);
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
      

   
      temp_weight.init(centroid_k, 2*data_dimension);
      /*end point epoch*/

      for(int i = 0; i < centroid_k ;i++) cluster_data(i,1) = 0.0; 

      for(int i = 0 ; i < data_set ; i++)
      {
        int cluster_num = origin_maching(i,0);
        for(int j = 0 ; j < data_dimension ; j++)
        {
          /*divergence*/
          cluster_data(cluster_num,1) = cluster_data(cluster_num,1) + (origin(i,data_dimension+j)+(origin(i,j)-weight(cluster_num,j))*((origin(i,j)-weight(cluster_num,j))))/weight(cluster_num,data_dimension+j) + (weight(cluster_num,data_dimension+j)+(origin(i,j)-weight(cluster_num,j))*((origin(i,j)-weight(cluster_num,j))))/origin(i,data_dimension+j);
          temp_weight(cluster_num,data_dimension+j) = temp_weight(cluster_num,data_dimension+j) + origin(i,data_dimension + j) + (origin(i,j)-weight(cluster_num,j))*((origin(i,j)-weight(cluster_num,j))); 
        
        
        }
      }
      for(int i = 0 ; i < centroid_k ; i++)
      {
        for(int j = 0 ; j < data_dimension ; j++)
        {
          weight(i,data_dimension + j) = temp_weight(i,data_dimension + j)/cluster_data(i,0);
        }
      }

      MSE = 0.0;
      for(int i = 0 ; i < cluster ;i++)
      {
        MSE = MSE + cluster_data(i,1);
      }
      MSE = MSE/data_set;
      e(epoch,0) = MSE;
      e(epoch,1) = centroid_k;
      e(epoch,2) = loser;
      epoch++;

      //if(loser < data_set*0.1 && centroid_k != cluster) break;
  
    }while(loser != 0);

    //-----------------------------------------------------------
    system("clear"); 
    cout<<"----------------------"<<endl;
    cout<<"percent : "<<this->get_percent()*100<<"%"<<endl;
    cout<<"k : "<<centroid_k<<endl;
    cout<<"epoch : "<<epoch<<endl;
    cout<<"----------------------"<<endl;
    //-----------------------------------------------------------


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
    small.random(-0.01,0.01);

    for(int i = 0 ; i < data_dimension ; i++)
    {
      weight(centroid_k-1,i) = weight(max_number,i) + small(0,i);
      weight(centroid_k-1,data_dimension+i) = weight(max_number,data_dimension+i) + small(0,i);
    }

  }
  error.init(epoch,3);
  for(int i = 0 ; i < epoch ; i++)
  {
    error(i,0) = e(i,0);
    error(i,1) = e(i,1);
    error(i,2) = e(i,2);
  }
}

void MY_CNN :: DCNN_pibonachi_learning()
{
  if(read_status == 0)
  {
    cout<<"there is no input data"<<endl;
    exit(1);
  }

  MY_DATA decent;
  learning_status = 1;
  origin.suffle();

  MY_DATA centroid,small,variance;

  MY_DATA e;
  e.init(MAX_EPOCH,3);
 
  int data_dimension = origin.get_row();
  int data_set = origin.get_column();
  int before_centroid_k;
  int pibonachi = 1;


  //if(data_dimension % 2 == 1)
  if(data_dimension != 4)
  {
    cout<<"no 2-dimensions GPFD"<<endl;
    exit(1);
  }
  data_dimension = data_dimension/2;

  centroid.init(1,data_dimension);
  small.init(1,data_dimension);
  variance.init(data_dimension);
  small.random(-0.01,0.01);


  //----------------- calculate centroid ----------------
  for(int i = 0 ;i < data_set ; i++)
  {
    for(int j = 0 ; j < data_dimension ; j++)
    {
      centroid(0,j) = centroid(0,j) + origin(i,j);
      variance(j) = variance(j) + origin(i,data_dimension+j);
    }
  }

  for(int j = 0; j < data_dimension ; j++)
  {
    centroid(0,j) = centroid(0,j)/data_set;
    variance(j) = variance(j)/data_set;
  }


  //----------------------------------------------------


  MY_DATA cluster_data;

  weight.init(cluster,data_dimension*2);
  cluster_data.init(cluster,2); //클러스터에 포함된 데이터 수, 에러^2의 합들


  centroid_k = 2;

  for(int i = 0 ; i < data_dimension ; i++)
  {
    weight(0,i) = centroid(0,i) + small(0,i);
    weight(1,i) = centroid(0,i) - small(0,i);
    weight(0,data_dimension+i) = variance(i);
    weight(1,data_dimension+i) = variance(i);
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
            /*divergence  */
            temp = temp + (origin(i,data_dimension + k)+(origin(i,k)-weight(j,k))*(origin(i,k)-weight(j,k)))/weight(j,data_dimension + k)+(weight(j,data_dimension + k)+(origin(i,k)-weight(j,k))*(origin(i,k)-weight(j,k)))/origin(i,data_dimension + k);
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
      

      MY_DATA temp_weight;
      temp_weight.init(cluster, 2*data_dimension);
      /*end point epoch*/

      for(int i = 0; i < centroid_k ;i++) cluster_data(i,1) = 0.0; 

      for(int i = 0 ; i < data_set ; i++)
      {
        int cluster_num = origin_maching(i,0);
        for(int j = 0 ; j < data_dimension ; j++)
        {
          /*divergence*/
          cluster_data(cluster_num,1) = cluster_data(cluster_num,1) + (origin(i,data_dimension+j)+(origin(i,j)-weight(cluster_num,j))*((origin(i,j)-weight(cluster_num,j))))/weight(cluster_num,data_dimension+j) + (weight(cluster_num,data_dimension+j)+(origin(i,j)-weight(cluster_num,j))*((origin(i,j)-weight(cluster_num,j))))/origin(i,data_dimension+j);
          temp_weight(cluster_num,data_dimension+j) = temp_weight(cluster_num,data_dimension+j) + origin(i,data_dimension + j) + (origin(i,j)-weight(cluster_num,j))*((origin(i,j)-weight(cluster_num,j))); 
        
        
        }
      }
      for(int i = 0 ; i < centroid_k ; i++)
      {
        for(int j = 0 ; j < data_dimension ; j++)
        {
          weight(i,data_dimension + j) = temp_weight(i,data_dimension + j)/cluster_data(i,0);
        }
      }

      MSE = 0.0;
      for(int i = 0 ; i < cluster ;i++)
      {
        MSE = MSE + cluster_data(i,1);
      }
      MSE = MSE/data_set;
      e(epoch,0) = MSE;
      e(epoch,1) = centroid_k;
      e(epoch,2) = loser;
      epoch++;

      //if(loser < data_set*0.1 && centroid_k != cluster) break;
  
    }while(loser != 0);

    //-----------------------------------------------------------
    system("clear"); 
    cout<<"----------------------"<<endl;
    cout<<"percent : "<<this->get_percent()*100<<"%"<<endl;
    cout<<"k : "<<centroid_k<<endl;
    cout<<"epoch : "<<epoch<<endl;
    cout<<"----------------------"<<endl;
    //-----------------------------------------------------------
    decent.init(centroid_k);
        
    for(int i = 0 ; i < centroid_k ; i++) decent.add_decending_sort(cluster_data(i,1),centroid_k);
    decent.print();

    before_centroid_k = centroid_k;
    centroid_k = centroid_k + pibonachi;
    
    if(centroid_k > cluster)
    {
      if(centroid_k - pibonachi < cluster)
      {

        pibonachi = cluster - (centroid_k - pibonachi);
        centroid_k = cluster;
        for(int i = 0 ; i < pibonachi ; i++)
        {
          for(int j = 0 ; j < centroid_k ;j++)
          {
            if(decent(i) == cluster_data(j,1))
            {
              for(int k = 0 ; k < data_dimension ; k++)
              {
                weight(centroid_k - pibonachi +i,k) = weight(j,k) + small(0,k);
      weight(centroid_k - pibonachi + i,data_dimension+k) = weight(j,data_dimension+k) + small(0,k);
 
              }
              break;
            }
          }
        }


        continue;
      }
      break;
    }


    double max;
    int max_number;


    for(int i = 0 ; i < pibonachi ; i++)
    {
      for(int j = 0 ; j < centroid_k - pibonachi ;j++)
      {
        if(decent(i) == cluster_data(j,1))
        {
          for(int k = 0 ; k < data_dimension ; k++)
          {
            weight(centroid_k - pibonachi +i,k) = weight(j,k) + small(0,k);
  weight(centroid_k - pibonachi + i,data_dimension+k) = weight(j,data_dimension+k) + small(0,k);
          }
          break;
        }
      }
    }
    pibonachi = before_centroid_k;
  }

  /*

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
    small.random(-0.01,0.01);

    for(int i = 0 ; i < data_dimension ; i++)
    {
      weight(centroid_k-1,i) = weight(max_number,i) + small(0,i);
      weight(centroid_k-1,data_dimension+i) = weight(max_number,data_dimension+i) + small(0,i);
    }

  }*/
  error.init(epoch,3);
  for(int i = 0 ; i < epoch ; i++)
  {
    error(i,0) = e(i,0);
    error(i,1) = e(i,1);
    error(i,2) = e(i,2);
  }
}



void MY_CNN :: pibonachi_learning()
{
  if(read_status == 0)
  {
    cout<<"there is no input data"<<endl;
    exit(1);
  }
  learning_status = 1;
  origin.suffle();

  MY_DATA e;
  e.init(MAX_EPOCH,3);
  MY_DATA centroid,small;

  int data_dimension = origin.get_row();
  int data_set = origin.get_column();
  int before_centroid_k;
  int pibonachi = 1;

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


  centroid_k = 2;

  for(int i = 0 ; i < data_dimension ; i++)
  {
    weight(0,i) = centroid(0,i) + small(0,i);
    weight(1,i) = centroid(0,i) - small(0,i);
  }
  MY_DATA decent;
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
      }
      MSE = MSE/data_set;
      e(epoch,0) = MSE;
      e(epoch,1) = centroid_k;
      e(epoch,2) = loser;
      epoch++; 
    }while(loser != 0);

    //-----------------------------------------------------------
    system("clear"); 
    cout<<"----------------------"<<endl;
    cout<<"percent : "<<this->get_percent()*100<<"%"<<endl;
    cout<<"k : "<<centroid_k<<endl;
    cout<<"epoch : "<<epoch<<endl;
    cout<<"MSE : "<<MSE<<endl;
    cout<<"----------------------"<<endl;
    //-----------------------------------------------------------
    decent.init(centroid_k);
        
    for(int i = 0 ; i < centroid_k ; i++) decent.add_decending_sort(cluster_data(i,1),centroid_k);
    decent.print();

    before_centroid_k = centroid_k;
    centroid_k = centroid_k + pibonachi;
    
    if(centroid_k > cluster)
    {
      if(centroid_k - pibonachi < cluster)
      {

        pibonachi = cluster - (centroid_k - pibonachi);
        centroid_k = cluster;
        for(int i = 0 ; i < pibonachi ; i++)
        {
          for(int j = 0 ; j < centroid_k ;j++)
          {
            if(decent(i) == cluster_data(j,1))
            {
              for(int k = 0 ; k < data_dimension ; k++)
              {
                weight(centroid_k - pibonachi +i,k) = weight(j,k) + small(0,k);
              }
              break;
            }
          }
        }


        continue;
      }
      break;
    }


    double max;
    int max_number;


    for(int i = 0 ; i < pibonachi ; i++)
    {
      for(int j = 0 ; j < centroid_k - pibonachi ;j++)
      {
        if(decent(i) == cluster_data(j,1))
        {
          for(int k = 0 ; k < data_dimension ; k++)
          {
            weight(centroid_k - pibonachi +i,k) = weight(j,k) + small(0,k);
          }
          break;
        }
      }
    }
    pibonachi = before_centroid_k;
  }
  error.init(epoch,3);
  for(int i = 0 ; i < epoch ; i++)
  {
    error(i,0) = e(i,0);
    error(i,1) = e(i,1);
    error(i,2) = e(i,2);
  }
//---------------temp expoeriment---------------
  MY_DATA b;
  b.init(cluster,2);
  for(int i = 0 ; i < cluster ; i++)
  {
    b(i,0) = weight(i,0);
    b(i,1) = cluster_data(i,0);
  }
  b.fwrite("p_temp_experiment.txt");


}
void MY_CNN :: pibonachi2_learning()
{
  if(read_status == 0)
  {
    cout<<"there is no input data"<<endl;
    exit(1);
  }
  learning_status = 1;
  origin.suffle();

  MY_DATA e;
  e.init(MAX_EPOCH,3);
  MY_DATA centroid,small;

  int data_dimension = origin.get_row();
  int data_set = origin.get_column();
  int before_centroid_k;
  int pibonachi = 1;

  centroid.init(1,data_dimension);
  small.init(1,data_dimension);

  small.random(-0.01,0.01);

  for(int i = 0 ; i < data_set ; i++) origin_maching(i,0) = -1;

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
  MY_DATA var;

  weight.init(cluster,data_dimension);
  cluster_data.init(cluster,2); //클러스터에 포함된 데이터 수, 에러^2의 합들


  centroid_k = 2;




  for(int i = 0 ; i < data_dimension ; i++)
  {
    weight(0,i) = centroid(0,i) + small(0,i);
    weight(1,i) = centroid(0,i) - small(0,i);
  }




  MY_DATA decent;
  epoch = 0;
  double temp = 0.0;
  double min = 0.0;
  int winner = 0;
  int loser = 0;


  int f = 0;

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
      }
      MSE = MSE/data_set;
      e(epoch,0) = MSE;
      e(epoch,1) = centroid_k;
      e(epoch,2) = loser;
      epoch++;
      int f_n;

      if(f == 0) f_n = loser;
      f++;
      if(loser <= f_n*0.08*(1-centroid_k/cluster) && centroid_k != cluster) break;
    }while(loser != 0);
    f = 0;
    //-----------------------------------------------------------
    system("clear"); 
    cout<<"----------------------"<<endl;
    cout<<"percent : "<<this->get_percent()*100<<"%"<<endl;
    cout<<"k : "<<centroid_k<<endl;
    cout<<"epoch : "<<epoch<<endl;
    cout<<"MSE : "<<MSE<<endl;
    cout<<"----------------------"<<endl;
    //-----------------------------------------------------------
    decent.init(centroid_k);
        
    for(int i = 0 ; i < centroid_k ; i++) decent.add_decending_sort(cluster_data(i,1),centroid_k);
    decent.print();

    before_centroid_k = centroid_k;
    centroid_k = centroid_k + pibonachi;
    
    if(centroid_k > cluster)
    {
      if(centroid_k - pibonachi < cluster)
      {

        pibonachi = cluster - (centroid_k - pibonachi);
        centroid_k = cluster;
        for(int i = 0 ; i < pibonachi ; i++)
        {
          for(int j = 0 ; j < centroid_k ;j++)
          {
            if(decent(i) == cluster_data(j,1))
            {
              for(int k = 0 ; k < data_dimension ; k++)
              {
                weight(centroid_k - pibonachi +i,k) = weight(j,k) + small(0,k);
              }
              break;
            }
          }
        }


        continue;
      }
      break;
    }


    double max;
    int max_number;


    for(int i = 0 ; i < pibonachi ; i++)
    {
      for(int j = 0 ; j < centroid_k - pibonachi ;j++)
      {
        if(decent(i) == cluster_data(j,1))
        {
          small.random(-0.01,0.01);

          for(int k = 0 ; k < data_dimension ; k++)
          {
            weight(centroid_k - pibonachi +i,k) = weight(j,k) + small(0,k);
          }
          break;
        }
      }
    }
    pibonachi = before_centroid_k;
  }
  error.init(epoch,3);
  for(int i = 0 ; i < epoch ; i++)
  {
    error(i,0) = e(i,0);
    error(i,1) = e(i,1);
    error(i,2) = e(i,2);
  }
  //---------------temp expoeriment---------------
  MY_DATA b;
  b.init(cluster,2);
  for(int i = 0 ; i < cluster ; i++)
  {
    b(i,0) = weight(i,0);
    b(i,1) = cluster_data(i,0);
  }
  b.fwrite("d_temp_experiment.txt");


}


void MY_CNN :: temp_learning()
{
  if(read_status == 0)
  {
    cout<<"there is no input data"<<endl;
    exit(1);
  }
  learning_status = 1;
  origin.suffle();

  MY_DATA e;
  e.init(MAX_EPOCH,2);
  MY_DATA centroid,small;

  int data_dimension = origin.get_row();
  int data_set = origin.get_column();
  int before_centroid_k;
  int two = 2;

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


  centroid_k = 2;

  for(int i = 0 ; i < data_dimension ; i++)
  {
    weight(0,i) = centroid(0,i) + small(0,i);
    weight(1,i) = centroid(0,i) - small(0,i);
  }
  MY_DATA decent;
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
      }
      MSE = MSE/data_set;
      e(epoch,0) = MSE;
      e(epoch,1) = centroid_k;
      epoch++; 
    }while(loser != 0);

    //-----------------------------------------------------------
    system("clear"); 
    cout<<"----------------------"<<endl;
    cout<<"percent : "<<this->get_percent()*100<<"%"<<endl;
    cout<<"k : "<<centroid_k<<endl;
    cout<<"epoch : "<<epoch<<endl;
    cout<<"MSE : "<<MSE<<endl;
    cout<<"----------------------"<<endl;
    //-----------------------------------------------------------
    decent.init(centroid_k);
        
    for(int i = 0 ; i < centroid_k ; i++) decent.add_decending_sort(cluster_data(i,1),centroid_k);
    decent.print();

    before_centroid_k = centroid_k;
    centroid_k = centroid_k*2;
    
    if(centroid_k > cluster)
    {
      if(centroid_k - two < cluster)
      {

        two = cluster - (centroid_k - two);
        centroid_k = cluster;
        for(int i = 0 ; i < two ; i++)
        {
          for(int j = 0 ; j < centroid_k ;j++)
          {
            if(decent(i) == cluster_data(j,1))
            {
              for(int k = 0 ; k < data_dimension ; k++)
              {
                weight(centroid_k - two +i,k) = weight(j,k) + small(0,k);
              }
              break;
            }
          }
        }


        continue;
      }
      break;
    }


    double max;
    int max_number;


    for(int i = 0 ; i < two ; i++)
    {
      for(int j = 0 ; j < centroid_k - two ;j++)
      {
        if(decent(i) == cluster_data(j,1))
        {
          for(int k = 0 ; k < data_dimension ; k++)
          {
            weight(centroid_k - two +i,k) = weight(j,k) + small(0,k);
          }
          break;
        }
      }
    }
    two = before_centroid_k;
  }
  error.init(epoch,2);
  for(int i = 0 ; i < epoch ; i++)
  {
    error(i,0) = e(i,0);
    error(i,1) = e(i,1);
  }

}


void MY_CNN :: WCNN_learning()
{



  mean.print();
  variance.print();
  wcnn_weight.print();
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


  centroid_k = 2;

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
          double win_temp = 1;

          for(int n = 0 ; n < data_dimension ; n++) win_temp= win_temp/wcnn_weight(i,n);

          cluster_data(winner,0) = cluster_data(winner,0) + win_temp;
          origin_maching(i,0) = winner;
          for(int k = 0 ; k < data_dimension ; k++)
          {
            weight(winner,k) = weight(winner,k)+(origin(i,k)-weight(winner,k))/cluster_data(winner,0)*win_temp;
          }    
        }
        else
        {
          int before =  (int)origin_maching(i,0); 
          if(winner != before)
          {
            loser++;
            double win_temp = 1;

            for(int n = 0 ; n < data_dimension ; n++) win_temp= win_temp/wcnn_weight(i,n);


            origin_maching(i,0) = winner;
            cluster_data(winner,0) = cluster_data(winner,0) + win_temp;
            cluster_data(before,0) = cluster_data(before,0) - win_temp;

            for(int k = 0 ; k < data_dimension ; k++)
            { 
              weight(winner,k) = weight(winner,k)+(origin(i,k)-weight(winner,k))/cluster_data(winner,0)*win_temp;
              weight(before,k) = weight(before,k)-(origin(i,k)-weight(before,k))/cluster_data(before,0)*win_temp;

            }

          }
        }
        //   weight.print(); 만약 실시간 그래프를 하려면 이 타이밍에 출력
      }
      epoch++; 
    }while(loser != 0);

    //-----------------------------------------------------------
    system("clear"); 
    cout<<"-------wcnn-----------"<<endl;
    cout<<"percent : "<<this->get_percent()*100<<"%"<<endl;
    cout<<"k : "<<centroid_k<<endl;
    cout<<"epoch : "<<epoch<<endl;
    cout<<"----------------------"<<endl;
    //-----------------------------------------------------------

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
  cluster = weight.get_column();
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
  this->centroid_k = 2;

}
MY_CNN ::~ MY_CNN()
{

}




