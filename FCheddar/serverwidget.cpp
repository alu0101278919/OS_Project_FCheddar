#include "serverwidget.h"
#include "ui_serverwidget.h"

#include <QMessageBox>
#include <QBuffer>
#include <iostream>
#include <unistd.h>


// Constructor
ServerWidget::ServerWidget(QWidget *parent, Database* database) :
    QDialog(parent),
    ui(new Ui::ServerWidget),
    mClientSocket(new QTcpSocket),
    mServerSocket(new QTcpServer),
    mDatabase(database),
    msgCounter(0) {

    ui->setupUi(this);
    ui->lineEdit->setText("127.0.0.1");
    connect(mServerSocket, SIGNAL(newConnection()), this, SLOT(processPendingRequest()));
}


// Destructor
ServerWidget::~ServerWidget() {
    delete ui;
    if (mClientSocket->isOpen()) mClientSocket->deleteLater();
    mServerSocket->deleteLater();
}


// Method that activates the listening of a port
void ServerWidget::on_start_clicked() {
    if (!mServerSocket->isListening()) {
        if (!mServerSocket->listen(QHostAddress::Any, ui->portBox->value())) {
            ui->listWidget->addItem(mServerSocket->errorString());
        } else {
            ui->listWidget->addItem("Server started.");
        }
    } else {
        ui->listWidget->addItem("Server is already listening.");
    }
}


// Method that disconnects
void ServerWidget::on_disconnect_clicked() {
    ui->listWidget->clear();
    disconnected();
}


// Method to send text
void ServerWidget::sendText(QString msg) {
    char id = 1;
    QTextStream T(mClientSocket);
    T << id;
    T << msg;
    mClientSocket->flush();
}


// Get image from pixmap to QByteArray
QByteArray ServerWidget::getImageData(const QPixmap &pixmap) {
    QByteArray imageData;
    QBuffer buffer(&imageData);
    // Está la puse yo porque en mi caso paso de un pixmap
    QImage im = pixmap.toImage();
    im.save(&buffer, "png");
    imageData = imageData.toBase64();
    return imageData;
}


// Method to send the image using the TCP protocol
void ServerWidget::sendImage(QPixmap pixmapVal) {
      QByteArray outImageBlock;  // Block to send
      QDataStream sendOut(&outImageBlock, QIODevice::WriteOnly);
      QString imageData = getImageData(pixmapVal);

      sendOut << qint64(0) << qint64(0) << imageData;  // Data header
      qint64 totalBytes = outImageBlock.size();

      ui->listWidget->addItem("Data header: " + QString::number(sizeof(qint64)*2));
      ui->listWidget->addItem("Image size: " + QString::number(outImageBlock.size() - sizeof(qint64)*2));
      ui->listWidget->addItem("Block size: " + QString::number(totalBytes));

      sendOut.device()->seek(0);
      sendOut << totalBytes << qint64((outImageBlock.size() - sizeof(qint64)*2));  // Header: totalBytes and imgSize

      mClientSocket->write(outImageBlock);
      mClientSocket->flush();

      outImageBlock.resize(0);
}


// Method that is called each time the readyRead signal of the mClientSocket is activated
void ServerWidget::readyRead() {
    ui->listWidget->addItem("Process request");
    if (mClientSocket->isReadable()) {
        QByteArray message = mClientSocket->readAll(); // Read message
        ui->listWidget->addItem("Message: " + QString(message));
        if(!msgCounter) {
            msgCounter++;
            return;
        } else {
            if(QString(message).contains("info")) { // filter to know if the client is asking for info or img
                QString msg = QString(message).remove(QString("info "));
                QSqlQuery query;
                query.exec(QString("SELECT projectName FROM FCheddar WHERE projectName='%1'").arg(QString(msg)));
                query.next();
                if (query.value(0).toString().isEmpty()) {
                    ui->listWidget->addItem(QString(msg) + " was not found.");
                    sendText(QString(msg) + " was not found.");
                } else {
                    // We send the information of the project found
                    sendText(QString(msg) + " was found");

                    query.exec(QString("SELECT * FROM FCheddar WHERE projectName='%1'").arg(QString(msg)));
                    query.next();
                    QString answer = "Data " + QString(msg) + "\nTask number: " + query.value(2).toString() +
                            "\nHyperperiod: " + query.value(3).toString() +
                            "\nScheduleable: " + query.value(4).toString() +
                            "\nDate: " + query.value(5).toString() + "\n";
                    sendText(answer);
                }
            } else if (QString(message).contains("img")) {
                QString msg = QString(message).remove(QString("img "));
                QSqlQuery query;
                query.exec(QString("SELECT projectName FROM FCheddar WHERE projectName='%1'").arg(QString(msg)));
                query.next();
                if (query.value(0).toString().isEmpty()) {
                    ui->listWidget->addItem(QString(msg) + " was not found.");
                    sendText(QString(msg) + " was not found.");
                } else {
                    // We send the information of the project found
                    query.exec(QString("SELECT * FROM FCheddar WHERE projectName='%1'").arg(QString(msg)));
                    query.next();

                    QPixmap pixmap;
                    pixmap.loadFromData(query.value(6).toByteArray());
                    sendImage(pixmap);
                }
            }
        }
        msgCounter++;
    } else {
        ui->listWidget->addItem("ERROR: could not receive message (" + QString(mClientSocket->errorString()) + ")");
    }
}


// Method called by the new connections signal and binds the corresponding readyRead
// signal to the readyRead method of the class
void ServerWidget::processPendingRequest() {
    while (mServerSocket->hasPendingConnections()) {
        mClientSocket = mServerSocket->nextPendingConnection();
        connect(mClientSocket, SIGNAL(readyRead()), SLOT(readyRead()));
    }
}


// Method to disconnect the current socket
void ServerWidget::disconnected_socket() {
    if(mClientSocket->isOpen()) {
        disconnect(mClientSocket, SIGNAL(readyRead()));
        ui->listWidget->addItem("Socket disconnected");
        mClientSocket->close();
    }
}


// Method to disconnect the server (stop listening to the current port)
void ServerWidget::disconnected() {
    if (mClientSocket->isOpen()) {
        disconnected_socket();
    }
    if(mServerSocket->isListening()) {
        mServerSocket->close();
        ui->listWidget->addItem("Server disconnected.");
    } else {
        ui->listWidget->addItem("Server is already disconnected.");
    }

}



