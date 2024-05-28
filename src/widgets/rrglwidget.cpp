#include "rrglwidget.h"
//#include <GL/glu.h>
#include <QDebug>


RRGLWidget::RRGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
//    m_theme = FluThemeUtils::getUtils()->getTheme();
//    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, [=]() {
//        if (!isVisible()) {
//            return;
//        }
//        onThemeChanged();
//    });
}

RRGLWidget::~RRGLWidget() {

}

void RRGLWidget::transformPoint(GLdouble out[4], const GLdouble m[16], const GLdouble in[4]) {
#define M(row, col)  m[col*4+row]
    out[0] =
            M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
    out[1] =
            M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
    out[2] =
            M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
    out[3] =
            M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
#undef M
}

GLint RRGLWidget::project(GLdouble objx, GLdouble objy, GLdouble objz,
                          const GLdouble model[16], const GLdouble proj[16],
                          const GLint viewport[4],
                          GLdouble *winx, GLdouble *winy, GLdouble *winz) {
    GLdouble in[4], out[4];

    in[0] = objx;
    in[1] = objy;
    in[2] = objz;
    in[3] = 1.0;
    transformPoint(out, model, in);
    transformPoint(in, proj, out);

    if (in[3] == 0.0)
        return GL_FALSE;

    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];

    *winx = viewport[0] + (1 + in[0]) * viewport[2] / 2;
    *winy = viewport[1] + (1 + in[1]) * viewport[3] / 2;

    *winz = (1 + in[2]) / 2;
    return GL_TRUE;
}

void RRGLWidget::renderText(double x, double y, double z, const QString &text, const QFont &font, const QColor &color) {
    QtSaveGLState();
//    int width = this->width();
    int height = this->height();

    GLdouble model[4][4], proj[4][4];
    GLint view[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, &model[0][0]);
    glGetDoublev(GL_PROJECTION_MATRIX, &proj[0][0]);
    glGetIntegerv(GL_VIEWPORT, &view[0]);
    GLdouble textPosX = 0, textPosY = 0, textPosZ = 0;

    project(x, y, z,
            &model[0][0], &proj[0][0], &view[0],
            &textPosX, &textPosY, &textPosZ);

    textPosY = height - textPosY; // y is inverted

    QPainter painter(this);

    painter.setPen(color);
    painter.setFont(font);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawText(textPosX, textPosY, text); // z = pointT4.z + distOverOp / 4
    painter.end();
    QtRestoreGLState();
}

