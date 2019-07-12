#include "testitem.h"

TestItem::TestItem(QString &port, QObject *parent) : QObject(parent), comm(new CommunicationDelegate(port, this)),
    next_step(0)
{
    comm->start();
}

void TestItem::processData(QObject *sender, const QByteArray & data)
{

}

void TestItem::processError(QObject *sender,int err)
{

}

void TestItem::doNextStep(void)
{

}

void TestItem::readyGo(void)
{

}
