#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QMessageBox>
#include <QBuffer>

class QTcpServer;

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    QTcpSocket* get_socket();
    QString get_text(void) const;

    void sendImage(QPixmap pixmapVal);
    QByteArray getImageData(const QPixmap&);

private slots:
    void readyRead();

private:
    QTcpSocket *mSocket;
    QImage *imageObject;
    QPixmap image;
    qint64 totalBytes;
    QString text;

};

#endif // TCPSERVER_H
