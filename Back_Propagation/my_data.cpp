/*
ver 1.1 2016.12.30
비교연산자 추가
print 함수 20개 이상은 생략해서 보여줌
timer 함수 한줄만 남은 시간보이게 함
*/

#include "my_data.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

MY_DATA :: MY_DATA()
{

  srand(time(NULL));
  this->row = 0;
  this->column = 0;
  this->data = NULL;
  start_point = clock();
}

void MY_DATA ::init()
{
  if(this->data != NULL) delete this->data;
  this->row = 0;
  this->column = 0;
  this->data = NULL;
}
void MY_DATA ::init(int y, int x)
{
  this->init();
  this->data = new double[x*y];
  this->row = x;
  this->column = y;
  for(int i = 0 ; i < x*y ; i++) this->data[i] = 0.0;
}
void MY_DATA ::init(int y)
{
  this->init();
  this->data = new double[y];
  this->row = 1;
  this->column = y;
  for(int i = 0 ; i < y ; i++) this->data[i] = 0.0;
}


void MY_DATA ::random()
{

  for(int i = 0 ; i < this->column ; i++)
  {
    for(int j = 0 ; j < this->row ; j++)
    {
      this->data[i * this->row + j] = (double)(rand()%100000)/100000;
    }
  }
}
void MY_DATA ::random(double min, double max)
{
  int temp = (max - min) *100000;

  for(int i = 0 ; i < this->column ; i++)
  {
    for(int j = 0 ; j < this->row ; j++)
    {
      this->data[i * this->row + j] = (double)(rand()%temp)/100000 + min;
    }
  }
}
void MY_DATA ::suffle()
{
  double *temp = new double[this->row];
  int num;
  for(int i = 0 ; i < this->column ; i++)
  {
    num = rand()%this->column;
    for(int j = 0 ; j < this->row ; j++)
    {
      temp[j] = this->data[i * this->row + j];
      this->data[i * this->row + j] = this->data[num * this->row + j];
      this->data[num * this->row + j] = temp[j];
    }
  }

  delete temp;

}
void MY_DATA ::timer(int now, int total)
{
  end_point = clock();
  
  int hour= (int)((total-now)*(double)(end_point - start_point)/CLOCKS_PER_SEC)/3600;
  int min = (int)((total-now)*(double)(end_point - start_point)/CLOCKS_PER_SEC)%3600/60;
  int sec = (int)((total-now)*(double)(end_point - start_point)/CLOCKS_PER_SEC)%60;


  if(now % PRINT_PER_EPOCH == 0)
  {
    printf("\033[A");
    printf("%6.2lf",(double)now/(double)total*100);
    cout<<"%";
    printf("%4dhour %3dmin %3dsec",hour,min,sec);
    cout<<endl; 
  }

  start_point = end_point;
}
const int MY_DATA ::get_row()
{
  return this->row;
}
const int MY_DATA ::get_column()
{
  return this->column;
}

bool MY_DATA ::fread(const char *fname)
{
   FILE *fp;

  this->init();

   if(fp = fopen( fname, "r"))
   {
     char buf[BUFF_SIZE];

     while( fgets( buf, BUFF_SIZE, fp)) // 가로와 세로의 개수 구하기
     {
       bool delta[2] = {0,0};
       for(int i = 0; i < BUFF_SIZE ; i++)
       {
         if(buf[i] == '\n') break;

         else if(buf[i] == '.'||buf[i] == '+'||buf[i] == '-'||(buf[i] >= '0' && buf[i] <= '9') )
         {
           delta[0] = delta[1];
           delta[1] = 1;
         }
         else
         {
           delta[0] = delta[1];
           delta[1] = 0;
         }
         if(delta[0] == 0 && delta[1] == 1) this->row++;

       }
       this->column++;

     }
     this->row = this->row / this->column; // 열(row)의 개수는 일정해야함
     rewind( fp);   // 파일 읽기/쓰기 위치를 처음 위치로 이동
     this->data = new double[this->row*this->column]; //동적할당 받기 1차원 배열을 2차원 배열처럼 쓰자

     double temp;
     for(int i = 0 ; i < this->column ; i++)
     {
       for(int j = 0 ; j < this->row ; j++)
       {
         fscanf(fp,"%lf",&temp);
         this->data[i * this->row + j] = temp; //값 읽기 오로지 공백으로만 검출
       }
     }
     fclose(fp);
     return 1;
   }
   else
   {
     cout<<"fail to read"<<endl;
     return -1;
   }
}

