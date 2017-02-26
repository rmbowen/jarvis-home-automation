/*
* VLC-Qt Simple Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/

#include <QFileDialog>
#include <QInputDialog>

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <QMessageBox>
#include <QTcpSocket>
#include <QByteArray>

#include "EqualizerDialog.h"

#include "SimplePlayer.h"
#include "ui_SimplePlayer.h"

SimplePlayer::SimplePlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SimplePlayer),
      _media(0),
      _equalizerDialog(new EqualizerDialog(this))
{
    ui->setupUi(this);

    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);
    _equalizerDialog->setMediaPlayer(_player);

    ui->video->setMediaPlayer(_player);
    ui->volume->setMediaPlayer(_player);
    ui->volume->setVolume(50);
    ui->seek->setMediaPlayer(_player);

    connect(ui->actionOpenLocal, &QAction::triggered, this, &SimplePlayer::openLocal);
    connect(ui->actionOpenUrl, &QAction::triggered, this, &SimplePlayer::openUrl);
    connect(ui->actionPause, &QAction::toggled, _player, &VlcMediaPlayer::togglePause);
    connect(ui->actionStop, &QAction::triggered, _player, &VlcMediaPlayer::stop);

    connect(ui->pause, &QPushButton::toggled, ui->actionPause, &QAction::toggle);
    connect(ui->stop, &QPushButton::clicked, _player, &VlcMediaPlayer::stop);

    _streamNames.append("rtsp://10.42.42.103:5554/Stream1");
    _streamNames.append("rtsp://10.42.42.103:5554/Stream2");

    _streamIndex = 0;



     _media = new VlcMedia(_streamNames.at(_streamIndex), _instance);
     _player->open(_media);

     // TCP SERVER INIT
     tcpServer = new QTcpServer(this);


        QHostAddress ipAddress;

        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
                ipAddress = ipAddressesList.at(i);
                break;
            }
        }
        // if we did not find one, use IPv4 localhost
        if (ipAddress.isNull())
            ipAddress = QHostAddress(QHostAddress::LocalHost);

        if (!tcpServer->listen(ipAddress,3181)) {
            QMessageBox::critical(this, tr("Jarvis Command Server"),
                                  tr("Unable to start the server: %1.")
                                  .arg(tcpServer->errorString()));
            close();
            return;
        }

        /*statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                                "Run the Fortune Client example now.")
                             .arg(ipAddress).arg(tcpServer->serverPort()));*/

         QMessageBox::critical(this, tr("Status"),tr("The server is running on\n\nIP: %1\nport: %2\n\n").arg(ipAddress.toString()).arg(tcpServer->serverPort()));

         connect(tcpServer,&QTcpServer::newConnection, this, &SimplePlayer::socketConnection);
}

SimplePlayer::~SimplePlayer()
{
    delete _player;
    delete _media;
    delete _instance;
    delete ui;
}

void SimplePlayer::socketConnection()
{
    qDebug() << "got a new incoming connection";

    //connect(tcpServer,&QTcpServer::newConnection, this, &SimplePlayer::socketConnection);
    _client = tcpServer->nextPendingConnection();

    qDebug() << "here";

    connect(_client,SIGNAL(readyRead()),this,SLOT(socketData()));
}

void SimplePlayer::socketData(void)
{
    qDebug() << "New Data";

    QByteArray data = _client->readAll();
    QString cmd(data);

    if(cmd.compare("bob's burgers")==0)
    {
        _media = new VlcMedia(_streamNames.at(0), _instance);
        _player->open(_media);
    }

    if(cmd.compare("Futurama")==0)
    {
        _media = new VlcMedia(_streamNames.at(1), _instance);
        _player->open(_media);
    }

    qDebug() << data;
}

void SimplePlayer::openLocal()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;

    _media = new VlcMedia(file, true, _instance);

    _player->open(_media);
}

void SimplePlayer::openUrl()
{
    QString url =
            QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));

    if (url.isEmpty())
        return;

    _media = new VlcMedia(url, _instance);

    _player->open(_media);
}

void SimplePlayer::on_btnSwitch_clicked()
{
    if(_streamIndex == (_streamNames.length()-1) )
    {
        _streamIndex = 0;
    }
    else
    {
        _streamIndex++;
    }
    _media = new VlcMedia(_streamNames.at(_streamIndex), _instance);
    _player->open(_media);

}
