#include "audiorecorder.h"
#include "audioconverter.h"
#include <QDebug>

void AudioRecorder::setDevice(const std::string& device)
{
    m_device = device;
}

void AudioRecorder::startRecording() {
    int err{};
    if ((err = snd_pcm_open (&m_captureHandle, m_device.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        qDebug() << "cannot open audio device " << m_device << snd_strerror(err);
        return;
    }

    if ((err = snd_pcm_set_params(m_captureHandle, format,  SND_PCM_ACCESS_RW_INTERLEAVED, CHANNELS, RATE, 1, 50000)) < 0) {   /* 50 ms */
        qDebug() << "capture open error: " << snd_strerror(err);
        return;
    }
}

std::vector<float> AudioRecorder::getData(int samples){
    const auto BUF_FRAMES = samples;
    std::vector<int8_t> buffer(BUF_FRAMES * sizeof(int16_t) * CHANNELS);
    int err = snd_pcm_readi(m_captureHandle, buffer.data(), BUF_FRAMES);
    if (err < 0) {
        qDebug() << "read from audio interface failed" << snd_strerror(err);
        return {};
    }
    return AudioConverter::fromByteArray(buffer);
}

void AudioRecorder::stopRecording() {
    snd_pcm_close(m_captureHandle);
}
