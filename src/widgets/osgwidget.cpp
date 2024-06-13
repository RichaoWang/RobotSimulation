//
// Created by JerryWang on 24-6-2.
//

#include "osgwidget.h"
#include <QDebug>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osg/PolygonMode>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>

void OsgWidget::resizeGL(int width, int height) {
    this->getEventQueue()->windowResize(this->x() * m_scale, this->y() * m_scale, width * m_scale,
                                        height * m_scale);
    _mGraphicsWindow->resized(this->x() * m_scale, this->y() * m_scale, width * m_scale, height * m_scale);
    osg::Camera *camera = _mViewer->getCamera();
    camera->setViewport(0, 0, this->width() * m_scale, this->height() * m_scale);
}

void OsgWidget::initializeGL() {
    osg::DisplaySettings::instance()->setNumMultiSamples(4);
    osg::StateSet *stateSet = _mViewer->getCamera()->getOrCreateStateSet();
    osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
    polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
    stateSet->setAttributeAndModes(polyMode, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateSet->setMode(GL_MULTISAMPLE_ARB, osg::StateAttribute::ON);

    stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
}

void OsgWidget::mouseMoveEvent(QMouseEvent *event) {
    this->getEventQueue()->mouseMotion(event->x() * m_scale, event->y() * m_scale);
}

void OsgWidget::mousePressEvent(QMouseEvent *event) {
    unsigned int button = 0;
    switch (event->button()) {
        case Qt::LeftButton:
            button = 1;
            break;
        case Qt::MiddleButton:
            button = 2;
            break;
        case Qt::RightButton:
            button = 3;
            break;
        default:
            break;
    }
    this->getEventQueue()->mouseButtonPress(event->x() * m_scale, event->y() * m_scale, button);
}

void OsgWidget::mouseReleaseEvent(QMouseEvent *event) {
    unsigned int button = 0;
    switch (event->button()) {
        case Qt::LeftButton:
            button = 1;
            break;
        case Qt::MiddleButton:
            button = 2;
            break;
        case Qt::RightButton:
            button = 3;
            break;
        default:
            break;
    }
    this->getEventQueue()->mouseButtonRelease(event->x() * m_scale, event->y() * m_scale, button);
}

void OsgWidget::wheelEvent(QWheelEvent *event) {
    int delta = event->delta();
    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
                                                     osgGA::GUIEventAdapter::SCROLL_UP
                                                               : osgGA::GUIEventAdapter::SCROLL_DOWN;
    this->getEventQueue()->mouseScroll(motion);
}

bool OsgWidget::event(QEvent *event) {
    bool handled = QOpenGLWidget::event(event);
    this->update();
    return handled;
}

void OsgWidget::paintGL() {
    _mViewer->frame();
}

OsgWidget::OsgWidget(QWidget *parent)
        : QOpenGLWidget(parent), _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded(this->x(), this->y(),
                                                                                        this->width(),
                                                                                        this->height())),
          _mViewer(new osgViewer::Viewer)
        // take care of HDPI screen, e.g. Retina display on Mac
        , m_scale(QApplication::desktop()->devicePixelRatio()) {

    /// 文字渲染
    _mRenderWidget = createTextRenderWidget();

    // create camera
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setViewport(0, 0, this->width(), this->height());
    /// 背景颜色
    camera->setClearColor(osg::Vec4(0.9, 0.9, 0.9, 1.0));

    float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height());
    camera->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
    camera->setGraphicsContext(_mGraphicsWindow);
    _mViewer->setCamera(camera);

    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    manipulator->setAllowThrow(false);
    manipulator->setHomePosition({2000, -2700, 1800}, {0, 0, 800}, {0, 0, 1.57});

    this->setMouseTracking(false);
    _mViewer->setCameraManipulator(manipulator);
    _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    _mViewer->realize();

    /// todo create items
    createRobotScene();

}


