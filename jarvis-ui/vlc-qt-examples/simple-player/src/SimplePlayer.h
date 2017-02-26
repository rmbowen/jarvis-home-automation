/*
* VLC-Qt Simple Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/

#ifndef SIMPLEPLAYER_H_
#define SIMPLEPLAYER_H_

#include <QMainWindow>
#include <QList>
#include <QTcpServer>
#include <QNetworkSession>

namespace Ui {
    class SimplePlayer;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class EqualizerDialog;

class SimplePlayer : public QMainWindow
{
    Q_OBJECT
public:
    explicit SimplePlayer(QWidget *parent = 0);
    ~SimplePlayer();

private slots:
    void openLocal();
    void openUrl();

    void on_btnSwitch_clicked();

    void socketConnection(void);

    void socketData(void);
private:
    Ui::SimplePlayer *ui;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

    EqualizerDialog *_equalizerDialog;
    QList<QString> _streamNames;
    int _streamIndex;

    QTcpSocket* _client;

    QTcpServer* tcpServer;
    QNetworkSession* networkSession;

};

#endif // SIMPLEPLAYER_H_
