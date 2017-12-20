#ifndef TOOLS_H
#define TOOLS_H
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstring>
#include <mutex>
#include <thread>
#include <bitset>
#include <list>
using namespace std;
#include <iterator>
class Tools
{

public:
    enum FIXED_VALUE{
        PATH_LENGTH=200,
        BUF_LENGTH=200,
        BUF_MAX_LENGTH=2000
    };
    static mutex lock;
private:
    int cc=0;
    static  char filename[FIXED_VALUE::PATH_LENGTH];
    const int static buf_size=200;
public:

    Tools()
    {
    }
    inline static string last_substr(string str,char split_ch)
    {
        char tmp[BUF_MAX_LENGTH];
        memset(tmp,0,BUF_MAX_LENGTH);
        strcpy(tmp,str.data());
        char *p=tmp;
        int total=0;
        char *p1=p;
        while(0!=*(++p)){
            if(*p==split_ch){
                p1=p+1;
                total++;
            }
        }
        return string(p1);
    }

    inline static void prt(const char *buf,const int line_no,const char *func_name,const char *file_name,const char *label,const char *time)
    {
        char buffer[buf_size];
        memset(buffer,0,buf_size);
        memcpy(buffer,time,strlen(time));
        int i;
//        for( i=0;buffer[i]!='\n';i++)
//            ;
//        buffer[i]='\0';

        //   cout<<"("<<buf<<")"<<'['<<line_no<<']'<<'['<<func_name<<']'<<'['<<file_name<<']'<<'['<<buffer<<']'<<'['<<label<<']'<<endl;
        //  cout<<"("<<buf<<")"<<'['<<line_no<<']'<<'['<<func_name<<']'<<'['<<file_name<<']'<<'['<<buffer<<']'<<'['<<label<<']'<<endl;

        string fn(file_name);
#if 0
#ifdef  IS_UNIX
        char t='/';
#else
        char t='\\';
#endif
        cout<<last_substr(fn,t).data()<<"(line"<<line_no<<")"<<":"<<buf<<endl;
#endif
   //     cout<<last_substr(last_substr(fn,'/'),'\\').data()<<"(line"<<line_no<<")"<<":"<<buf<<endl;
         cout<<"["<<time<<"]["<<label<<"]["<<last_substr(last_substr(fn,'/'),'\\').data()<<":"<<line_no<<"]===>"<<buf<<endl;
        fflush(NULL);
    }
    inline static char* get_time_n()
    {
        chrono::system_clock::time_point today= chrono::system_clock::now();
        time_t tt= chrono::system_clock::to_time_t(today);
        return ctime(&tt);
    }
    inline static string get_time()
    {
        chrono::system_clock::time_point today= chrono::system_clock::now();
        time_t tt= chrono::system_clock::to_time_t(today);
        string tm(ctime(&tt));
        int pos=tm.find('\n');
        if(pos!=string::npos){
            tm[pos]=0;
        }

        return tm;
    }
    static void init(const char *)
    {
    }
    static int  aaaa;
    static const int ss=123;
    enum test{
        ABC=1,
        BCD=2
    };
    typedef test sss;

