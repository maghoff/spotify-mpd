#ifndef CINEMADCONTROL_HPP
#define CINEMADCONTROL_HPP

#include <QMetaType>
#include <QObject>
#include <QTimer>

class CinemadControl : public QObject {
    Q_OBJECT

    QTimer timer;
    bool desiredState;

public:
    explicit CinemadControl(QObject *parent = 0);

signals:
    void playbackReady(bool);

public slots:
    void setDesiredState(bool);

private slots:
    void updateInternalState();
};

Q_DECLARE_METATYPE(CinemadControl*)

#endif // CINEMADCONTROL_HPP
