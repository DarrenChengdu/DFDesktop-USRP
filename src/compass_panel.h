#ifndef COMPASS_PANEL_H
#define COMPASS_PANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QBoxLayout>

#include "qwt/qwt_plot.h"
#include "qwt/qwt_plot_canvas.h"
#include "qwt/qwt_plot_marker.h"
#include "qwt/qwt_plot_curve.h"
#include "qwt/qwt_plot_zoomer.h"
#include "qwt/qwt_plot_shapeitem.h"
#include "qwt/qwt_scale_widget.h"
#include "qwt/qwt_plot_grid.h"
#include "qwt/qwt_picker_machine.h"
#include "qwt/qwt_plot_panner.h"
#include "qwt/qwt_symbol.h"
#include "qwt/qwt_plot_renderer.h"
#include "qwt/qwt_plot_layout.h"
#include "qwt/qwt_plot_picker.h"
#include "qwt/qwt_slider.h"
#include "qwt/qwt_compass.h"
#include "qwt/qwt_dial_needle.h"

class QwtCompass;

class CompassPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CompassPanel(QWidget *parent = 0);

private:
    QwtCompass *compass;
    QTableWidget *table;

signals:

public slots:
};

#endif // COMPASS_PANEL_H
