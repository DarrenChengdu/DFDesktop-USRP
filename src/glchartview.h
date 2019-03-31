#ifndef GLCHARTVIEW_H
#define GLCHARTVIEW_H

#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
class QLineSeries;
class QScatterSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class GLChartView : public QGraphicsView
{
    Q_OBJECT
public:
    GLChartView(QString title, QWidget *parent = 0);
    QLineSeries *getLineSeries() const {return lineSeries;}
    QScatterSeries *getMarkerSeries() const {return markerSeries;}
    QGraphicsSimpleTextItem *getMarkerTextItem() const {return m_markerText;}

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QGraphicsSimpleTextItem *m_markerText;
    QChart *m_chart;
    QLineSeries *lineSeries;
    QScatterSeries *markerSeries;

public slots:
    void setAxisXRange(double xmin, double xmax);
    void setAxisYRange(double ymin, double ymax);
};

#endif // GLCHARTVIEW_H
