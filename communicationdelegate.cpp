#include "communicationdelegate.h"

#include <QDebug>

#include "checksum.h"

#include <QtAlgorithms>

CommunicationDelegate::CommunicationDelegate(const QString &name, QObject *parent) : QThread(parent), portName(name)
{
}


void CommunicationDelegate::run()
{
    QSerialPort *port = new QSerialPort(portName);
    port->open(QIODevice::ReadWrite);
    while(1) {
        mutex.lock();
        if (cmdQueue.isEmpty())
            queueNotEmpty.wait(&mutex);
        snd = cmdQueue.dequeue();
        mutex.unlock();


        err = INIT;
        QByteArray respondData;
        QByteArray result;

        // 发送数据

        port->write(snd.first);

        while(1) {

            if(!port->waitForReadyRead())
                err = NO_REPLY;
            else
                respondData.append(port->readAll());
            if (respondData.size() < 2)
                continue;
            QByteArray temp;
            temp = respondData.left(2);
            uint frameLength = (uint)temp.at(1);

            if (frameLength <= respondData.length())
            {
                temp = respondData.right(2);
                uint8_t first = (uint8_t)temp.at(0);
                uint8_t second = (uint8_t)temp.at(1);
                if(((uint16_t)first * 256 + second) == CRC16::crc16_table_256((unsigned short)0xFFFF, (unsigned char *)respondData.data(), (unsigned int)frameLength-2)) {
                    err = NO_ERROR;
                    result = respondData.left(frameLength);
                    respondData.remove(0, frameLength);
                }
                else {
                    respondData.clear();
                }
                break;
            }
        }

        if(err == NO_ERROR) {
            qDebug() << "emit sendResponse" << result;
            emit sendResponse(snd.second, result);
        }
        else {
            qDebug() << "emit commFailed" << err;
            emit commFailed(snd.second, err);
        }

    }

}

void CommunicationDelegate::sendCmd(const QByteArray &cmd)
{
    mutex.lock();
    cmdQueue.enqueue(testObj_t(cmd, sender()));
    queueNotEmpty.wakeOne();
    mutex.unlock();
}