void RRGLWidget::QtSaveGLState() {
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void RRGLWidget::QtRestoreGLState() {
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
    glPopClientAttrib();
}

void RRGLWidget::drawGrid() {
    glPushMatrix();         // 存储当前坐标系位置
    GLfloat color[3] = {170.0 / 255, 170.0 / 255, 170.0 / 255};
//    if (m_theme == FluTheme::Dark) {
////         color = {125 / 255, 10/ 255, 185 / 255};
//        color[0] = 125.0 / 255;
//        color[1] = 10.0 / 255;
//        color[2] = 185.0 / 255;
//    } else {
////        color = {170.0 / 255, 170.0 / 255, 170.0 / 255};
//        color[0] = 170.0 / 255;
//        color[1] = 170.0 / 255;
//        color[2] = 170.0 / 255;
//    }
    ///
    /// \brief glMaterialfv  指定材质对漫射光的反射率
    /// @param face   决定该材质运用于图元的正面还是反面
    /// @param pname  表示对何种光进行设置(环境光和漫射光)
    /// @param params 四维数组，这个数组描述了反光率的RGBA值，每一项取值都为0-1之间
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    int step = 100;
    int num = 8;
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
//    qglColor(QColor::fromRgbF(1, 1, 1));
//    qt_save_gl_state();
    renderText(1050, 0, 0, "+X", QFont("helvetica", 12, QFont::Bold, true), QColor(255, 0, 0));
    renderText(0, 1050, 0, "+Y", QFont("helvetica", 12, QFont::Bold, true), QColor(0, 255, 0));
    renderText(0, 0, 1050, "+Z", QFont("helvetica", 12, QFont::Bold, true), QColor(0, 0, 255));
    renderText(500, 500, 0, "World", QFont("helvetica", 12, QFont::Bold, true), QColor(255, 255, 255));
//    qt_restore_gl_state();

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
//    qglColor(QColor::fromRgbF(r / 255, g / 255, b / 255));
    renderText(300, 0, 0, "+X", QFont("helvetica", 12, QFont::Bold, true), QColor(255, 255, 255));
    renderText(0, 300, 0, "+Y", QFont("helvetica", 12, QFont::Bold, true), QColor(255, 255, 255));
    renderText(0, 0, 300, "+Z", QFont("helvetica", 12, QFont::Bold, true), QColor(255, 255, 255));
    renderText(50, 50, 100, text.c_str(), QFont("helvetica", 12, QFont::Bold, true), QColor(255, 255, 255));
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
        update();
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
    update();
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
    update();
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
    QWidget::mousePressEvent(event);
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
    QWidget::mousePressEvent(event);
}

QVector<float> RRGLWidget::getJVars() {
    return mRobotConfig.JVars;
}

void RRGLWidget::setPoseText(QString x, QString y, QString z, QString rx, QString ry, QString rz) {
    mEulerConfig.x = x;
    mEulerConfig.y = y;
    mEulerConfig.z = z;
    mEulerConfig.rx = rx;
    mEulerConfig.ry = ry;
    mEulerConfig.rz = rz;
    if (mGlobalConfig.isDrawPoseText) {
        update();
    }
}

void RRGLWidget::drawPoseText() {
    QtSaveGLState();
    QPainter painter(this);
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("helvetica", 10));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawText(10, 30, "x:");
    painter.drawText(25, 30, mEulerConfig.x);

    painter.drawText(90, 30, "y:");
    painter.drawText(105, 30, mEulerConfig.y);

    painter.drawText(170, 30, "z:");
    painter.drawText(185, 30, mEulerConfig.z);


    painter.drawText(10, 50, "rx:");
    painter.drawText(25, 50, mEulerConfig.rx);

    painter.drawText(90, 50, "ry:");
    painter.drawText(105, 50, mEulerConfig.ry);

    painter.drawText(170, 50, "rz:");
    painter.drawText(185, 50, mEulerConfig.rz);

    painter.end();
    QtRestoreGLState();
}

void RRGLWidget::drawCone() {
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int i = 0;
    for (i = 0; i <= 360; i += 15) {
        float p = i * 3.14 / 180;
        glColor3f(sin(p), cos(p), 1.0f);
        glVertex3f(100, 100, 100.0f);
        glVertex3f(sin(p), cos(p), 0.0f);
    }
    glEnd();
    //bottom circle
    glColor3f(0, 1, 1);
    drawCircle();
}

void RRGLWidget::drawCircle() {
    glBegin(GL_TRIANGLE_FAN);           //扇形连续填充三角形串
    glVertex3f(0.0f, 0.0f, 0.0f);
    int i = 0;
    for (i = 0; i <= 360; i += 15) {
        float p = i * 3.14 / 180;
        glColor3f(sin(p), cos(p), tan(p));
        glVertex3f(sin(p), cos(p), 100.0f);
    }
    glEnd();
}

//void RRGLWidget::showEvent(QShowEvent *event) {
//    QWidget::showEvent(event);
//    if (m_theme != FluThemeUtils::getUtils()->getTheme()) {
//        onThemeChanged();
//        m_theme = FluThemeUtils::getUtils()->getTheme();
//    }
//
//}
//
//void RRGLWidget::onThemeChanged() {
//    m_theme = FluThemeUtils::getUtils()->getTheme();
//    update();
////    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
////    {
//////        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluCodeBox.qss", this);
////        qDebug()<<"Light Mode";
////        m_theme = FluThemeUtils::getUtils()->getTheme();
////    }
////    else
////    {
//////        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluCodeBox.qss", this);
////        qDebug()<<"Dark Mode";
////
////    }
//}
//
//void RRGLWidget::drawBackGround() {
//    if (m_theme == FluTheme::Dark) {
//        glClearColor(0.2, 0.4, 0.6, 0.8);
//    } else {
//        glClearColor(0.5, 0.6, 0.6, 0.8);
//    }
//}
