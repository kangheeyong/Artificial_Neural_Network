#include "my_interface.hpp"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>



MY_INTERFACE :: MY_INTERFACE()
{
  fname_limit = 0;

}
MY_INTERFACE :: ~MY_INTERFACE()
{
}

void MY_INTERFACE :: print_menu()
{
  cout<<endl;
  cout<<"1. training"<<endl;
  cout<<"2. mapping"<<endl;
  cout<<"3. setting"<<endl;
  cout<<"4. exit"<<endl;
}
void MY_INTERFACE :: print_training()
{
  system(SCREEN_CLEAR);
  cout<<"1. random weight"<<endl;
  cout<<"2. sellect a weight"<<endl;
}
void MY_INTERFACE :: print_mapping()
{

}
void MY_INTERFACE :: print_setting()
{}

void MY_INTERFACE :: search_file(char* path)
{
   DIR            *dir_info;
   struct dirent  *dir_entry;
   fname_limit = 0;

   dir_info = opendir(path);              // 현재 디렉토리를 열기
   if ( NULL != dir_info)
   {
      while( dir_entry   = readdir( dir_info))  // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
      {
        if(strcmp( dir_entry->d_name, ".") == 0) continue;
        if(strcmp( dir_entry->d_name, "..") == 0) continue;
         strcpy( fname[fname_limit++], dir_entry->d_name);
      }
      closedir( dir_info);
   }
}
char* MY_INTERFACE :: sellect_file(int num)
{
  strcpy( recent_name,INPUT_DATA_PATH);
  strcat( recent_name, fname[num]);
  strcpy( buff,fname[num]);
  return fname[num];
}
char* MY_INTERFACE :: sellect_dir(int num)
{
  strcpy( weight_name,"output_data/");
  strcat( weight_name,fname[num]);
  strcat( weight_name,"/weight.txt");

  return weight_name;
}
char* MY_INTERFACE :: get_recent_name()
{
  return recent_name;
}
char* MY_INTERFACE :: get_recent_weight()
{
  return weight_name;
}
char* MY_INTERFACE :: get_recent_dir()
{
  char temp[100];
  strcpy( recent_dir,OUTPUT_DATA_PATH);
  strcat( recent_dir, buff);
  strcpy( temp,OUTPUT_PATH);
  strcat( temp, buff);

  for(int i = 0 ; i < 100 ; i++)
  {
    if(recent_dir[i] == '.')
    {
      recent_dir[i] = 0;
      temp[i] = 0;
      break;
    }
  }
  mkdir(temp,0755);
  strcat( recent_dir, "/");
  return recent_dir;

}
void MY_INTERFACE :: print_file()
{
  system(SCREEN_CLEAR);
  for(int i = 0 ; i < fname_limit ; i++)
  {
    printf("%2d. %s\n",i,fname[i]);
  }
}