osg::Geode *drawCoordinate(float a_x, float a_y, float a_z, float font_size) {
    osg::ref_ptr<osg::Sphere> pSphereShape = new osg::Sphere(osg::Vec3(0, 0, 0), 20.0f);
    osg::ref_ptr<osg::ShapeDrawable> pShapeDrawable = new osg::ShapeDrawable(pSphereShape.get());
    pShapeDrawable->setColor(osg::Vec4(0.5, 0.5, 0.5, 1.0));

    //创建保存几何信息的对象
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    //创建四个顶点
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
    v->push_back(osg::Vec3(a_x, 0.0f, 0.0f));

    v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
    v->push_back(osg::Vec3(0.0f, a_y, 0.0f));
    v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));

    v->push_back(osg::Vec3(0.0f, 0.0f, a_z));
    geom->setVertexArray(v.get());


    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
    c->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    c->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));

    c->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    c->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    c->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));

    c->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    geom->setColorArray(c.get());
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);


    //xyz
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 2, 2));
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 4, 2));


    osg::ref_ptr<osgText::Text> pTextXAuxis1 = new osgText::Text;
    pTextXAuxis1->setText("X");
    pTextXAuxis1->setFont("arial.ttf");
    pTextXAuxis1->setColor(osg::Vec4(0, 0, 0, 1));
    pTextXAuxis1->setAxisAlignment(osgText::Text::SCREEN);
    pTextXAuxis1->setCharacterSize(font_size);
    pTextXAuxis1->setPosition(osg::Vec3(a_x + 50, 0.0f, 0.0f));

    osg::ref_ptr<osgText::Text> pTextYAuxis1 = new osgText::Text;
    pTextYAuxis1->setText("Y");
    pTextYAuxis1->setFont("arial.ttf");
    pTextYAuxis1->setColor(osg::Vec4(0, 0, 0, 1));
    pTextYAuxis1->setAxisAlignment(osgText::Text::SCREEN);
    pTextYAuxis1->setCharacterSize(font_size);
    pTextYAuxis1->setPosition(osg::Vec3(0.0f, a_y + 50, 0.0f));

    osg::ref_ptr<osgText::Text> pTextZAuxis1 = new osgText::Text;
    pTextZAuxis1->setText("Z");
    pTextZAuxis1->setFont("arial.ttf");
    pTextZAuxis1->setColor(osg::Vec4(0, 0, 0, 1));
    pTextZAuxis1->setAxisAlignment(osgText::Text::SCREEN);
    pTextZAuxis1->setCharacterSize(font_size);
    pTextZAuxis1->setPosition(osg::Vec3(0.0f, 0.0f, a_z + 50));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(4.0), osg::StateAttribute::ON);

    geode->addDrawable(pShapeDrawable.get());
    geode->addDrawable(geom.get());
    geode->addDrawable(pTextXAuxis1.get());

    geode->addDrawable(pTextYAuxis1.get());
    geode->addDrawable(pTextZAuxis1.get());

    float coneRadius = 20.0f;
    float coneHeight = 100.0f;
    if (font_size <= 30) {
        coneRadius = 20.0f;
        coneHeight = 50.0f;
    }


    // Add cone arrows
    osg::ref_ptr<osg::Cone> xCone = new osg::Cone(osg::Vec3(0.0, 0.0f, 0.0f), coneRadius, coneHeight);
    osg::ref_ptr<osg::ShapeDrawable> xConeDrawable = new osg::ShapeDrawable(xCone.get());
    xConeDrawable->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::MatrixTransform> xTransform = new osg::MatrixTransform();
    xTransform->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0.0f, 1.0f, 0.0f)) *
                          osg::Matrix::translate(a_x, 0, 0));
    xTransform->addChild(xConeDrawable.get());

    osg::ref_ptr<osg::Cone> yCone = new osg::Cone(osg::Vec3(0.0f, 0.0, 0.0f), coneRadius, coneHeight);
    osg::ref_ptr<osg::ShapeDrawable> yConeDrawable = new osg::ShapeDrawable(yCone.get());
    yConeDrawable->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::MatrixTransform> yTransform = new osg::MatrixTransform();
    yTransform->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(-90.0), osg::Vec3(1.0f, 0.0f, 0.0f)) *
                          osg::Matrix::translate(0, a_y, 0));
    yTransform->addChild(yConeDrawable.get());

    osg::ref_ptr<osg::Cone> zCone = new osg::Cone(osg::Vec3(0.0f, 0.0f, a_z), coneRadius, coneHeight);
    osg::ref_ptr<osg::ShapeDrawable> zConeDrawable = new osg::ShapeDrawable(zCone.get());
    zConeDrawable->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    osg::ref_ptr<osg::MatrixTransform> zTransform = new osg::MatrixTransform();
    zTransform->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(0.0), osg::Vec3(0.0f, 0.0f, 1.0f)));
    zTransform->addChild(zConeDrawable.get());

    geode->addChild(xTransform.get());
    geode->addChild(yTransform.get());
    geode->addChild(zTransform.get());

    return geode.release();
}