    void aaa()
    {



    }
};
#define prt(label,...) {Tools::lock.lock(); Tools::init("log.txt"); char buf[1000];sprintf(buf,__VA_ARGS__);\
    Tools::prt(buf,__LINE__,__FUNCTION__,__FILE__,#label,Tools::get_time().data());Tools::lock.unlock();}


#define THREAD_DEF(cls,fun) new thread(std::mem_fn(&cls::fun),*(cls*)this);


/*

head defination:

    /    head  /   /  body   /
    -- -- -- --    ---.......
    byte1-2:data body length,byte 3-4:protocol version, byte 5-6:operation,byte 7-8:reply rst,9->:body

*/

class Protocol{
public :
    const int static camera_max_num=8;
    enum VER{
        VERSION=1
    };
    enum LEN{
        HEAD_LENGTH=8
    };

    enum AABB{
        SS=0,
        PP=1
    };
    enum CMD{
        GET_CONFIG,
        ADD_CAMERA,
        DEL_CAMERA,
        MOD_CAMERA
    };
    enum RET{
        RET_SUCCESS,
        RET_FAIL,
        RET_REFRESH
    };
    enum PORTS{
        SERVER_PORT=12345,
        SERVER_DATA_OUTPUT_PORT=12346,
        CLIENT_REPORTER_PORT=12347,
        SERVER_REPORTER_PORT=12348
    };

    static void pkg_set_len(char *c,int len)//encode length of pkg
    {
        char *dst=c;
        short *p_len=( short *)dst;
        *p_len=len;
    }
    static int pkg_get_len(char *c)//decode length of pkg
    {
        char *dst=c;
        short *p_len=( short *)dst;
        return *p_len;
    }
    static void pkg_set_ret(char *c,int ret)//encode ret of pkg
    {
        char *dst=c+2+2+2;
        short *p_ret=( short *)dst;
        *p_ret=ret;
    }
    static int pkg_get_ret(char *c)//decode ret of pkg
    {
        char *dst=c+2+2+2;
        short *p_ret=( short *)dst;
        return *p_ret;
    }
    static void pkg_set_version(char *c,int version)
    {
        char *dst=c+2;
        short *p_version=( short *)dst;
        *p_version=version;
    }
    static int pkg_get_version(char *c)
    {
        char *dst=c+2;
        short *p_version=( short *)dst;
        return *p_version;
    }
    static void pkg_set_op(char *c,int op)
    {
        char *dst=c+2+2;
        short *p_op=( short *)dst;
        *p_op=op;
    }
    static int pkg_get_op(char *c)
    {
        char *dst=c+2+2;
        short *p_op=( short *)dst;
        return *p_op;
    }

    static int encode_configuration_request(char *buf){
        //   pkg_set_len(ba);
        memset(buf,0,Tools::BUF_LENGTH);
        pkg_set_len(buf,0);
        pkg_set_version(buf,VERSION);
        pkg_set_op(buf,GET_CONFIG);
        pkg_set_ret(buf,RET_SUCCESS);
        return HEAD_LENGTH;
    }
    static int encode_configuration_reply(char *buf,int len,int ret){
        //   pkg_set_len(ba);
        //   memset(buf,0,BUF_MAX_LEN);
        pkg_set_len(buf,len);
        pkg_set_version(buf,VERSION);
        pkg_set_op(buf,GET_CONFIG);
        pkg_set_ret(buf,ret);
        return HEAD_LENGTH;
    }
    static int encode_addcam_request(char *buf,int len){
        //   pkg_set_len(ba);
        memset(buf,0,Tools::BUF_LENGTH);
        pkg_set_len(buf,len);
        pkg_set_version(buf,VERSION);
        pkg_set_op(buf,ADD_CAMERA);
        pkg_set_ret(buf,RET_SUCCESS);
        return HEAD_LENGTH+len;
    }
    static int encode_delcam_request(char *buf,int index){
        //   pkg_set_len(ba);
        memset(buf,0,Tools::BUF_LENGTH);
        pkg_set_len(buf,0);
        pkg_set_version(buf,VERSION);
        pkg_set_op(buf,DEL_CAMERA);
        pkg_set_ret(buf,index);
        return HEAD_LENGTH;
    }
    static int get_operation(char *buf){
        return pkg_get_op(buf);
    }
    static int get_length(char *buf){
        return pkg_get_len(buf);
    }
    static int get_cam_index(char *buf){
        return pkg_get_ret(buf);
    }
};

class Timer{
    typedef void (*fun0)();
    typedef void (*fun1)(void *);
    typedef struct data{
        int duration;
        fun0 timer_func;
        fun1 timer_func1;
        bool quit;
        bool start;
        void *dt;
    }data_t;

public:
    Timer(fun0 fun,int dur){
        //d.th=new thread(run);
        d.duration=dur;
        d.quit=false;
        d.start=false;
        d.timer_func=fun;
        d.dt=NULL;
        th=new thread(run,&d);

    }
    Timer(fun1 fun,void *dat,int dur){
        //d.th=new thread(run);
        d.duration=dur;
        d.quit=false;
        d.start=false;
        d.timer_func1=fun;
        d.dt=dat;
        th=new thread(run,&d);

    }
    ~Timer()
    {
        d.quit=true;
        th->join();
        delete th;
    }

    void start()
    {
        d.start=true;
    }

    void stop()
    {
        d.start=false;
    }
    static void run(data_t *p_d)
    {
        while(!p_d->quit){
            if(!p_d->start){
                this_thread::sleep_for(chrono::milliseconds(1));
                continue;
            }
            //   prt(info,"runing timer");
            if(p_d->dt==NULL)
                p_d->timer_func();
            else
                p_d->timer_func1(p_d->dt);
            this_thread::sleep_for(chrono::milliseconds(p_d->duration));
        }
    }

private:
    data_t d;
    thread *th;
};

#endif // TOOLS_H
