#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::on_pushButton_search_device_clicked()
{


    client->search_device();
    ui->lineEdit_search->setText(client->wait_server_info_reply(2));
}

void MainWindow::on_pushButton_connect_server_clicked()
{
    client->server_ip=ui->lineEdit_search->text();
    ui->lineEdit_connect->setText( client->server_ip);
    client->connect_to_server();
}
#include "protocol.h"

void MainWindow::on_pushButton_get_config_clicked()
{
    int request_length=Protocol::encode_configuration_request(buf);//encoder buffer
    QByteArray rst=client->call_server(buf,request_length);//talk to server
    rst=rst.remove(0,Protocol::HEAD_LENGTH);//TODO:get the ret value

    cam_manager->save_config(rst);
   // cam_manager->reconfig_camera(ui->gridLayout_2);

    ui->lineEdit_get->setText( cam_manager->get_config());
}




void MainWindow::on_pushButton_add_clicked()
{
    QString ip=ui->lineEdit_add->text();
    cam_manager->add_camera(ip);//add camera on client
    QByteArray setting=cam_manager->get_config(1);//get new config from local database
    int len=Protocol::encode_addcam_request(buf,setting.length());//encode buffer
    memcpy(buf+Protocol::HEAD_LENGTH,setting.data(),setting.length());
    QByteArray rst=client->call_server(buf,len);//talk to server
  //  cam_manager->reconfig_camera(ui->gridLayout_2);
}



void MainWindow::on_pushButton_del_clicked()
{
    QString index=ui->lineEdit_del->text();
    if(index.toInt()<=cam_manager->get_size()&&index.toInt()>0)
    {
        cam_manager->del_camera(index.toInt());// delete camera local
        Protocol::encode_delcam_request(buf,index.toInt());//encode buffer
        client->call_server(buf,Protocol::HEAD_LENGTH);//talk to server
    }
  //   cam_manager->reconfig_camera(ui->gridLayout_2);
}
