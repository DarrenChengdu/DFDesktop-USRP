#include "glchartview.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>
#include <QDebug>

#define TEXT_X_DIST_PIXEL 100
#define TEXT_Y_DIST_PIXEL 10

GLChartView::GLChartView(QString title, QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_chart(0), m_markerText(0)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContentsMargins(0,0,0,0);

    // chart
    m_chart = new QChart;
    m_chart->setMinimumSize(200, 150);
//    m_chart->setTitle(title);
    m_chart->legend()->hide();
    lineSeries = new QLineSeries;
    lineSeries->setPen(QPen(QBrush(QColor("blue")), 1));
    lineSeries->setUseOpenGL(true);
    m_chart->addSeries(lineSeries);

    markerSeries = new QScatterSeries;
    markerSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    markerSeries->setMarkerSize(7.0);
    markerSeries->setPen(QPen("orange"));
    markerSeries->setBrush(Qt::SolidPattern);
    markerSeries->setBorderColor(QColor("orange"));
    markerSeries->setColor(QColor("orange"));
    markerSeries->setUseOpenGL(false);
    m_chart->addSeries(markerSeries);

    m_chart->createDefaultAxes();
    m_chart->axisX()->setRange(90.0, 110.0);
    m_chart->axisY()->setRange(-120.0, 10.0);
    m_chart->axisY()->setTitleText(title);
//    m_chart->axisY()->setTitleFont(QFont("Times", 10, QFont::Bold));

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_markerText = new QGraphicsSimpleTextItem(m_chart);
    qreal textXPos = m_chart->size().width() - TEXT_X_DIST_PIXEL;
    qreal textYPos = TEXT_Y_DIST_PIXEL;
    m_markerText->setPos(textXPos, textYPos);
    m_markerText->setText("X:    \nY:    ");
}

void GLChartView::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        m_chart->resize(event->size());
        qreal textXPos = m_chart->size().width() - TEXT_X_DIST_PIXEL;
        qreal textYPos = TEXT_Y_DIST_PIXEL;
        m_markerText->setPos(textXPos, textYPos);
    }

    QGraphicsView::resizeEvent(event);
}

void GLChartView::setAxisXRange(double xmin, double xmax)
{
    if (xmax < xmin) {
        qDebug() << "GLChartView::setAxisXRange => xmax < xmin";
        return;
    }

    m_chart->axisX()->setRange(xmin, xmax);    
}

void GLChartView::setAxisYRange(double ymin, double ymax)
{
    if (ymax < ymin) {
        qDebug() << "GLChartView::setAxisYRange => ymax < ymin";
        return;
    }

    m_chart->axisY()->setRange(ymin, ymax);
}
