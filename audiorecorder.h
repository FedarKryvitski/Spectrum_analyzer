#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <alsa/asoundlib.h>
#include <QAudioDevice>
#include <QByteArray>
#include <QObject>

class AudioRecorder {
public:
    AudioRecorder() noexcept = default;

    void setDevice(const std::string& device);
    void startRecording();
    void stopRecording();
    std::vector<float> getData(int samples);

private:
    constexpr static uint32_t CHANNELS = 2;
    constexpr static uint32_t RATE = 44100;
    constexpr static snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

    snd_pcm_t *m_captureHandle;
    std::string m_device{"default"};
};

#endif // AUDIORECORDER_H

