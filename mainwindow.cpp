#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAudioSource>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QIODevice>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <alsa/asoundlib.h>
#include <span>

using namespace std::chrono_literals;

constexpr static uint32_t AMPLITUDE_PLOT_UPDATE_MS = 1000 / 70;
constexpr static uint32_t FREQUENCY_PLOT_UPDATE_MS = 1000 / 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_amplitudeTimer(this)
    , m_frequencyTimer(this)
{
    ui->setupUi(this);

    m_frequencyPlot.initialize(ui->frequencyPlot);
    m_amplitudePlot.initialize(ui->amplitudePlot);

    connect(&m_amplitudeTimer, &QTimer::timeout, this, [this](){
        m_amplitudePlot.updatePlot();
    });

    connect(&m_frequencyTimer, &QTimer::timeout, this, [this](){
        m_frequencyPlot.updatePlot();
    });

    connect(&m_loader, &AudioLoader::audioLoaded, this,
            [this](){
        m_isLoaded = true;
    });
}

MainWindow::~MainWindow()
{
    stopRecording();
    delete ui;
}

void MainWindow::startRecording()
{
    bool fromFile = false;
    if (m_filePath != ""){
        m_loader.loadAudio(m_filePath);
        fromFile = true;
    }
    else{
        QMessageBox::warning(this, tr("File Not Found"),
                             tr("The selected file does not exist!"));
        return;
    }

    m_soundThread = std::thread([&, this](){
        m_player.startPlaying();
        //m_recorder.startRecording();
        m_isRecording = true;

        while (m_isRecording){
            //auto data = m_loader.getAudioSamples();
            std::vector<float> data{};
            if (m_isLoaded){
                data = m_loader.getSamples(256);
                if (data.size() == 0){
                    break;
                }
            }
            else{
                continue;
                data = m_recorder.getData(256);
            }
            m_volume.processAudio(data);

            m_frequencyPlot.addData(data);
            m_amplitudePlot.addData(data);
            m_player.playSound(data);
        }

        m_player.stopPlaying();
        //m_recorder.stopRecording();
    });

    m_amplitudeTimer.start(AMPLITUDE_PLOT_UPDATE_MS);
    m_frequencyTimer.start(FREQUENCY_PLOT_UPDATE_MS);
}

void MainWindow::stopRecording()
{
    m_amplitudeTimer.stop();
    m_frequencyTimer.stop();

    m_isRecording = false;
    if (m_soundThread.joinable()){
        m_soundThread.join();
    }
}

void MainWindow::on_buttonStart_clicked()
{
    stopRecording();
    startRecording();
}

void MainWindow::on_buttonStop_clicked()
{
    stopRecording();
}

void MainWindow::on_buttonOpenFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Audio File"),
                                                    "", tr("Audio Files (*.wav *.mp3);;All Files (*)"));
    QFile file(filePath);
    if (file.exists()) {
        m_filePath = filePath;
    } else {
        QMessageBox::warning(this, tr("File Not Found"),
                             tr("The selected file does not exist!"));
        m_filePath = "";
    }
}

void MainWindow::on_sliderVolume_valueChanged(int value)
{
    float sliderValue = ui->sliderVolume->value() / 100.f;
    m_volume.setVolume(sliderValue);
}

