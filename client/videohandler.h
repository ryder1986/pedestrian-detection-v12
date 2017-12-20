#ifndef VIDEOHANDLER_H
#define VIDEOHANDLER_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <QObject>
#include "yuvrender.h"
using namespace cv;
class VideoHandler : public QObject
{
    Q_OBJECT
public:
    explicit VideoHandler(QObject *parent = 0){
        p_render=new YuvRender();
      //  p_render->show();
    }
    ~VideoHandler()
    {

        delete p_render;
    }

    void set_frame(Mat *frm){
        p_render->render_set_mat(*frm);
    }
    void set_null_frame(){
        Mat mt;
        mt.resize(0);
        p_render->render_set_mat(mt);
    }
    bool  work(QByteArray  &ba)
    {
        return true;
    }
    QWidget *get_render()
    {
       return p_render;
    }

signals:

public slots:
    void set_layout_infomation(QByteArray ba)
    {
        p_render->set_rect(ba);
    }

private:
    YuvRender *p_render;
};

#endif // VIDEOHANDLER_H
