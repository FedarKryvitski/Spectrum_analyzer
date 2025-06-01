#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioSource>
#include <QAudioFormat>

#include "plotcontroller.h"
#include "inputcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

private:
    Ui::MainWindow *m_ui_;
    PlotController m_plotController_;
    InputController m_inputController_;
};
#endif // MAINWINDOW_H
