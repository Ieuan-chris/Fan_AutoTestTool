#ifndef TESTITEM_H
#define TESTITEM_H


#include <QObject>


#include "communicationdelegate.h"

class TestItem : public QObject
{
    Q_OBJECT

public:
    explicit TestItem(QString &port, QObject *parent = nullptr);
    virtual void readyGo(void);

signals:
    void requestSend(const QByteArray &cmd);

public slots:
    virtual void processData(QObject *sender, const QByteArray & data);
    virtual void processError(QObject *sender,int err);
    virtual void doNextStep(void);

protected:
    CommunicationDelegate *comm;
    int next_step;
//    typedef  std::tuple<QByteArray, QTime> cmdSeq_t;
//    static const cmdSeq_t cmdSeq[4];
};


#endif // TESTITEM_H
