#include "client.h"
#include "ui_client.h"

#include <QMessageBox>
#include <iostream>


// Constructor
Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client),
    mClientSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    totalBytes = 0;
    bytesReceived = 0;
    imageSize = 0;

    ui->statusLabel->setStyleSheet("background-color:red;");
    ui->serverEdit->setText("127.0.0.1");
    connect(mClientSocket, SIGNAL(connected()), this, SLOT(connected()));
}


// Destructor
Client::~Client()
{
    delete ui;
}


// Method to connect the socket to the server by the port that is listening
void Client::on_connectButton_clicked()
{
    if(mClientSocket->waitForConnected())
    {
        ui->listWidget->addItem("Client is already connected.");
        return;
    }
    mClientSocket->connectToHost(ui->serverEdit->text(), ui->portSpinBox->value());
    if (mClientSocket->waitForConnected())
    {
        ui->listWidget->addItem("Connected to the server.");
        connect(mClientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }
    else
    {
        ui->listWidget->addItem("Client could not connected.");
    }
}


// Method to get the image from QString to QImage
QImage Client::getImage(const QString &data)
{
    QByteArray imageData = QByteArray::fromBase64(data.toLatin1());
    QImage image;
    image.loadFromData(imageData);
    return image;
}


// Method that is called each time the readyRead signal of the mClientSocket is activated
void Client::readyRead()
{
    if (!mClientSocket->isTransactionStarted())
    {
        mClientSocket->startTransaction();
    }
    char id;
    mClientSocket->getChar(&id);
    if (id == 1)
    {
        QByteArray message = mClientSocket->readAll(); // Read message
        ui->listWidget->addItem("Message: " + QString(message));
        mClientSocket->commitTransaction();
        return;
    }
    mClientSocket->rollbackTransaction();  // To restablished socket information

    // QDataStream now has all socket information
    QDataStream in(mClientSocket);
    ui->listWidget->addItem("Image info:");
    ui->listWidget->addItem("The size of the packet that is currently being read: " + QString::number(mClientSocket->bytesAvailable()));
    // If what is received is less than 16 bytes, it is because
    // it is the part of the header, where the size is specified.
    if (bytesReceived <= sizeof(qint64)*2)
    {
        if((mClientSocket->bytesAvailable() >= sizeof(qint64)*2) && (imageSize == 0))
        {
            in >> totalBytes  >> imageSize;
            bytesReceived += sizeof(qint64) * 2;
            if(imageSize == 0)
            {
                ui->socketImage->setText("<b>No image</b>");
                totalBytes = 0;
                bytesReceived = 0;
                imageSize = 0;
                return;
            }
        }
        // As long as the picture is not the same as the size, we do not enter
        // We wait for the image to be complete
        if((mClientSocket->bytesAvailable() >= imageSize) && (imageSize != 0))
        {
            in >> imageContent;
            QImage imageData = getImage(imageContent);
            QPixmap resImage = QPixmap::fromImage(imageData);
            ui->socketImage->setPixmap(resImage);
            bytesReceived += imageSize;
            if(bytesReceived == totalBytes)
            {
                 ui->listWidget->addItem("Sent image size: " + QString::number(imageSize));
                 totalBytes = 0;
                 bytesReceived = 0;
                 imageSize = 0;
            }
         }
    }
}


// Method to send a message to the server and connect the corresponding readyRead
void Client::connected()
{
    sendText("Socket is connected");
    ui->statusLabel->setStyleSheet("background-color:green;");
    connect(mClientSocket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}


// Method to close the connection with the server
// This method allows to be activated by a disconnect signal from the server
void Client::disconnected()
{
    if(mClientSocket->isOpen())
    {
        sendText("Socket is disconnected");
        ui->listWidget->addItem("Disconnected from server.");
        ui->statusLabel->setStyleSheet("background-color:red;");
        disconnect(mClientSocket, SIGNAL(readyRead()),this, SLOT(readyRead()));
        mClientSocket->close();
    }
}


// Method to send text to the server
void Client::sendText(QString msg)
{
    QTextStream T(mClientSocket);
    T << msg;
    mClientSocket->flush();
}


// Method to ask the server if it has the information of a project determined by the QLineEdit
void Client::on_requestButton_clicked()
{
    if (mClientSocket->isOpen())
    {
        if (ui->projectNameReqEdit->text().isEmpty())
        {
            ui->listWidget->addItem("You cannot ask for a project without a name.");
        }
        else
        {
            sendText("info " + ui->projectNameReqEdit->text());
        }
    }
    else
    {
        ui->listWidget->addItem("You are not connected to any server.");
    }
}


// Method to ask the server if it has the image of a project determined by the QLineEdit
void Client::on_imgReqButton_clicked()
{
    if (mClientSocket->isOpen())
    {
        if (ui->projectNameReqEdit->text().isEmpty())
        {
            ui->listWidget->addItem("You cannot ask for a project without a name.");
        }
        else
        {
            sendText("img " + ui->projectNameReqEdit->text());
        }
    }
    else
    {
        ui->listWidget->addItem("You are not connected to any server.");
    }
}


// Method to disconnect the client
void Client::on_disconnectButton_clicked()
{
    if(mClientSocket->isOpen()) {
        disconnected();
    }
    else
    {
        ui->listWidget->addItem("You are already disconnected");
    }
}


// Method to clear the listWidget and socketImage and the name of the project to request
void Client::on_clearButton_clicked()
{
    ui->listWidget->clear();
    ui->socketImage->clear();
    ui->projectNameReqEdit->clear();
}

