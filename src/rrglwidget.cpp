#include "rrglwidget.h"
//#include <GL/glu.h>

#include <QDebug>
#include <QMouseEvent>
#include <QtOpenGL>

RRGLWidget::RRGLWidget(QWidget *parent) : QGLWidget(parent) {

}

RRGLWidget::~RRGLWidget() {

}

void RRGLWidget::drawGrid() {
    glPushMatrix();         // 存储当前坐标系位置
    GLfloat color[] = {170.0 / 255, 170.0 / 255, 170.0 / 255};
    ///
    /// \brief glMaterialfv  指定材质对漫射光的反射率
    /// @param face   决定该材质运用于图元的正面还是反面
    /// @param pname  表示对何种光进行设置(环境光和漫射光)
    /// @param params 四维数组，这个数组描述了反光率的RGBA值，每一项取值都为0-1之间
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    int step = 50;
    int num = 15;
    for (int i = -num; i < num + 1; i++) {
        glBegin(GL_LINES);
        glVertex3f(i * step, -num * step, 0);
        glVertex3f(i * step, num * step, 0);
        glVertex3f(-num * step, i * step, 0);
        glVertex3f(num * step, i * step, 0);
        glEnd();
    }
    glPopMatrix();          // 恢复存储的坐标系位置
}

void RRGLWidget::drawCoordinates() {
    glPushMatrix();
    glLineWidth(3.0f);
    // x coord
    glBegin(GL_LINES);
    setupColor(255, 0, 0);
    glVertex3f(-1000, 0, 0);
    glVertex3f(1000, 0, 0);

    glVertex3f(950, 0, 20);
    glVertex3f(1000, 0, 0);
    glVertex3f(950, 0, -20);
    glVertex3f(1000, 0, 0);

    // y coord
    setupColor(0, 255, 0);
    glVertex3f(0, -1000, 0);
    glVertex3f(0, 1000, 0);

    glVertex3f(0, 950, 20);
    glVertex3f(0, 1000, 0);
    glVertex3f(0, 950, -20);
    glVertex3f(0, 1000, 0);

    // z coord
    setupColor(0, 0, 255);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1000);

    glVertex3f(20, 0, 950);
    glVertex3f(0, 0, 1000);
    glVertex3f(-20, 0, 950);
    glVertex3f(0, 0, 1000);

    glEnd();

    // 标签
    qglColor(QColor::fromRgbF(1, 1, 1));
    renderText(1050, 0, 0, "+X", QFont("helvetica", 12, QFont::Bold, true));
    renderText(0, 1050, 0, "+Y", QFont("helvetica", 12, QFont::Bold, true));
    renderText(0, 0, 1050, "+Z", QFont("helvetica", 12, QFont::Bold, true));
    renderText(500, 500, 0, "World", QFont("helvetica", 12, QFont::Bold, true));
    glLineWidth(1.0f);
    glPopMatrix();
}

void RRGLWidget::drawSTLCoordinates(int r, int g, int b, std::string text) {
    glPushMatrix();
    glLineWidth(1.5f);
    setupColor(r, g, b);
    glBegin(GL_LINES);

    glVertex3f(-300, 0, 0);
    glVertex3f(300, 0, 0);
    glVertex3f(0, -300, 0);
    glVertex3f(0, 300, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 300);
    glEnd();

    // 标签
    qglColor(QColor::fromRgbF(r / 255, g / 255, b / 255));
    renderText(300, 0, 0, "+X", QFont("helvetica", 12, QFont::Bold, true));
    renderText(0, 300, 0, "+Y", QFont("helvetica", 12, QFont::Bold, true));
    renderText(0, 0, 300, "+Z", QFont("helvetica", 12, QFont::Bold, true));
    renderText(50, 50, 100, text.c_str(), QFont("helvetica", 12, QFont::Bold, true));
    glLineWidth(1.0f);
    glPopMatrix();
}

void RRGLWidget::drawGL() {

}

void RRGLWidget::setupColor(int r, int g, int b) {
    GLfloat color[] = {static_cast<GLfloat>(r / 255.0), static_cast<GLfloat>(g / 255.0),
                       static_cast<GLfloat>(b / 255.0)};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

void RRGLWidget::setXRotation(int angle) {
    int tangle = angle;         // normalizeAngle(angle);
    if (tangle != xRot) {
        xRot = tangle;
        emit xRotationChanged(angle);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        updateGL();
    }
}

void RRGLWidget::setYRotation(int angle) {
    int tangle = angle;         // normalizeAngle(angle);
    if (tangle != yRot) {
        yRot = tangle;
        emit yRotationChanged(angle);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void RRGLWidget::setXYTranslate(int dx, int dy) {
    xTran += 3.0 * dx;
    yTran -= 3.0 * dy;
    updateGL();
}

// 注意角度： 360*16
int RRGLWidget::normalizeAngle(int angle) {
    int tangle = angle;
    while (tangle < 0) {
        tangle += 360 * 16;
    }
    while (tangle > 360 * 16) {
        tangle -= 360 * 16;
    }
    return tangle;
}

void RRGLWidget::setZoom(int zoom) {
    z_zoom = zoom;
    updateGL();
}

void RRGLWidget::initializeGL() {
    //用来初始化这个OpenGL窗口部件的，可以在里面设定一些有关选项
    GLfloat ambientLight[] = {0.7f, 0.7f, 0.7f, 1.0f};      //光源环境光强度数组
    GLfloat diffuseLight[] = {0.7f, 0.8f, 0.8f, 1.0f};      //光源散射光强度数组
    GLfloat specularLight[] = {0.4f, 0.4f, 0.4f, 1.0f};     //光源镜面反射光强度数组
    GLfloat positionLight[] = {20.0f, 20.0f, 20.0f, 0.0f};  //光源位置数组

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);     //设置0号光源的环境光属性
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);     //设置0号光源的散射光属性
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);   //设置0号光源的镜面反射光属性
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);        //设置照明模型参数
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight);   //设置0号光源的位置属性

    glEnable(GL_LIGHTING);   //启用光照
    glEnable(GL_LIGHT0);     //打开光源
    //glEnable(GL_DEPTH_TEST); //隐藏表面消除，打开深度缓冲区，绘制3D图像时候使用

    glClearDepth(1.0);       // 设置深度缓存
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL);  // 设置深度测试的类型
    glEnable(GL_NORMALIZE);

    glClearColor(0.5, 0.6, 0.6, 0.8);
}

//用来绘制OpenGL的窗口了，只要有更新发生，这个函数就会被调用
void RRGLWidget::paintGL() {

}

//用来处理窗口大小变化这一事件的，width和height就是新的大小状态下的宽和高，另外resizeGL()在处理完后会自动刷新屏幕
void RRGLWidget::resizeGL(int w, int h) {
    if (w < 0 || h < 0) {
        return;
    }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat zNear = 1.0;
    GLfloat zFar = 20000.0;
    GLfloat aspect = (GLfloat) w / (GLfloat) h;
    GLfloat fH = tan(GLfloat(70.0 / 360.0 * 3.14159)) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);       //将当前矩阵与一个透视矩阵相乘，把当前矩阵转变成透视矩阵，

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}

void RRGLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
}

void RRGLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    // 这里必须使用buttons()
    if (event->buttons() & Qt::LeftButton) {  //进行的按位与
        setXRotation(xRot + 4 * dy);
        setYRotation(yRot - 4 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setZoom(z_zoom + 5.0 * dy);
    } else if (event->buttons() & Qt::MiddleButton) {
        setXYTranslate(dx, dy);
    }
    lastPos = event->pos();
}
