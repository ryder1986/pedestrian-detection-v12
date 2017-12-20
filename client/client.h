#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QObject>
#include <QtNetwork>
#include <QMessageBox>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>
#include <QThread>
#include "protocol.h"
#include "common.h"
class Client : public QObject
{
    Q_OBJECT
public:
    QString server_ip;
    Client(){
        tcp_socket=new QTcpSocket();

        //      connect(tcp_socket, &QIODevice::readyRead, this, &Client::read_msg);
        //  f->open(tcp_socket,QFile::ReadOnly);

        in.setDevice(tcp_socket);
        in.setVersion(QDataStream::Qt_1_0);
        udp_skt_find_server=new QUdpSocket(this);
        udp_skt_find_server->bind(Protocol::CLIENT_REPORTER_PORT,QUdpSocket::ShareAddress);
        connect(udp_skt_find_server,SIGNAL(readyRead()),this,SLOT(get_reply()));


        udp_skt_alg_output=new QUdpSocket(this);
        udp_skt_alg_output->bind(Protocol::SERVER_DATA_OUTPUT_PORT,QUdpSocket::ShareAddress);
        connect(udp_skt_alg_output,SIGNAL(readyRead()),this,SLOT(get_rst()));



        //  broadcast_to_client();
        //  search();
    }

    void broadcast_info()
    {
        QByteArray b;
        b.append("pedestrian");
        udp_skt_find_server->writeDatagram(b.data(), b.size(),
                               QHostAddress::Broadcast, Protocol::SERVER_REPORTER_PORT);
        prt(info,"finding server ...");
    }

    void pack_tcp_data(char *c,int length){

        quint16  *pos_version=( quint16  *)c+2;
        quint16 *pos_length=( quint16  *)c;
        *pos_version=1;
        *pos_length=length;
    }
    char buf[10000];
    void get_client_setting(){
        qDebug()<<"try to get server setting";
        char *data=buf;
        quint16 *pos_op=(quint16 *)(data+4);
        *pos_op=0;

        pack_tcp_data(buf,0);
        int ret=tcp_socket->write(buf,6);
        qDebug()<<"ret "<<ret;
    }

    void read_msg()
    {
        //        in.setDevice(tcp_socket);
        //        in.setVersion(QDataStream::Qt_4_0);
        //        in.startTransaction();

        static  char buf[100];

        //    int len=3;
        //  in.readBytes(buf,len);
        int ret=  in.readRawData((char *)buf,3);
        //   int ret=  in.readBytes((char &)buf,len);

        // QString nextFortune;
        //in >> nextFortune;
        qDebug()<<QString::fromRawData((QChar *)buf,3);
        //        if (!in.commitTransaction())
        //            return;

    }

    void search_device()
    {
        broadcast_info();
        //  search();
    }

    QByteArray  call_server(char *buf,int len)
    {
        QByteArray ret;
        // int read_bytes;
        int write_bytes;
        ret.resize(0);
        //   char  bf[1000];
        write_bytes=tcp_socket->write(buf,len);
        //   QThread::msleep(3000);
        if(tcp_socket->waitForReadyRead())
            //      tcp_socket->read(bf,300);
            ret=tcp_socket->readAll();
        //   read_bytes=tcp_socket->read(bf,300);
        return ret;
    }


signals:
    void send_camera_rst(int index, QByteArray ba);
public slots:
    void get_rst()
    {
        QByteArray datagram_rst;
         // char sss[500];
       //   memset(sss,0,500);
        //  while(udp_skt->hasPendingDatagrams())
        if(udp_skt_alg_output->hasPendingDatagrams())
        {
         //   int size=udp_skt_alg_output->pendingDatagramSize();
            datagram_rst.resize((udp_skt_alg_output->pendingDatagramSize()));
          udp_skt_alg_output->readDatagram(datagram_rst.data(),datagram_rst.size());
//        udp_skt_alg_output->readDatagram(sss,500);
//          datagram_rst= udp_skt_alg_output->readAll();
             QList <QByteArray > bl= datagram_rst.split(':');
            QByteArray b_index=bl[0];
            int index=*(b_index);

            prt(info,"get cam   %d rst",index);

            QByteArray b_loc=bl[1];
            emit send_camera_rst(index,b_loc);
        //    QList <QByteArray > xy= b_loc.split(',');
//            int x=xy[0].toInt();
//            int y=xy[1].toInt();
//           prt(info," %d : %d",x,y);

        }
    }
    void get_reply()
    {
        //  while(udp_skt->hasPendingDatagrams())
        if(udp_skt_find_server->hasPendingDatagrams())
        {
            datagram.resize((udp_skt_find_server->pendingDatagramSize()));
            udp_skt_find_server->readDatagram(datagram.data(),datagram.size());
            prt(info,"get server info : %s",datagram.data());
            server_ip.append(datagram.split(',')[0]);
        }
    }
    QString wait_server_info_reply(int timeout_seconds)
    {
        int tick=0;
        QString str;
        int sleep_time=10;
        while(!udp_skt_find_server->hasPendingDatagrams())
        {
             QThread::msleep(sleep_time);
            if(tick++>timeout_seconds*1000/sleep_time){
                    return str;
            }
        }
        //    if(udp_skt->hasPendingDatagrams())
        {
            datagram.resize((udp_skt_find_server->pendingDatagramSize()));
            udp_skt_find_server->readDatagram(datagram.data(),datagram.size());
            prt(info,"get server info : %s",datagram.data());
            server_ip.clear();
            server_ip.append(datagram.split(',')[0]);
        }
        return server_ip;
    }
    void connect_to_server()
    {
        prt(info,"trying to connect %s",server_ip.data());
        qDebug()<<server_ip;
        tcp_socket->connectToHost(server_ip,Protocol::SERVER_PORT);
    }


    void  displayError(QAbstractSocket::SocketError socketError)
    {
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<"1";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<"1";
            break;
        default:
            qDebug()<<"1";
        }

        //  getFortuneButton->setEnabled(true);
    }

private:
    QTcpSocket *tcp_socket;
    QDataStream in;
    QFile *f;
    QUdpSocket *udp_skt_find_server;
    QUdpSocket *udp_skt_alg_output;
    QByteArray datagram;
};

#endif // CLIENT_H
