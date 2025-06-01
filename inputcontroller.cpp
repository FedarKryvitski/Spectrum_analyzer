#include "inputcontroller.h"

#include <QMediaDevices>
#include <QDebug>

InputController::InputController()
{
    m_format_.setChannelCount(2);
    m_format_.setSampleFormat(QAudioFormat::Int16);
    m_format_.setSampleRate(48000);

    auto defaultInput = QMediaDevices::defaultAudioInput();
    m_source_.reset(new QAudioSource(defaultInput, m_format_));
}
