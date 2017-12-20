#ifndef YUVRENDER_H
#define YUVRENDER_H

#include <QOpenGLWidget>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <QImage>
#include "config.h"
using namespace cv;
using namespace std;
#include <QGLWidget>
#include "common.h"

class  VideoReader:public QObject{
    Q_OBJECT
public:
    bool video_connected_flag;
    void restart()
    {

        cvReleaseCapture(&p_cap);
        p_cap= cvCreateFileCapture(url);  //create video source

    }
    VideoReader(QString path):p_cap(NULL)
    {

        video_connected_flag=true;
        memset(url,0,1000);
        strcpy(url,path.toStdString().data());
        p_cap= cvCreateFileCapture(url);  //create video source
        width=cvGetCaptureProperty(p_cap,CV_CAP_PROP_FRAME_WIDTH);
        height=cvGetCaptureProperty(p_cap,CV_CAP_PROP_FRAME_HEIGHT);
        //    prt(info,"video widtbh %d  ",ret);
        if(p_cap==NULL){
            prt(info,"video src start  %s err  ",url);
            video_connected_flag=false;
        }
        else {
            prt(info,"video src  start %s ok  ",url);
        }
        //        if(p_cap==NULL)
        //            emit video_disconnected();
        //        else
        //            emit video_connected();




        //    timer=new QTimer();
        //  tmr->singleShot(1000,this,SLOT(time_up()));

        //    prt(info," shot afer 100 ms")
        // QTimer::singleShot(1000,this,SLOT(time_up()));
        //   connect(timer,SIGNAL(timeout()),this,SLOT(time_up()));
        //   timer->start(wait_duration);
    }
    ~VideoReader()
    {
        //   cap_lock.lock();
        // timer->stop();
        //  disconnect(timer,SIGNAL(timeout()),this,SLOT(time_up()));
        //   delete timer;
        //     QThread::sleep(1);
        //   prt(info," delete src");
        //    disconnect(tmr,SIGNAL(timeout()),this,SLOT(time_up()));
        cvReleaseCapture(&p_cap);
        p_cap=NULL;
        //    cap_lock.unlock();
        //   delete tmr;
        //   delete p_cap;
    }
    Mat *get_frame()
    {

        //     tmr->singleShot(10,this,SLOT(time_up()));
        int err=0;
        if(p_cap==NULL){
            video_connected_flag=false;
            err=1;
            //   emit video_disconnected();
        }
        IplImage *ret_img;

        //            prt(info,"try to grb");
        //        int tmp= cvGrabFrame(p_cap);
        //             prt(info,"grub source url:%s ret %d (%p)",url,tmp,p_cap);
        //        ret_img= cvRetrieveFrame(p_cap);
        //   prt(info,"try to query");
        //    CV_CAP_PROP_XI_TIMEOUT
        //CV_CAP_PROP_FRAME_WIDTH
        //    int ret=cvSetCaptureProperty(p_cap,CV_CAP_PROP_XI_TIMEOUT,999);
        // double pro=cvGetCaptureProperty(p_cap,CV_CAP_PROP_XI_TIMEOUT);
        //  prt(info," set %d ,opecv time out %d",ret ,pro);
        //      CV_CAP_PROP_XI_TIMEOUT
        //prt(info,"  start query 1 frame ");
        ret_img=cvQueryFrame(p_cap);
        Mat(ret_img).copyTo(mat_rst);
        if(ret_img==NULL){
            err=1;
            //     std::this_thread::sleep_for(chrono::milliseconds(1000));
            //    QThread::sleep(1);
            if(video_connected_flag==true)
            {
                //    prt(info,"%s disconnected",url);
                video_connected_flag=false;
            }
        }else{
            if(video_connected_flag==false)
            {
                //     prt(info,"%s connected",url);
                video_connected_flag=true;
            }
        }
        if(err)
            return NULL;
        else
            return &mat_rst;

    }
    char *get_url(){
        return url;
    }
public slots:

signals:
private:
    CvCapture *p_cap;

    int width;
    int height;
    Mat mat_rst;
public:
    char url[1000];
};


#include <thread>
#include <mutex>
#include <iostream>
using namespace std;
class YuvRender : public QOpenGLWidget
{
    Q_OBJECT
    typedef struct data{
        int *p_x;
        YuvRender *p;
        VideoReader *r;
        std::mutex lk;
        int quit;
    }data_t;
    data_t d;
public:
    std::thread *read_thread;
    //   VideoReader *r;
    static void fun(void *addr)
    {

        data_t *p_d=( data_t *)addr;
        YuvRender *p=(YuvRender *) p_d->p;
        while(!p_d->quit)
        {
             //    *(p_d->p_x)+=10;
            p_d->lk.lock();

            Mat *mt= p_d->r->get_frame();
            if(mt){
                p->render_set_mat(*mt);
                p->update();

            }else{
                p_d->r->restart();
            }

            p_d->lk.unlock();
            QString ss=p_d->r->get_url();
            if(!ss.contains(QString("rtsp"))&&!ss.contains(QString("http")))
            {

                this_thread::sleep_for(chrono::milliseconds(50));
               // prt(info,"sleep for file");
            }


        }
    }
public:
    void start(QString  url)
    {
        d.lk.lock();

        std::cout<<url.toStdString().data()<<std::endl;
        d.quit=false;
        if(d.r)
            delete d.r;
        d.r=new VideoReader(url);
        if(!read_thread)
            read_thread=new std::thread(fun,&d);
        //     read_thread->detach();
        d.lk.unlock();
    }

