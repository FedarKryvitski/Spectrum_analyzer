#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include <QAudioSource>
#include <QAudioFormat>

class InputController
{
public:
    InputController();
    virtual ~InputController() = default;

    void initialize();

private:
    std::unique_ptr<QAudioSource> m_source_{nullptr};
    QAudioFormat m_format_;
};

#endif // INPUTCONTROLLER_H
