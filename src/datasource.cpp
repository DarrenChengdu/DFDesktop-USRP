/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "datasource.h"
#include <QtCore/QtMath>
#include <QDebug>
#include <QThread>
#include "globals.h"

QT_CHARTS_USE_NAMESPACE

DataSource::DataSource(int bufferLength, QObject *parent) :
    QObject(parent),
    dataLengthMax(bufferLength),
    m_markerPos(0)
{
    m_data.reserve(dataLengthMax);
}

void DataSource::handleSceneChanged()
{
    m_dataUpdater.start();
}

void DataSource::updateAllSeries()
{
    static int frameCount = 0;
    static QString labelText = QStringLiteral("FPS: %1");

    dataMutex.lock();

    // m_dataUpdater是单次执行, 启动后超时则执行DataSource::updateAllSeries,
    // 如果m_data为空,下面代码将直接返回,未能给主窗口"更新计数槽函数"以场景改变信号,
    // 导致m_dataUpdater未能再次启动. 因此这里需要增加对定时器的重新启动.
    if (m_data.isEmpty()) {
        dataMutex.unlock();
        m_dataUpdater.start();
        return;
    }
    QVector<QVector<QPointF> > seriesData = m_data.takeFirst();
    dataMutex.unlock();

    for (int i = 0; i < m_seriesList.size(); i++)
    {
        const QVector<QPointF> &singleSeries = seriesData.at(i);
        QLineSeries *lineSeries = m_seriesList.at(i);
        lineSeries->replace(singleSeries);

        QScatterSeries *markerSeries = m_markerList.at(i);
        const QPointF &point = singleSeries.at(m_markerPos);
        QPointF point2(point.x(),point.y()+3);
        QVector<QPointF> marker;
        marker.append(point2);
        markerSeries->replace(marker);

        QGraphicsSimpleTextItem *markerText = m_markerTextList.at(i);
        QString tex;// = QString("X: %1 MHz\nY: %2 dBm").arg(point.x()).arg(point.y());
        tex.sprintf("X: %.3f MHz\nY: %.1f dBm", point.x(), point.y());
        markerText->setText(tex);
    }

    frameCount++;
    int elapsed = m_fpsTimer.elapsed();
    if (elapsed >= 1000) {
        elapsed = m_fpsTimer.restart();
        qreal fps = qreal(0.1 * int(10000.0 * (qreal(frameCount) / qreal(elapsed))));
        m_fpsLabel->setText(labelText.arg(QString::number(fps, 'f', 1)));
        m_fpsLabel->adjustSize();
        frameCount = 0;
    }
}

void DataSource::startUpdates(const QList<QLineSeries *> &seriesList,
                              const QList<QScatterSeries *> &markerList,
                              const QList<QGraphicsSimpleTextItem *> &markerTextList,
                              QLabel *fpsLabel)
{
    m_seriesList = seriesList;
    m_markerList = markerList;
    m_markerTextList = markerTextList;
    m_fpsLabel = fpsLabel;

    m_dataUpdater.setInterval(0);
    m_dataUpdater.setSingleShot(true);
    QObject::connect(&m_dataUpdater, &QTimer::timeout,
                     this, &DataSource::updateAllSeries);

    m_fpsTimer.start();
    updateAllSeries();
}

void DataSource::clear()
{
    QVector<QVector<QVector<QPointF> > > nullData;
    dataMutex.lock();
    m_data.swap(nullData);
    dataMutex.unlock();
}

bool DataSource::setBufferLength(int len)
{
    clear();

    dataLengthMax = len;
    m_data.resize(dataLengthMax);

    return true;
}

void DataSource::pushData(QVector<QVector<QPointF>> _data)
{
    dataMutex.lock();
    if (m_data.size() > dataLengthMax) {
        dataMutex.unlock();
        return;
    }

    m_data.append(_data);
    dataMutex.unlock();
}

void DataSource::setMarkerPos(int pos)
{
    m_markerPos = pos;
}
