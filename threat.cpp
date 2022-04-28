#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <queue>
#include <cstdio>
#include <list>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <sstream>
#include <map>
#include <filesystem>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include<time.h>
#include <sys/time.h>
#include<fcntl.h>
#include <semaphore.h>
#include <sys/shm.h>
sem_t shared;
using namespace std;
struct timeval ;
struct mes
{
    int start_size;
    unsigned char* mytext;
    unsigned long long size;
    int stop_size;
}m;
void * thread_func(void *arg)
{
    int stop=m.stop_size;
    int start=m.start_size;
    sem_post(&shared);
    for(int i=start;i<stop;i=i+3)
    {
        m.mytext[i]=(m.mytext[i]+m.mytext[i+1]+m.mytext[i+2])/3.0f;
       m.mytext[i+1]=m.mytext[i];
        m.mytext[i+2]=m.mytext[i];
    }
    pthread_exit(NULL);
}
int main()
{
    sem_init(&shared,0,0);
    float delta;
    struct timeval start, end;
    int  threads;
    pthread_t thread[12];
    for(;;)
    {
    cout<<"clear window (y/n)?"<<endl;
    char var;
    cin>>var;
    if(var=='y')
         system("clear");
    else if (var=='n')
            cout<<"OK"<<endl;
    else cout<<"we can't clean the window"<<endl;
    FILE *photo;
    string bmp_name;
    cout<<"input bmp name"<<endl;
    cin>>bmp_name; 
    string way="/home/vova/laba6/";
    photo=fopen(bmp_name.c_str(),"rb");
    if(bmp_name=="0") 
    {
        system("clear");
        return 0;
    }
    if(!photo) 
    {
        cout<<"warning!!"<<endl;
        return 0;
    }
    
     gettimeofday(&start, NULL);
    FILE *result3;
    result3=fopen("result3.bmp","wb");
    unsigned char* mytext;
    fseek(photo,0,SEEK_END);
    unsigned long long size=ftell(photo);
    rewind(photo);
    mytext=(unsigned char*)malloc((size+1)*sizeof(unsigned char));
    fread(mytext,size*sizeof(unsigned char),1,photo);
    m.mytext=mytext;
    m.size=size;
    m.start_size=54;
    for(int i=0;i<12;i++)
    {
        m.stop_size=m.start_size+(size-54)/12;
        threads = pthread_create(&thread[i], NULL, thread_func, &m); 
        sem_wait(&shared);
        m.start_size=m.start_size+(size-54)/12;
    } 
    for(int i=0;i<12;i++)
    {
        threads = pthread_join(thread[i], NULL); 
    } 
    if (threads!= 0)                    
    {
        perror("Ждём поток");
        return EXIT_FAILURE;
    }
    fwrite(m.mytext,size,1,result3);
    gettimeofday(&end, NULL);
    delta = ((end.tv_sec  - start.tv_sec) * 1000000u+ end.tv_usec - start.tv_usec) / 1.e6;
    printf("Time is : %f\n",delta);
    free(mytext);
    fclose(result3);
    rewind(photo);
    
    gettimeofday(&start, NULL);
    FILE *result1;
    result1=fopen("result1.bmp","wb");
    fseek(photo,0,SEEK_END);
    size=ftell(photo);
    rewind(photo);
    mytext=(unsigned char*)malloc((size+1)*sizeof(unsigned char));
    fread(mytext,size*sizeof(unsigned char),1,photo);
    for(int i=55;i<size;i=i+3)
    {
        mytext[i]=(mytext[i]+mytext[i+1]+mytext[i+2])/3.0f;
        mytext[i+1]=mytext[i];
        mytext[i+2]=mytext[i];
    }
    fwrite(mytext,size,1,result1);
    gettimeofday(&end, NULL);
    delta = ((end.tv_sec  - start.tv_sec) * 1000000u+ end.tv_usec - start.tv_usec) / 1.e6;
    printf("Time is : %f\n",delta);
    fclose(result1);
    sem_destroy(&shared);
    }
    return 0;
}