osg::ref_ptr<osg::Geode> createGrid(float width, float height, int numLines, float lineWidth, const osg::Vec4 &color) {
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
    osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES);

    float dx = width / numLines;
    float dy = height / numLines;

    // Create grid lines
    for (int i = 0; i <= numLines; ++i) {
        float x = -width / 2.0f + i * dx;
        float y = -height / 2.0f + i * dy;

        // Vertical lines
        vertices->push_back(osg::Vec3(x, -height / 2.0f, 0.0f));
        vertices->push_back(osg::Vec3(x, height / 2.0f, 0.0f));

        // Horizontal lines
        vertices->push_back(osg::Vec3(-width / 2.0f, y, 0.0f));
        vertices->push_back(osg::Vec3(width / 2.0f, y, 0.0f));

        indices->push_back(4 * i);
        indices->push_back(4 * i + 1);
        indices->push_back(4 * i + 2);
        indices->push_back(4 * i + 3);

        // Set color for each vertex
        colors->push_back(color);
        colors->push_back(color);
        colors->push_back(color);
        colors->push_back(color);
    }

    geometry->setVertexArray(vertices);
    geometry->setColorArray(colors, osg::Array::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(indices);

    // Set line width
    osg::ref_ptr<osg::LineWidth> lineWidthAttribute = new osg::LineWidth(lineWidth);
    geometry->getOrCreateStateSet()->setAttributeAndModes(lineWidthAttribute, osg::StateAttribute::ON);

    // Enable color material
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2f, 0.2f, 0.2f, 1.0f));
    material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    geometry->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON);
    geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    geode->addDrawable(geometry);

    return geode;
}

osg::Node *loadSTLModel(const std::string &filePath, double r, double g, double b) {
    // 加载STL模型
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(filePath);
    if (model.valid()) {
        // 重新设置节点规模后，重新设置法向量
        model->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
    }
    // 创建材质对象并设置颜色属性
    osg::ref_ptr<osg::Material> material = new osg::Material;
    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    material->setDiffuse(osg::Material::FRONT, osg::Vec4(r / 255.0, g / 255.0, b / 255.0, 1.0f)); // 设置漫射光颜色为红色

    // 将材质对象与模型的状态集关联起来
    osg::StateSet *stateSet = model->getOrCreateStateSet();
    stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);

    return model.release();
}

osg::Camera *createHUDCamera(double left, double right, double bottom, double top) {
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;

    // 设置投影矩阵
    camera->setProjectionMatrix(osg::Matrix::ortho2D(left, right, bottom, top));

    // 设置视图矩阵
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());

    // 设置渲染顺序
    camera->setRenderOrder(osg::Camera::POST_RENDER);
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    return camera.release();
}

//osg::ref_ptr<osg::Camera> OsgWidget::createHUDText() {
//    osg::ref_ptr<osg::Camera> camera = new osg::Camera();
//
//    // 设置投影矩阵
//    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1280, 0, 800));
//
//    // 设置视图矩阵，同事确保不被场景中其它图形位置变换影响，使用绝对帧引用
//    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//    camera->setViewMatrix(osg::Matrix::identity());
//
//    // 清除深度缓存
//    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
//
//    // 设置渲染顺序为POST
//    camera->setRenderOrder(osg::Camera::POST_RENDER);
//
//    // 设置为不接收时间，始终不得到焦点
//    camera->setAllowEventFocus(false);
//
//    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
//    osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
//
//    // 关闭光照
//    stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//
//    // 关闭深度测试
//    stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
//
//
//    // 设置文字
//    _mRenderText = new osgText::Text;
//    osg::ref_ptr<osgText::Font> font = new osgText::Font();
//    font = osgText::readFontFile("arial.ttf");
//    _mRenderText->setFont(font.get());
//    _mRenderText->setText("x: y: z: \nrx: ry: rz: ");
//    _mRenderText->setPosition(osg::Vec3(30.0, 760.0, 0));
//    _mRenderText->setCharacterSize(20.0);
//    _mRenderText->setColor(osg::Vec4(0.1, 0.1, 0.1, 1.0));
//    _mRenderText->setDrawMode(osgText::Text::TEXT);
//
//    geode->addDrawable(_mRenderText.get());
//    camera->addChild(geode.get());
//
//    return camera.get();
//}


