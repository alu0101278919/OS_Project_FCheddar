#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QBuffer>
#include <QListWidget>

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

    QImage getImage(const QString &data);
    QListWidgetItem* getText(const int index) const;


private slots:
    void on_connectButton_clicked();
    void on_quitButton_clicked();
    void on_requestButton_clicked();

    void readyRead();
    void connected();
    void disconnected();

    void on_pushButton_clicked();

signals:
    void dataSet();

private:
    void sendText(QString msg);


    Ui::Client *ui;
    QTcpSocket *client;
    qint64 bytesReceived;  // The size of the received data
    qint64 imageSize;      //Image size
    qint64 totalBytes;     // Store total size information
    QString imageContent;
    QBuffer *mBuffer;

};

#endif // CLIENT_H