    void stop()
    {

        d.lk.lock();

        std::cout<<d.r->url<<std::endl;

        if(read_thread)
        {
            d.quit=true;

#if 0
            read_thread->detach();
#else
              read_thread->join();
#endif
            delete read_thread;
            read_thread=NULL;
        }
        delete d.r;

        d.r=NULL;

        d.lk.unlock();

    }
    // explicit YuvRender(QWidget *parent = 0);
    YuvRender(QWidget *parent=0) :
        QOpenGLWidget(parent),video_width(VIDEO_WIDTH),video_height(VIDEO_HEIGHT),pressed_x(0),pressed_y(0)
    {
read_thread=NULL;
        //  r=new VideoReader();
        pos_x=100;
        pos_y=100;
        tick=0;
        d.quit=false;
        d.r=NULL;
        d.p_x=&pos_x;
        d.p=this;
        frame.resize(VIDEO_WIDTH*VIDEO_HEIGHT*3);
        frame=Mat(VIDEO_WIDTH,VIDEO_HEIGHT,CV_8UC3);

        memset(frame.data,100,VIDEO_WIDTH*VIDEO_HEIGHT*3);
    }
    static unsigned char  CONVERT_ADJUST(double tmp)
    {
        return (unsigned char)((tmp >= 0 && tmp <= 255)?tmp:(tmp < 0 ? 0 : 255));
    }
    //YUV420P to RGB24
    static  void   CONVERT_YUV420PtoRGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth,int nHeight)
    {
        unsigned char *tmpbuf=(unsigned char *)malloc(nWidth*nHeight*3);
        unsigned char Y,U,V,R,G,B;
        unsigned char* y_planar,*u_planar,*v_planar;
        int rgb_width , u_width;
        rgb_width = nWidth * 3;
        u_width = (nWidth >> 1);
        int ypSize = nWidth * nHeight;
        int upSize = (ypSize>>2);
        int offSet = 0;

        y_planar = yuv_src;
        u_planar = yuv_src + ypSize;
        v_planar = u_planar + upSize;

        for(int i = 0; i < nHeight; i++)
        {
            for(int j = 0; j < nWidth; j ++)
            {
                // Get the Y value from the y planar
                Y = *(y_planar + nWidth * i + j);
                // Get the V value from the u planar
                offSet = (i>>1) * (u_width) + (j>>1);
                V = *(u_planar + offSet);
                // Get the U value from the v planar
                U = *(v_planar + offSet);

                // Cacular the R,G,B values
                // Method 1
                R = CONVERT_ADJUST((Y + (1.4075 * (V - 128))));
                G = CONVERT_ADJUST((Y - (0.3455 * (U - 128) - 0.7169 * (V - 128))));
                B = CONVERT_ADJUST((Y + (1.7790 * (U - 128))));
                /*
                  // The following formulas are from MicroSoft' MSDN
                  int C,D,E;
                  // Method 2
                  C = Y - 16;
                  D = U - 128;
                  E = V - 128;
                  R = CONVERT_ADJUST(( 298 * C + 409 * E + 128) >> 8);
                  G = CONVERT_ADJUST(( 298 * C - 100 * D - 208 * E + 128) >> 8);
                  B = CONVERT_ADJUST(( 298 * C + 516 * D + 128) >> 8);
                  R = ((R - 128) * .6 + 128 )>255?255:(R - 128) * .6 + 128;
                  G = ((G - 128) * .6 + 128 )>255?255:(G - 128) * .6 + 128;
                  B = ((B - 128) * .6 + 128 )>255?255:(B - 128) * .6 + 128;
                  */
                offSet = rgb_width * i + j * 3;

                rgb_dst[offSet] = B;
                rgb_dst[offSet + 1] = G;
                rgb_dst[offSet + 2] = R;
            }
        }
        free(tmpbuf);
    }



    void paint_layout1(QPainter &painter){
#if 0
        painter.beginNativePainting();
        makeCurrent();
        CONVERT_YUV420PtoRGB24((unsigned char *)yuv_buf,(unsigned char *)rgb_buf,video_width,video_height);
        glPixelZoom(1.0f*this->width()/video_width, 1.0f*this->height()/video_height);
#if 0
        glDrawPixels(video_width, video_height,GL_RGB, GL_UNSIGNED_BYTE, rgb_buf);
#else
        QImage img((uchar *)rgb_buf,video_width,video_height,QImage::Format_RGB888);
        //  QImage img((uchar *)rgb_buf,video_width,video_height,QImage::Format_RGB888);
        //QImage  img = QImage((const uchar*)(frame.data),frame.cols,frame.rows,frame.cols*frame.channels(),QImage::Format_Indexed8);
        //   QImage  img = QImage((const uchar*)(frame.data),frame.cols,frame.rows,frame.cols*frame.channels(),QImage::Format_Grayscale8);

        painter.drawImage(QRect(0,0,this->width(),this->height()),img);
#endif
        painter.endNativePainting();
#else
        painter.beginNativePainting();
        makeCurrent();
        //   QImage  img = QImage((const uchar*)\
        (frame.data),frame.cols,frame.rows,frame.cols*frame.channels(),\
                QImage::Format_Indexed8);
        //   QImage  img = QImage((const uchar*)\
        (frame.data),frame.cols,frame.rows,frame.cols*frame.channels(),\
                QImage::Format_RGB888);
        //  char t=*frame.data;

        Mat rgb_frame=frame;
        // cvtColor(frame,rgb_frame,CV_YUV2BGR);
        Mat yuv_frame;
        // imwrite("test.jpg",rgb_frame);
        int test=*rgb_frame.data;

        // if(20<test<128){
        //   if(1){
        ////     cvtColor(rgb_frame,yuv_frame,CV_RGB2GRAY);
        cvtColor(rgb_frame,yuv_frame,CV_BGR2RGB);
        QImage  img = QImage((const uchar*)(yuv_frame.data),
                             yuv_frame.cols,yuv_frame.rows,
                             yuv_frame.cols*yuv_frame.channels(),
                             QImage::Format_RGB888);
        painter.drawImage(QRect(0,0,this->width(),this->height()),img);
        //   }
        painter.endNativePainting();

