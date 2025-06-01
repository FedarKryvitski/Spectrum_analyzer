#ifndef PLOTCONTROLLER_H
#define PLOTCONTROLLER_H

#include "qcustomplot.h"

class PlotController
{
public:
    PlotController() noexcept = default;
    virtual ~PlotController() = default;

    void setPlot(QCustomPlot* plot);

private:
    QCustomPlot* m_plot_{nullptr};
};

#endif // PLOTCONTROLLER_H
