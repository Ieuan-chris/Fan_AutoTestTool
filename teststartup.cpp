#include "teststartup.h"

#include <QDebug>

TestStartup::TestStartup(QString port, QObject *parent) : TestItem(port, parent)
{
    connect(this, &TestStartup::requestSend, comm, &CommunicationDelegate::sendCmd);
    connect(comm, &CommunicationDelegate::sendResponse, this, &TestStartup::processData);
    connect(comm, &CommunicationDelegate::commFailed, this, &TestStartup::processError);
}

const std::tuple<QByteArray, int> TestStartup::cmdSeq[4] = {
    std::tuple<QByteArray, int>(QByteArray::fromHex("00050473ED"), 4000),
    std::tuple<QByteArray, int>(QByteArray::fromHex("002100010000010100000000FFFF0000FFFF0100FFFF1100FFFF110100000051B7"), 20000),
    std::tuple<QByteArray, int>(QByteArray::fromHex("000508B261"), 4000),
    std::tuple<QByteArray, int>(QByteArray::fromHex("002100010000000400000000FFFF0000FFFF0100FFFF1100FFFF1101000000FF0C"), 60000)
};


void TestStartup::processData(QObject *sender, const QByteArray &data)
{
    //
    if (sender != this)
        return;

    qDebug() << "Received: " << data;

    if(std::get<1>(cmdSeq[next_step])) {
        QTimer::singleShot(std::get<1>(cmdSeq[next_step]), this, &TestStartup::doNextStep);
        next_step = (++next_step) % 4;
    }

}

void TestStartup::processError(QObject *sender,int err)
{
    if (sender != this)
        return;
    qDebug() << "Error: " << err;

    if(next_step < 4 && std::get<1>(cmdSeq[next_step])) {
        QTimer::singleShot(std::get<1>(cmdSeq[next_step]), this, &TestStartup::doNextStep);
        next_step++;
    }

}

void TestStartup::doNextStep(void)
{
    emit requestSend(std::get<0>(cmdSeq[next_step]));
}

void TestStartup::readyGo(void)
{
    qDebug() << "emit requestSend";
    emit requestSend(std::get<0>(cmdSeq[next_step]));
}
