#ifndef AUDIOLOADER_H
#define AUDIOLOADER_H

#include <QObject>
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QVector>
#include <QDebug>

class AudioLoader : public QObject {
    Q_OBJECT

public:
    explicit AudioLoader(QObject *parent = nullptr);

    bool loadAudio(const QString &filePath);
    std::vector<float> getSamples(int count);

private slots:
    void processBuffer();

signals:
    void audioLoaded();

private:
    QAudioDecoder m_decoder;
    std::vector<float> m_audioData;
    int m_bufferPos{};
};

#endif // AUDIOLOADER_H
