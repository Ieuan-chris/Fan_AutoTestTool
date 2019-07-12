#ifndef COMMUNICATIONDELEGATE_H
#define COMMUNICATIONDELEGATE_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QQueue>
#include <QByteArray>
#include <QPair>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>


class CommunicationDelegate : public QThread
{
    Q_OBJECT
public:
    explicit CommunicationDelegate(const QString &name, QObject *parent = nullptr);

    enum ERROR {INIT, NO_ERROR, NO_REPLY, FRAME_ERROR};
    Q_ENUM(ERROR)

signals:
    void commFailed(QObject *sender, int err);
    void sendResponse(QObject *sender, const QByteArray &response);

public slots:
    void sendCmd(const QByteArray &cmd);

private:
    QString portName;
    typedef QPair<QByteArray, QObject*> testObj_t;
    QQueue<testObj_t> cmdQueue;
    QWaitCondition queueNotEmpty;
    QMutex mutex;
    testObj_t snd;
    ERROR err;
//    QAbstractTableModel *model; 这个应该放在mainwindow.cpp中

    // QThread interface
protected:
    void run() override;
};

#endif // COMMUNICATIONDELEGATE_H
