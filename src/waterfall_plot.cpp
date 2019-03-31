#include "waterfall_plot.h"
#define MAX_HORIZONTAL_LINES (512)

#include <time.h>

// 返回自系统开机以来的毫秒数（tick）
unsigned long GetTickCount()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

WaterfallCanvas::WaterfallCanvas(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_fps(0)
    , m_limitValue(-9999.0)
    , m_pixHeight(0)
{
    initColor();
    setFixedSize(800,400);
}

WaterfallCanvas::~WaterfallCanvas()
{
    makeCurrent();
    m_vbo.destroy();
    doneCurrent();
}

void WaterfallCanvas::initializeGL()
{
    initializeOpenGLFunctions();

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex);
    const char *vsrc =
        "#version 330 \n"
        "in layout(location=0) vec3 position; \n"
        "in layout(location=1) vec3 color; \n"
        "uniform vec3 changePos; \n"
        "out vec3 outColor; \n"
        "void main() \n"
        "{\n"
        "    gl_Position = vec4(position, 1.0) + vec4(changePos, 0.0); \n"
        "    outColor = color; \n"
        "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment);
    const char *fsrc =
        "#version 330 \n"
        "in vec3 outColor;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(outColor,1.0);\n"
        "}\n";
    fshader->compileSourceCode(fsrc);
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShader(vshader);
    m_program->addShader(fshader);
    delete vshader;
    vshader = nullptr;
    delete fshader;
    fshader = nullptr;
    m_program->link();
    m_vbo.create();
}

void WaterfallCanvas::paintGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    draw();

    getFPS();
    QPainter painter;
    painter.begin(this);
    painter.beginNativePainting();
    draw();
    painter.endNativePainting();
    QPen pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);
    QFont sansFont("Helvetica [Cronyx]", 14, QFont::Bold);
    painter.setFont(sansFont);
    painter.drawText(5, 25, QString("fps:%1").arg(m_fps));
    painter.end();
}

void WaterfallCanvas::initColor()
{
    int tempColor[] = { 0x000000ff, 0x0000ffff, 0x0000ff00, 0x00ffff00, 0x00ff0000 };
    //颜色值不是规律增减，分四段考虑。
    const int cases = 4;
    int eachSegment = COLORCOUNT / cases;
    //segment[4]：四种情况的平分值，index：colorNum种情况，indexTemp：控制每段赋值
    int segment[cases] = { 0 };	int index = 0;	int indexTemp = eachSegment;

    for (int i = 0; i < cases; i++)
    {
        segment[i] = (tempColor[i + 1] - tempColor[i]) / eachSegment;

        for (index; index < indexTemp; index++)
        {
            int temp = tempColor[i] + (index - eachSegment * i) * segment[i];

            m_rgb[index].r = static_cast<uchar>(temp >> 16) / 255.0f;
            m_rgb[index].g = static_cast<uchar>(temp >> 8) / 255.0f;
            m_rgb[index].b = static_cast<uchar>(temp & 0xff) / 255.0f;
        }

        indexTemp += eachSegment;
    }
}

void WaterfallCanvas::getFPS()
{
    static int g_fps = 0;
    quint64 startTime = GetTickCount();
    static quint64 g_lastTime = 0;
    if (startTime - g_lastTime > 1000){
        g_lastTime = startTime;
        m_fps = g_fps;
        g_fps = 0;
    }
    else{
        g_fps++;
    }
}

void WaterfallCanvas::draw()
{
    m_program->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_vbo.bind();
    glLineWidth(2.0);

    for (int i = 0; i < m_dataBuffer.size(); i++)
    {
        int allocateSize = m_dataBuffer[i].size();
        m_vbo.allocate(m_dataBuffer[i].constData(), allocateSize * sizeof(GLfloat));
        //排列顺序是位置1(x1,y1,z1)颜色1(r1,g1,b1)……
        m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
        m_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
        m_program->setUniformValue("changePos", 0.0, 2 * 2.0 / m_pixHeight * i, 0.0);
        glDrawArrays(GL_LINE_STRIP, 0, allocateSize / 6);
    }

    glLineWidth(1.0);
    m_vbo.release();
    m_program->disableAttributeArray(0);
    m_program->disableAttributeArray(1);
    m_program->release();
}

