#ifndef TESTSTARTUP_H
#define TESTSTARTUP_H

#include "testitem.h"
#include <QObject>
#include <tuple>
#include <QTime>
#include <QTimer>

class TestStartup : public TestItem
{
    Q_OBJECT

public:
    TestStartup(QString port, QObject *parent=nullptr);
    void readyGo(void) override;

private:
    static const std::tuple<QByteArray, int> cmdSeq[4];

    // TestItem interface
//signals:
//    void requestSend(const QByteArray &cmd);

public slots:
    void processData(QObject *sender, const QByteArray &data) override;
    void processError(QObject *sender, int err) override;
    void doNextStep(void) override;
};

#endif // TESTSTARTUP_H
