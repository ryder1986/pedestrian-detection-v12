#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "ui_form.h"
#include "client.h"
#include "common.h"
#include "camera.h"
class ClientCameraManager:public CameraManager{
    Q_OBJECT
public:
    ClientCameraManager() :CameraManager((char *)"config.json-client")
    {

    }
    ~ClientCameraManager()
    {

    }

    void reconfig_camera(QGridLayout *g)
    {
        QList <Camera *> &c=get_cam();
        use_camera_config();// delete all exist cameras , add all configured cameras;
        int size=get_size();
        for(int i=0;i<size;i++){
            QWidget *render=(c[i])->get_render();//get render from camera
            g->addWidget(render,(i)/4,(i)%4);//set camera in window
        }

    }
public slots:
signals:
private:

};
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

signals:

public slots:
    void add_camera(bool checked);
    void del_camera(bool checked);
    void submit_camera(bool checked);
  private slots:
    void on_pushButton_search_clicked();

    void on_treeWidget_devices_customContextMenuRequested(const QPoint &pos);

    void on_treeWidget_devices_doubleClicked(const QModelIndex &index);

private:
    Ui::Form *f;
    ClientCameraManager *cam_manager;
    Client *client;
    char buf[BUF_MAX_LEN];

    QTreeWidgetItem *itm_root;
    int playing_index;
Config *p_cfg;
    QTreeWidgetItem *itm;
};

#endif // MAINWIDGET_H
