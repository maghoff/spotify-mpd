#include <QTcpSocket>
#include "cinemadcontrol.hpp"

CinemadControl::CinemadControl(QObject *parent) :
    QObject(parent)
{
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateInternalState()));
}

void CinemadControl::setDesiredState(bool state) {
    desiredState = state;

    QTcpSocket cinemad;
    cinemad.connectToHost("10.0.0.200", 1337);
    if (desiredState) {
        cinemad.write("r:denon_select(\"blackrider\")\n");
    } else {
        cinemad.write("r:denon_off()\n");
    }
    cinemad.waitForBytesWritten();
    cinemad.close();

    if (desiredState) {
        timer.start(8000);
    } else {
        timer.stop();
        playbackReady(false);
    }
}

void CinemadControl::updateInternalState() {
    if (desiredState) playbackReady(true);
}
