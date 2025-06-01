#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMediaDevices>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui_(new Ui::MainWindow)
{
    m_ui_->setupUi(this);
}

void MainWindow::init()
{
    auto plot = m_ui_->plot;
    m_plotController_.setPlot(plot);
}

MainWindow::~MainWindow()
{
    delete m_ui_;
}
