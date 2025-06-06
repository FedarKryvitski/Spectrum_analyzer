#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QByteArray>
#include <alsa/asoundlib.h>
#include <span>

class AudioPlayer {
public:
    AudioPlayer() noexcept = default;

    void setDevice(const std::string& device);
    void startPlaying();
    void stopPlaying();
    void playSound(const std::span<float>& data);

private:
    constexpr static uint32_t BUF_BYTES = 256 * sizeof(int16_t);
    constexpr static uint32_t CHANNELS = 2;
    constexpr static uint32_t BUF_FRAMES = BUF_BYTES / CHANNELS / sizeof(int16_t);
    constexpr static uint32_t RATE = 44100;
    constexpr static snd_pcm_format_t FORMAT = SND_PCM_FORMAT_S16_LE;

    snd_pcm_t *playback_handle;
    std::string m_device{"default"};
};

#endif // AUDIOPLAYER_H
