#include "audioloader.h"
#include <QAudioBuffer>
#include <QAudioFormat>
#include <QUrl>
#include <QFile>
#include "audioconverter.h"

AudioLoader::AudioLoader(QObject *parent)
    : QObject(parent), m_decoder(this) {
    connect(&m_decoder, &QAudioDecoder::bufferReady, this, &AudioLoader::processBuffer);
    connect(&m_decoder, &QAudioDecoder::finished, this, [this]() {
        emit audioLoaded();
    });

    connect(&m_decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error),
            [this](QAudioDecoder::Error error){
                qCritical() << "[AudioLoader] Decoding error:" << m_decoder.errorString();
            });
}

bool AudioLoader::loadAudio(const QString &filePath) {
    if (!QFile::exists(filePath)) {
        qCritical() << "[AudioLoader] File doesn't exist" << filePath;
        return false;
    }

    m_audioData.clear();

    m_decoder.setSource(QUrl::fromLocalFile(filePath));
    m_decoder.start();
    return true;
}

std::vector<float> AudioLoader::getSamples(int count) {
    int size = std::min((int)m_audioData.size() - m_bufferPos, count);
    std::vector<float> result(m_audioData.begin() + m_bufferPos,
                              m_audioData.begin() + m_bufferPos + size);
    m_bufferPos += size;
    return result;
}

void AudioLoader::processBuffer() {
    QAudioBuffer buffer = m_decoder.read();
    if (!buffer.isValid()) {
        qWarning() << "[AudioLoader] Предупреждение: Получен недействительный аудиобуфер.";
        return;
    }

    const QAudioFormat format = buffer.format();

    if (format.sampleFormat() == QAudioFormat::SampleFormat::Float) {
        auto data = buffer.constData<float>();
        for (int i = 0; i < buffer.frameCount() * format.channelCount(); ++i) {
            m_audioData.push_back(data[i]);
        }
    } else if (format.sampleFormat() == QAudioFormat::SampleFormat::Int16) {
        auto data = buffer.constData<int16_t>();
        for (int i = 0; i < buffer.frameCount() * format.channelCount(); ++i) {
            m_audioData.push_back(data[i] / 32768.0f);
        }
    } else {
        qCritical() << "[AudioLoader] Ошибка: Неподдерживаемый формат аудио.";
        return;
    }
}
