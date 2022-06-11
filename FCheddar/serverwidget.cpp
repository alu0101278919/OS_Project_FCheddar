#include "serverwidget.h"
#include "ui_serverwidget.h"

#include <QMessageBox>
#include <QBuffer>



ServerWidget::ServerWidget(QWidget *parent, Database* database) :
    QDialog(parent),
    ui(new Ui::ServerWidget),
    mClientSocket(new QTcpSocket),
    mServerSocket(new QTcpServer),
    mDatabase(database),
    msgCounter(0)
{
    ui->setupUi(this);
    connect(mServerSocket, SIGNAL(newConnection()), this, SLOT(processPendingRequest()));
}


ServerWidget::~ServerWidget()
{
    delete ui;
}


void ServerWidget::on_start_clicked()
{
    if (!mServerSocket->isListening()) {
        if (!mServerSocket->listen(QHostAddress::Any, ui->portBox->value())) {
            ui->listWidget->addItem(mServerSocket->errorString());
        } else {
            ui->listWidget->addItem("Server started.");
        }
    } else {
        QMessageBox::critical(this, "Error", "Server is listening already.");
    }
}


void ServerWidget::on_quit_clicked()
{
    QDialog::close();
}


void ServerWidget::sendText(QString msg) {
    QTextStream T(mClientSocket);
    T << msg;
    mClientSocket->flush();
}


QByteArray ServerWidget::getImageData(const QPixmap &pixmap) {
    QByteArray imageData;
    QBuffer buffer(&imageData);
    // Está la puse yo porque en mi caso paso de un pixmap
    QImage im = pixmap.toImage();
    im.save(&buffer, "png");
    imageData = imageData.toBase64();
    return imageData;
}


void ServerWidget::sendImage(QByteArray img) {
    // El buffer de los datos, que almacena el conjunto de bytes de la Imagen que se va a enviar por el DataStream
    QByteArray outImageBlock;
    // El objeto DataStream. Se "come" lo que le eches, no solo objetos QBytesArray
    QDataStream sendOut(&outImageBlock, QIODevice::WriteOnly);
    // Se reserva el tamañano donde se guarda la información del tamaño de la imagen y se mete ya el objeto en sí, en este
    // caso la imagen
    sendOut << qint64(0) << qint64(0) << img;
    // El tamaño total es la suma de los tamaños de los bloques, en este caso un bloque con el tamaño de la imagen
    // y la propia información
    totalBytes += outImageBlock.size();
    // Ahora sí metemos en el DataStream los tamaños sustituyendo al espacio asignado
    // Para ello tenemos que volver al inicio
    sendOut.device()->seek(0);
    sendOut << totalBytes << qint64((outImageBlock.size() - sizeof(qint64)*2));
    // Send the readyRead() signal
    // Desde que se envía el readyRead del socket lo tiene en cuenta
    mClientSocket->write(outImageBlock);
    mClientSocket->flush();
    // Esto sólo para que vean el debug, lo pueden BORRAR
    ui->listWidget->addItem("Tamaño de la imagen: " + QString::number(outImageBlock.size() - sizeof(qint64)*2));
    ui->listWidget->addItem("Tamaño entero del paquete que se envía: " + QString::number(totalBytes));
    // Borramos lo que tenemos
    outImageBlock.resize(0);
    totalBytes = 0;
}


void ServerWidget::readyRead() {
    ui->listWidget->addItem("Process request");
    if (mClientSocket->isReadable())
    {
        QByteArray message = mClientSocket->readAll(); // Read message
        ui->listWidget->addItem("Message: " + QString(message));
        if(!msgCounter) {
            msgCounter++;
            return;
        } else {
            QSqlQuery query;
            query.exec(QString("SELECT projectName FROM FCheddar WHERE projectName='%1'").arg(QString(message)));
            query.next();
            if (query.value(0).toString().isEmpty()) {
                ui->listWidget->addItem(QString(message) + " was not found.");
                sendText(QString(message) + " was not found.");
            } else {
                // Enviamos la información del proyecto encontrado
                sendText(QString(message) + " was found");

                query.exec(QString("SELECT * FROM FCheddar WHERE projectName='%1'").arg(QString(message)));
                query.next();
                QString answer = "Data " + QString(message) + "\nTask number: " + query.value(2).toString() +
                        "\nHyperperiod: " + query.value(3).toString() +
                        "\nScheduleable: " + query.value(4).toString() +
                        "\nDate: " + query.value(5).toString() + "\n";
                sendText(answer);


                sendImage(query.value(6).toByteArray());

                mClientSocket->waitForBytesWritten(3000);

                ui->listWidget->addItem(QString(message) + " was sent");

            }
        }
        msgCounter++;
    }
    else
    {
        ui->listWidget->addItem("ERROR: could not receive message (" + QString(mClientSocket->errorString()) + ")");
    }
}


void ServerWidget::processPendingRequest() {
    while (mServerSocket->hasPendingConnections())
    {
        mClientSocket = mServerSocket->nextPendingConnection();
        connect(mClientSocket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(mClientSocket, SIGNAL(disconnected()), SLOT(disconnected()));
    }
}

void ServerWidget::disconnected()
{
    disconnect(mClientSocket, SIGNAL(readyRead()));
    disconnect(mClientSocket, SIGNAL(disconnected()));
    mClientSocket->deleteLater();
}



