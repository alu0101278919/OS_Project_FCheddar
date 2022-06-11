#include "client.h"
#include "ui_client.h"

#include <QMessageBox>

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client),
    client(new QTcpSocket(this))
{
    ui->setupUi(this);

    totalBytes = 0;
    bytesReceived = 0;
    imageSize = 0;

    ui->statusLabel->setStyleSheet("background-color:red;");
    ui->serverEdit->setText("127.0.0.1");
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
}

Client::~Client()
{
    delete ui;
}

void Client::on_connectButton_clicked()
{
    client->connectToHost(ui->serverEdit->text(), ui->portSpinBox->value());
    QMessageBox::information(this, "Client", "Connected.");
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}


QImage Client::getImage(const QString &data) {
    QByteArray imageData = QByteArray::fromBase64(data.toLatin1());
    QImage image;
    image.loadFromData(imageData);
    return image;
}


QListWidgetItem* Client::getText(const int index) const {
    return ui->listWidget->item(index);
}


void Client::on_quitButton_clicked()
{
    close();
}


void Client::readyRead() {
    ui->listWidget->addItem("Server message: ");
    if (client->isReadable())
    {
        QByteArray message = client->readAll(); // Read message
        bool ok;
        QString(message.front()).toInt(&ok);
        if (!ok && message.front() != '\0') {
            ui->listWidget->addItem("Message: " + QString(message));
            return;
        }
        // El Datastream lee lo que le eches en el socket
        QDataStream in(client);
        // O es texto o cabecera
        if (bytesReceived <= sizeof(qint64)*2) {
            if((client->bytesAvailable() >= sizeof(qint64)*2) && (imageSize == 0)) {
                in >> totalBytes >> imageSize;
                // Se añade a los bytes recibidos
                bytesReceived += sizeof(qint64) * 2;
                if(imageSize == 0){
                    ui->imagen_socket->setText("No image sent</b>");
                    return;
                }
            }
            // Mientras la imagen no sea igual que el tamaño, no entramos
            // Esperamos a que la imagen esté entera
            if((client->bytesAvailable() >= imageSize)
                    && (imageSize != 0)) {
                // Guardamos en image conte
                in >> imageContent;
                // Obtenemos la imagen del dataStream
                QImage imageData = getImage(imageContent);
                // Pasamos la imagen a un pixmap que incrustar
                QPixmap resImage = QPixmap::fromImage(imageData);
                QPixmap* imgPointer = &resImage;
               // Si queremos mantener la escala o ponerla a un tamaño
               // imgPointer->scaled(ui->imagen_socket->size(), Qt::IgnoreAspectRatio);//Resize the image to fit the window
               // imgPointer->scaled(ui->imagen_socket->size(), Qt::KeepAspectRatio);//Set the size of pixmap scaling
               // ui->label_imagen->setPixmap(pixmap.scaled(300,300,Qt: :KeepAspectRatio));
               //  ui->imagen_socket->setScaledContents(true);//Set the attributes of the label to be able to zoom the pixmap to fill the entire available space.
                // Imagen_socket es el lugar donde pongo la imagen
                ui->imagen_socket->setPixmap(*imgPointer);
                bytesReceived += imageSize;
                // Debug que pueden borrar, para que vean que se está enviando
                qDebug() << "Los bytes recibidos: " << bytesReceived;
                // Cuando hemos llegado al final reiniciamos las variables
                // Cuidado con esto porque igual si metemos más cosas habrá
                // que plantearse más cosas
                if(bytesReceived == totalBytes){
                     //
                     qDebug() <<"SE ACABA. Tamaño:" << imageSize;
                     totalBytes = 0;
                     bytesReceived = 0;
                     imageSize = 0;
                }
             }
        }


    }
    else
    {
        ui->listWidget->addItem("ERROR: could not receive message (" + QString(client->errorString()) + ")");
    }
}


void Client::connected() {
    sendText("Socket client 1 is connected");
    ui->statusLabel->setStyleSheet("background-color:green;");
    connect(client, SIGNAL(readyRead()),this, SLOT(readyRead()));
}


void Client::disconnected() {
    sendText("Socket client 1 is disconnected");
    ui->statusLabel->setStyleSheet("background-color:red;");
    disconnect(client, SIGNAL(readyRead()),this, SLOT(readyRead()));
}


void Client::sendText(QString msg) {
    QTextStream T(client);
    T << msg;
    client->flush();
}


void Client::on_requestButton_clicked()
{
    sendText(ui->projectNameReqEdit->text());
}