void OsgWidget::createRobotScene() {
    osg::ref_ptr<osg::Group> root = new osg::Group();

//    auto posTextRender = createHUDText();
//    _mCoordMap["posText"] = posTextRender;
//    root->addChild(posTextRender);

    auto grid = createGrid(2000, 2000, 20, 2, osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
    root->addChild(grid);
    _mCoordMap["grid"] = grid;

    auto worldCoord = drawCoordinate(1200, 1200, 1200, 60);
    root->addChild(worldCoord);
    _mCoordMap["worldCoord"] = worldCoord;

    /// 桌子
    osg::ref_ptr<osg::Node> deskModel = osgDB::readNodeFile("meshes/other/desk.stl");
    applyColorAndMaterial(deskModel, osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f));
    osg::ref_ptr<osg::PositionAttitudeTransform> deskModelPAT = new osg::PositionAttitudeTransform;
    deskModelPAT->addChild(deskModel);
    deskModelPAT->setPosition(osg::Vec3(0.0f, 0.0f, 500.0f));
    root->addChild(deskModelPAT);
    _mCoordMap["desk"] = deskModel;

    ///                                    机器人
    /// base
    osg::ref_ptr<osg::Node> baselinkModel = osgDB::readNodeFile("meshes/aubo_i5/base.stl");
    applyColorAndMaterial(baselinkModel, osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::PositionAttitudeTransform> baselinkPAT = new osg::PositionAttitudeTransform;
    baselinkPAT->addChild(baselinkModel);
//    auto baseCoord = drawCoordinate(800, 800, 800, 30);
//    baselinkPAT->addChild(baseCoord);

    /// shoulder
    osg::ref_ptr<osg::Node> shoulderlinkModel = osgDB::readNodeFile("meshes/aubo_i5/shoulder.stl");
    applyColorAndMaterial(shoulderlinkModel, osg::Vec4(255.0 / 255.0f, 127.0 / 255.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::PositionAttitudeTransform> shoulderlinkPAT = new osg::PositionAttitudeTransform;
    shoulderlinkPAT->setName("shoulder");

    shoulderlinkPAT->setAttitude(osg::Quat(3.1416, osg::Vec3(0.0f, 0.0f, 1.0f)));
    shoulderlinkPAT->setPosition(osg::Vec3d(0, 0, 0.122 * 1000));

    shoulderlinkPAT->addChild(shoulderlinkModel);
//    auto shoulderCoord = drawCoordinate(800, 800, 800, 30);
//    shoulderlinkPAT->addChild(shoulderCoord);
    _mJointInitMap["shoulder"] = shoulderlinkPAT->getAttitude();


    /// upperarm
    osg::ref_ptr<osg::Node> upperArmlinkModel = osgDB::readNodeFile("meshes/aubo_i5/upperarm.stl");
    applyColorAndMaterial(upperArmlinkModel, osg::Vec4(255.0 / 255.0f, 127.0 / 255.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::PositionAttitudeTransform> upperArmlinkPAT = new osg::PositionAttitudeTransform;
    upperArmlinkPAT->setName("upperarm");

    upperArmlinkPAT->setAttitude(
            osg::Quat(-1.5708, osg::Vec3(1.0f, 0.0f, 0.0f)) * osg::Quat(-1.5708, osg::Vec3(0.0f, 1.0f, 0.0f)));
    upperArmlinkPAT->setPosition(osg::Vec3d(0, 0.1215 * 1000, 0));

    upperArmlinkPAT->addChild(upperArmlinkModel);
//    auto upperarmCoord = drawCoordinate(800, 800, 800, 30);
//    upperArmlinkPAT->addChild(upperarmCoord);
    _mJointInitMap["upperarm"] = upperArmlinkPAT->getAttitude();


    /// forearm
    osg::ref_ptr<osg::Node> forearmlinkModel = osgDB::readNodeFile("meshes/aubo_i5/forearm.stl");
    applyColorAndMaterial(forearmlinkModel, osg::Vec4(255.0 / 255.0f, 127.0 / 255.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::PositionAttitudeTransform> forearmlinkPAT = new osg::PositionAttitudeTransform;
    forearmlinkPAT->setName("forearm");

    forearmlinkPAT->setAttitude(
            osg::Quat(-3.1416, osg::Vec3(1.0f, 0.0f, 0.0f)) * osg::Quat(-5.1632e-18, osg::Vec3(0.0f, 1.0f, 0.0f)) *
            osg::Quat(-5.459e-16, osg::Vec3(0.0f, 0.0f, 1.0f)));
    forearmlinkPAT->setPosition(osg::Vec3d(0.408 * 1000, 0, 0));

    forearmlinkPAT->addChild(forearmlinkModel);
//    auto forearmCoord = drawCoordinate(800, 800, 800, 30);
//    forearmlinkPAT->addChild(forearmCoord);
    _mJointInitMap["forearm"] = forearmlinkPAT->getAttitude();


    /// wrist1
    osg::ref_ptr<osg::Node> wrist1Model = osgDB::readNodeFile("meshes/aubo_i5/wrist1.stl");
    applyColorAndMaterial(wrist1Model, osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::PositionAttitudeTransform> wrist1PAT = new osg::PositionAttitudeTransform;
    wrist1PAT->setName("wrist1");

    wrist1PAT->setAttitude(osg::Quat(3.1416, osg::Vec3(1.0f, 0.0f, 0.0f))
                           * osg::Quat(-1.8323E-15, osg::Vec3(0.0f, 1.0f, 0.0f))
                           * osg::Quat(1.5708, osg::Vec3(0.0f, 0.0f, 1.0f)));
    wrist1PAT->setPosition(osg::Vec3d(0.376 * 1000, 0, 0));

    wrist1PAT->addChild(wrist1Model);
//    auto wrist1Coord = drawCoordinate(800, 800, 800, 30);
//    wrist1PAT->addChild(wrist1Coord);
    _mJointInitMap["wrist1"] = wrist1PAT->getAttitude();


    /// wrist2
    osg::ref_ptr<osg::Node> wrist2Model = osgDB::readNodeFile("meshes/aubo_i5/wrist2.stl");
    applyColorAndMaterial(wrist2Model, osg::Vec4(255.0 / 255.0f, 127.0 / 255.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::PositionAttitudeTransform> wrist2PAT = new osg::PositionAttitudeTransform;
    wrist2PAT->setName("wrist2");

    wrist2PAT->setAttitude(osg::Quat(-1.5708, osg::Vec3(1.0f, 0.0f, 0.0f))
                           * osg::Quat(-1.8709E-15, osg::Vec3(0.0f, 1.0f, 0.0f))
                           * osg::Quat(-1.6653E-16, osg::Vec3(0.0f, 0.0f, 1.0f)));
    wrist2PAT->setPosition(osg::Vec3d(0, 0.1025 * 1000, 0));

    wrist2PAT->addChild(wrist2Model);
//    auto wrist2Coord = drawCoordinate(800, 800, 800, 30);
//    wrist2PAT->addChild(wrist2Coord);
    _mJointInitMap["wrist2"] = wrist2PAT->getAttitude();


    /// wrist3
    osg::ref_ptr<osg::Node> wrist3Model = osgDB::readNodeFile("meshes/aubo_i5/wrist3.stl");
    applyColorAndMaterial(wrist3Model, osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    osg::ref_ptr<osg::PositionAttitudeTransform> wrist3PAT = new osg::PositionAttitudeTransform;
    wrist3PAT->setName("wrist3");

    wrist3PAT->setAttitude(osg::Quat(1.5708, osg::Vec3(1.0f, 0.0f, 0.0f))
                           * osg::Quat(0, osg::Vec3(0.0f, 1.0f, 0.0f))
                           * osg::Quat(1.7907E-15, osg::Vec3(0.0f, 0.0f, 1.0f)));

    wrist3PAT->setPosition(osg::Vec3d(0, -0.094 * 1000, 0));

    wrist3PAT->addChild(wrist3Model);
    auto wrist3Coord = drawCoordinate(300, 300, 300, 30);
    wrist3PAT->addChild(wrist3Coord);
    _mCoordMap["wrist3Coord"] = wrist3Coord;
    _mJointInitMap["wrist3"] = wrist3PAT->getAttitude();


    baselinkPAT->addChild(shoulderlinkPAT);
    shoulderlinkPAT->addChild(upperArmlinkPAT);
    upperArmlinkPAT->addChild(forearmlinkPAT);
    forearmlinkPAT->addChild(wrist1PAT);
    wrist1PAT->addChild(wrist2PAT);
    wrist2PAT->addChild(wrist3PAT);


    deskModelPAT->addChild(baselinkPAT);

    //优化场景数据
    osgUtil::Optimizer optimizer;
    optimizer.optimize(root.get());
    _mViewer->setSceneData(root.get());

}

void OsgWidget::changeObservationSlot(OBSERVATION_ENUM e) {
    if (e == 0) {
        /// 初始
        dynamic_cast<osgGA::TrackballManipulator *>(_mViewer->getCameraManipulator())->setTransformation(
                {2000, -2700, 1800}, {0, 0, 800}, {0, 0, 1.57});
    } else if (e == 1) {
        /// 正视图
        dynamic_cast<osgGA::TrackballManipulator *>(_mViewer->getCameraManipulator())->setTransformation(
                {0, -2500, 800}, {0, 0, 1000}, {0, 0, 0});
    } else if (e == 2) {
        /// 俯视图
        dynamic_cast<osgGA::TrackballManipulator *>(_mViewer->getCameraManipulator())->setTransformation(
                {0, 0, 2700}, {0, 0, 0}, {0, 0, 0});
    } else if (e == 3) {
        /// 侧视图
        dynamic_cast<osgGA::TrackballManipulator *>(_mViewer->getCameraManipulator())->setTransformation(
                {3000, 0, 1800}, {0, 0, 900}, {0, 0, 1.57});
    }
}

void OsgWidget::updateJoint(std::string nodeName, float var) {
    auto findNode = findNodeInGroup(nodeName);
    if (!findNode) {
        return;
    }
    auto _mNode = dynamic_cast<osg::PositionAttitudeTransform *>(findNode);
    osg::Quat _mRot(osg::DegreesToRadians(var), osg::Vec3(0, 0, 1));
    _mNode->setAttitude(_mRot * _mJointInitMap[_mNode->getName()]);
}

void OsgWidget::updateJoints(const std::vector<double> &v) {
    FindPATsVisitor findPATsVisitor;
    dynamic_cast<osg::Group *>(_mViewer->getSceneData())->accept(findPATsVisitor);
    auto nodes = findPATsVisitor.getNodes();
//    qDebug()<<v;
    for (int i = 0; i < nodes.size(); ++i) {
        osg::Quat _mRot(v[i], osg::Vec3(0, 0, 1));
        nodes[i]->setAttitude(_mRot * _mJointInitMap[nodes[i]->getName()]);
    }
//    qDebug()<<"ik suc";
}

void OsgWidget::updateSceneDisplay(std::string nodeName, bool f) {
    if (_mCoordMap.find(nodeName) == _mCoordMap.end()) {
        return;
    }
    if (f)
        _mCoordMap[nodeName]->setNodeMask(-1);
    else
        _mCoordMap[nodeName]->setNodeMask(0);

}

void
OsgWidget::setRendText(QString x, QString y, QString z, QString rx, QString ry, QString rz) {
    QVector<QString> t{x, y, z, rx, ry, rz};
    QVector<QString> t2{"X:", "Y:", "Z:", "Rx:", "Ry:", "Rz:"};
    auto ls = _mRenderWidget->findChildren<QLabel *>();
    for (int i = 0; i < ls.size(); ++i) {
        ls[i]->setText(t2[i] + t[i]);
    }
}

QWidget *OsgWidget::createTextRenderWidget() {
    auto frame = new QFrame(this);
    frame->setStyleSheet("background-color: rgba(180, 180, 180, 50);");
    frame->setGeometry(10, 10, 240, 80);
    auto textlayout = new QGridLayout(frame);
    auto x = new QLabel(this);
    auto y = new QLabel(this);
    auto z = new QLabel(this);
    auto rx = new QLabel(this);
    auto ry = new QLabel(this);
    auto rz = new QLabel(this);
    x->setStyleSheet("background-color: rgba(100, 100, 100, 0);");
    y->setStyleSheet("background-color: rgba(100, 100, 100, 0);");
    z->setStyleSheet("background-color: rgba(100, 100, 100, 0);");
    rx->setStyleSheet("background-color: rgba(100, 100, 100, 0);");
    ry->setStyleSheet("background-color: rgba(100, 100, 100, 0);");
    rz->setStyleSheet("background-color: rgba(100, 100, 100, 0);");
    textlayout->addWidget(x, 0, 0);
    textlayout->addWidget(y, 0, 1);
    textlayout->addWidget(z, 0, 2);
    textlayout->addWidget(rx, 1, 0);
    textlayout->addWidget(ry, 1, 1);
    textlayout->addWidget(rz, 1, 2);
    frame->setLayout(textlayout);
    return frame;
}

osg::ref_ptr<osg::Node> OsgWidget::readQFile(QString fp) {
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(fp.toStdString());
    return model;
}