void WaterfallCanvas::SetData(const QVector<double> &data)
{
    int count = data.size();

	if (count == 0) {
		m_dataBuffer.clear();
		update();
		return;
	}

    QVector<float> normal;

    int wid = width();
	m_pixHeight = height();
    float currentStep = 0.0f;
    float step = (float)wid / count;
    float xScale = 2.0f / wid;
    float yScale = 1.0f / (max - min);

    if(m_dataBuffer.size() >= MAX_HORIZONTAL_LINES)
        m_dataBuffer.removeLast();

    if(count <= wid)
    {
        for(int i = 0; i < count; i++)
        {
            normal.push_back(xScale * currentStep - 1.0f); // axis x
            normal.push_back(-1.0f);  // axis y
            normal.push_back(0.0f);  // axis z

            int index = 0;

            if(data.at(i) > m_limitValue)
            {
                index = yScale * (data.at(i) - min) * COLORCOUNT;
                index = qBound(0, index, COLORCOUNT - 1);
            }

            normal.push_back(m_rgb[index].r);
            normal.push_back(m_rgb[index].g);
            normal.push_back(m_rgb[index].b);

            currentStep += step;
        }
    }
    else
    {
        int currentPix = 0;
        float temp = min;

        for(int i = 0; i < count; i++)
        {
            if(data.at(i) > temp)
                temp = data.at(i);

            currentStep += step;

            if(currentStep > currentPix)
            {
                normal.push_back(xScale * currentPix - 1.0f);
                normal.push_back(-1.0f);
                normal.push_back(0.0f);

                int index = 0;

                if(temp > m_limitValue)
                {
                    index = yScale * (temp - min) * COLORCOUNT;
                    index = qBound(0, index, COLORCOUNT - 1);
                }

                normal.push_back(m_rgb[index].r);
                normal.push_back(m_rgb[index].g);
                normal.push_back(m_rgb[index].b);

                temp = min;
                currentPix++;
            }
        }
    }

    m_dataBuffer.prepend(normal);
    update();
}

void WaterfallCanvas::Clear()
{
    m_dataBuffer.clear();
    update();
}

void WaterfallCanvas::SetLimitLine(double v)
{
    m_limitValue = v;
}

ColorBar::ColorBar(QWidget *parent) : QWidget(parent)
{
    m_linear.setSpread(QGradient::PadSpread);
    m_linear.setColorAt(0, QColor(0, 0, 255));
    m_linear.setColorAt(0.25, QColor(0, 255, 255));
    m_linear.setColorAt(0.5, QColor(0, 255, 0));
    m_linear.setColorAt(0.75, QColor(255, 255, 0));
    m_linear.setColorAt(1, QColor(255, 0, 0));

    setFixedWidth(25);
}

void ColorBar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
//    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p.setPen(Qt::NoPen);
    m_linear.setStart(x(), height());
    m_linear.setFinalStop(x(), 0);
    p.setBrush(QBrush(m_linear));
    p.drawRect(this->rect());

    QWidget::paintEvent(event);
}

Scales::Scales(QWidget *parent, int min, int max)
    : QWidget(parent)
    , m_min(min)
    , m_max(max)
    , m_div(8)
{
    setFixedWidth(25);
}

void Scales::SetMinMax(int _min, int _max)
{
    m_min = _min; m_max = _max;
    m_step = (m_max - m_min)*1.0/(m_div-1);
}

void Scales::resizeEvent(QResizeEvent *event)
{

    QWidget::resizeEvent(event);
}

void Scales::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::black);

    double scale_y = height()*1.0/(m_div-1);

    for(int i = 0; i < m_div; i++)
    {
        int pos_y = scale_y*i;

        if (i == 0)
            pos_y = pos_y + 10;

        p.drawText(x(), pos_y, QString::number(m_max-i*m_step, 'f', 0));
    }

    QWidget::paintEvent(event);
}

WaterfallPlot::WaterfallPlot(QWidget *parent) : QWidget(parent)
{
    m_scales = new Scales(this);
    m_colorBar = new ColorBar(this);
    m_plot = new WaterfallCanvas(this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_scales);
    layout->addWidget(m_colorBar);
    layout->addWidget(m_plot);
    layout->setStretch(0, 1);
    layout->setStretch(1, 2);
    layout->setStretch(2, 10);
}

void WaterfallPlot::SetMinMax(double _min, double _max)
{
    m_plot->SetMinMax(_min, _max);
    m_scales->SetMinMax(_min, _max);
}

void WaterfallPlot::SetData(const QVector<double> &y)
{
    m_plot->SetData(y);
}

void WaterfallPlot::Clear()
{
    m_plot->Clear();
}

void WaterfallPlot::SetLimitLine(double v)
{
    m_plot->SetLimitLine(v);
}