#endif
    }
    void paint_layout_rect(QPainter &painter){
        QBrush red_brush_trans(QColor(0,0,200,100));
        painter.setBrush(red_brush_trans);
        painter.drawEllipse(pos_x,pos_y,50,50);
    }
    void set_rect(QByteArray ba)
    {
        QList<QByteArray> xy=ba.split(',');
        int x=xy[0].toInt();
        int y=xy[1].toInt();
        pos_x=x;
        pos_y=y;
        prt(info," %d : %d",x,y);
    }
    void paint_layout2(QPainter &painter){
#if 0
        QBrush red_brush_trans(QColor(0,0,200,100));
        painter.setBrush(red_brush_trans);
        // pressed_x=0;
        // pressed_y=0;
        //  if(flg++>10)


        painter.drawEllipse(pressed_x+tick%300,pressed_y,100,100);
        QPen testpen(red_brush_trans,10);
        painter.setPen(testpen);

        for(int i=0;i<8;i++){
            painter.drawLine(pt[i*2],pt[i*2+1]);
        }


        for(int i=0;i<8;i++){
            painter.drawLine(pt[i*2],pt[i*2+1]);
        }

        for(int i=0;i<8;i++){
            painter.drawLine(pt_rear[i*2],pt_rear[i*2+1]);
        }
        painter.drawEllipse(p1,10,10);
        painter.drawEllipse(p2,10,10);
        painter.drawEllipse(p3,10,10);
#endif
    }

    void paintEvent(QPaintEvent *)
    {
        int len=frame.cols;
        if(frame.cols>0){
            tick++;
            //prt(info,"painting");
            tick+=10;
            QPainter painter(this);
            paint_layout1(painter);
            paint_layout_rect(painter);
        }
        //  paint_layout2(painter);
    }
    void  initializeGL()
    {

    }

signals:

public slots:
    int render_set_mat(Mat frame_mat)
    {
        int size=frame_mat.total();
        if(size>0)
        {
            frame=frame_mat;
        }else
        {
            prt(info,"render set frame fail");
        }
        this->update();
        //    prt(info,"update pic menually");
        return 1;
    }

private:
    int tmp_tick;
    //    char yuv_buf[640*480*3/2];
    //    char rgb_buf[640*480*3];
    //    char rgb_buf_tmp[640*480*3];
    int video_width;
    int video_height;
    int pressed_x,pressed_y,pressed;
    int tick;
    int tickmat;
    Mat frame;
    //   Mat empty_frame;
    int pos_x;
    int pos_y;

    QPoint pt[16];
    QPoint pt_rear[16];
    QPoint p1;
    QPoint p2;
    QPoint p3;
protected:
    //  void  initializeGL();
    //  void  resizeGL(int w, int h);
    //  void  paintEvent(QPaintEvent *);


};

#endif // YUVRENDER_H