bool MY_DATA ::fwrite(const char *fname)
{
  FILE *fp;
  if(fp = fopen( fname, "w"))
  {
    for(int i = 0 ; i < this->column ; i++)
    {
      for(int j = 0 ; j < this->row ; j++)
      {
        fprintf(fp,"%lf ",this->data[i * this->row + j]);
      }
      fprintf(fp,"\n");
    }
    fclose(fp);
    return 1;
  }
  else
  {
    cout<<"fail to write"<<endl;
    return -1;
  }
}
const double&  MY_DATA ::operator()(int y) const
{
  return this->data[y];
}

double&  MY_DATA ::operator()(int y)
{
  return this->data[y];
}

const double&  MY_DATA ::operator()(int y, int x) const
{
  return this->data[y*this->row+x];
}

double&  MY_DATA ::operator()(int y, int x)
{
  return this->data[y*this->row+x];
}
MY_DATA& MY_DATA ::operator=(const MY_DATA &other)
{
 
  
  MY_DATA temp;
  temp.init(other.column,other.row);

   for(int i = 0 ; i < other.column ; i++)
  {
    for(int j = 0 ; j < other.row ; j++)
    {
      temp.data[i*other.row + j] = other.data[i*other.row + j];
    }
  }

  this->init(other.column, other.row);

  for(int i = 0 ; i < this->column ; i++)
  {
    for(int j = 0 ; j < this->row ; j++)
    {
      this->data[i*this->row + j] = temp.data[i*this->row + j];
    }
  }
  return *this;
}
MY_DATA& MY_DATA ::operator+(const MY_DATA &other)
{

  MY_DATA  temp;

  temp = *this;


  for(int i = 0 ; i < this->column ; i++)
  {
    for(int j = 0 ; j < this->row ; j++)
    {
      this->data[i*this->row + j] =  temp.data[i*this->row + j] + other.data[i*this->row + j];
    }
  }

  return *this;
}
MY_DATA& MY_DATA ::operator-(const MY_DATA &other)
{

  MY_DATA  temp;

  temp = *this;
  for(int i = 0 ; i < this->column ; i++)
  {
    for(int j = 0 ; j < this->row ; j++)
    {
      this->data[i*this->row + j] =  temp.data[i*this->row + j] - other.data[i*this->row + j];
    }
  }
  return *this;
}

bool MY_DATA ::print()
{
  if(this->data == NULL) return -1;
  else
  {
    cout<<endl;
    if(this->column < 21)
    {
      for(int i = 0 ; i < this->column ; i++)
      {
        for(int j = 0 ; j < this->row ; j++)
        {
          cout << this->data[i*this->row + j]<<" ";
        }
        cout<<endl;
      }
    }
    else
    {
      for(int i = 0 ; i < 10 ; i++)
      {
        for(int j = 0 ; j < this->row ; j++)
        {
          cout << this->data[i*this->row + j]<<" ";
        }
        cout<<endl;
      }
      cout<<"..."<<endl;
      for(int i = this->column-10 ; i < this->column ; i++)
      {
        for(int j = 0 ; j < this->row ; j++)
        {
          cout << this->data[i*this->row + j]<<" ";
        }
        cout<<endl;
      }
    }
    cout<<endl;
  }
}

MY_DATA :: ~MY_DATA()
{
  MY_DATA ::init();
}
