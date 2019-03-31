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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QtCore/QObject>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsTextItem>
#include <QtCore/QElapsedTimer>
#include <QtCore/QTimer>
#include <QMutex>
#include <QVector>
#include <atomic>

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(int bufferLength = 32, QObject *parent = 0);
    void pushData(QVector<QVector<QPointF>> _data);
    void startUpdates(const QList<QLineSeries *> &seriesList,
                      const QList<QScatterSeries *> &markerList,
                      const QList<QGraphicsSimpleTextItem *> &markerTextList,
                      QLabel *fpsLabel);
    bool setBufferLength(int len);
    void clear();

public slots:
    void setMarkerPos(int);
    void handleSceneChanged();
    void updateAllSeries();

private:
    std::atomic<int> m_markerPos;
    QMutex dataMutex;
    QVector<QVector<QVector<QPointF> > > m_data;
    int dataLengthMax;
    QList<QLineSeries *> m_seriesList;
    QList<QScatterSeries *> m_markerList;
    QList<QGraphicsSimpleTextItem *> m_markerTextList;
    QLabel *m_fpsLabel;
    QElapsedTimer m_fpsTimer;
    QTimer m_dataUpdater;
};

#endif // DATASOURCE_H
