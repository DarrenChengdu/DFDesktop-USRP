#ifndef WATERFALL_H
#define WATERFALL_H

#include <QtOpenGL>
#include <QVector>
#define COLORCOUNT (1020)

class WaterfallCanvas : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit WaterfallCanvas(QWidget *parent = nullptr);
    ~WaterfallCanvas();
    void SetMinMax(double _min, double _max) {min = _min; max = _max;}
    void SetData(const QVector<double> &y);
    void Clear();
    void SetLimitLine(double v);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    void draw();

    typedef struct RGBCOLOR{
        float r;
        float g;
        float b;
    } RGBCOLOR;

    RGBCOLOR m_rgb[COLORCOUNT];
    void initColor();

    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer m_vbo;

    void getFPS();
    int m_fps;

    int m_pixHeight;

    double m_limitValue;
    double min;
    double max;

    QVector<QVector<float>> m_dataBuffer;    

signals:

public slots:

};

class ColorBar : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBar(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QLinearGradient m_linear;
};

class Scales : public QWidget
{
    Q_OBJECT
public:
    explicit Scales(QWidget *parent = nullptr, int min = -120, int max = 0);
    void SetMinMax(int _min, int _max);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    int m_min, m_max;
    double m_step;
    int m_div;
};

class WaterfallPlot : public QWidget
{
    Q_OBJECT
public:
    explicit WaterfallPlot(QWidget *parent = nullptr);
    void SetMinMax(double _min, double _max);
    void Clear();

private:
    Scales *m_scales;
    ColorBar *m_colorBar;
    WaterfallCanvas *m_plot;

public slots:
    void SetData(const QVector<double> &y);
    void SetLimitLine(double _threshold);
};

#endif // WATERFALL_H
