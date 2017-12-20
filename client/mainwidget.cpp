#include "mainwidget.h"
#include "yuvrender.h"
MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    playing_index=0;
    f=new Ui::Form;
    f->setupUi(this);
//    YuvRender *r=new YuvRender;
//    r->show();
    //f->widget_pic->setLayout();
    cam_manager=new ClientCameraManager;
    client=new Client;
    f->treeWidget_devices->clear();
}


void MainWidget::on_pushButton_search_clicked()
{
    client->search_device();
    client->wait_server_info_reply(2);
    client->connect_to_server();

    f->treeWidget_devices->clear();
    int request_length=Protocol::encode_configuration_request(buf);//encoder buffer
    QByteArray rst=client->call_server(buf,request_length);//talk to server
    rst=rst.remove(0,Protocol::HEAD_LENGTH);//TODO:get the ret value

    cam_manager->save_config(rst);
    p_cfg=cam_manager->get_config(true);
    itm_root=new QTreeWidgetItem(QStringList(client->server_ip));

    f->treeWidget_devices->addTopLevelItem(itm_root);
    int i=0;
    for(i=0;i<p_cfg->data.camera_amount;i++){
        QTreeWidgetItem *itm1=new QTreeWidgetItem(QStringList(p_cfg->data.camera[i].ip));
        itm_root->addChild(itm1);
    }




}
#include <QMenu>
void MainWidget::add_camera(bool checked)
{
    qDebug()<<"checked  "<<checked;
    QTreeWidgetItem *tmp_item=new QTreeWidgetItem(QStringList("edit here"));
    itm_root->addChild(tmp_item);
    tmp_item->setFlags(Qt::ItemIsEditable | tmp_item->flags()) ;
    itm_root->setExpanded(true);
}

void MainWidget::del_camera(bool checked)
{
    qDebug()<<"del  "<<checked;
    int now=itm->parent()->indexOfChild(itm);
    qDebug()<<"index " <<now  ;
    qDebug()<<"max " <<p_cfg->data.camera_amount  ;
    if(now+1>p_cfg->data.camera_amount)
    {
        itm_root->removeChild(itm);
    }else{

        int del_index=itm->parent()->indexOfChild(itm)+1;
        if(playing_index>0){
            if(playing_index==del_index)
            {
                qDebug()<<"stop playing";
                f->openGLWidget->stop();
playing_index=0;
            }
        }
        int size=cam_manager->get_size();
        if(del_index<=size&&del_index>0)
        {

            cam_manager->del_camera(del_index);// delete camera local
            Protocol::encode_delcam_request(buf,del_index);//encode buffer
            client->call_server(buf,Protocol::HEAD_LENGTH);//talk to server
        }
        itm_root->removeChild(itm);

    }
}

void MainWidget::submit_camera(bool checked)
{
    qDebug()<<" submit  "<<itm->text(0).toStdString().data();


    QString ip=itm->text((0));
    cam_manager->add_camera(ip);//add camera on client
    QByteArray setting=cam_manager->get_config(1);//get new config from local database
    int len=Protocol::encode_addcam_request(buf,setting.length());//encode buffer
    memcpy(buf+Protocol::HEAD_LENGTH,setting.data(),setting.length());
    QByteArray rst=client->call_server(buf,len);//talk to server

   // itm->setFlags(!Qt::ItemIsEditable | itm_root->flags()) ;
    itm->setFlags(  itm->flags() & ~Qt::ItemIsEditable) ;


}

void MainWidget::on_treeWidget_devices_customContextMenuRequested(const QPoint &pos)
{
    //itm= ui->treeWidget->itemAt(pos);

#if 1
    itm= f->treeWidget_devices->itemAt(pos);
    if(itm){
        if(!itm->parent()){

            //  std::cout<<pos.x()<<","<<pos.y()<<std::endl;
            QMenu *cmenu = new QMenu(f->treeWidget_devices);
            QAction *action_add = cmenu->addAction("add");

            connect(action_add, SIGNAL(triggered(bool)), this, SLOT(add_camera(bool)));

            cmenu->exec(QCursor::pos());
        }else{
            QMenu *cmenu = new QMenu(f->treeWidget_devices);
            QAction *action_del = cmenu->addAction("del");

            int now=itm->parent()->indexOfChild(itm);
            qDebug()<<"index " <<now  ;
            qDebug()<<"max " <<p_cfg->data.camera_amount  ;
            if(now+1>p_cfg->data.camera_amount)
            {
                QAction *action_submit = cmenu->addAction("submit");
                connect(action_submit, SIGNAL(triggered(bool)), this, SLOT(submit_camera(bool)));
            }
            connect(action_del, SIGNAL(triggered(bool)), this, SLOT(del_camera(bool)));


            cmenu->exec(QCursor::pos());
        }
    }
#else



    QMenu *cmenu = new QMenu(f->treeWidget_devices);
    QAction *action_add = cmenu->addAction("add");

    connect(action_add, SIGNAL(triggered(bool)), this, SLOT(add_camera(bool)));

    cmenu->exec(QCursor::pos());
#endif

}

void MainWidget::on_treeWidget_devices_doubleClicked(const QModelIndex &index)
{
    //itm= f->treeWidget_devices->itemAt(pos);
    itm=  f->treeWidget_devices->currentItem();
    if(itm){
        if(!itm->parent()){//root


        }else{//child
            int now=itm->parent()->indexOfChild(itm);
            playing_index=now+1;
            if(now<p_cfg->data.camera_amount){
                QString url=itm->text(0);
                qDebug()<<"get "<<url;
//                f->openGLWidget->render_set_mat(mat);
//                f->openGLWidget->update();
                 f->openGLWidget->start(itm->text(0));

            }
        }
    }
}
