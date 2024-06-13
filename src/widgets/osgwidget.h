//
// Created by JerryWang on 24-6-2.
//

#ifndef ROBOTSIMULATION_OSGWIDGET_H
#define ROBOTSIMULATION_OSGWIDGET_H

#include <QOpenGLWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QDebug>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>
#include <osg/MatrixTransform>
#include <osg/LineWidth>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/Switch>

enum OBSERVATION_ENUM {
    INIT, FONT, TOP, SIDE
};


class OsgNodeVisitor : public osg::NodeVisitor {
public:
    OsgNodeVisitor(const std::string &name)
            : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), _name(name), _foundNode(nullptr) {}

    virtual void apply(osg::Node &node) {
        if (node.getName() == _name) {
            _foundNode = &node;
        }
        traverse(node);
    }

    osg::Node *getFoundNode() const { return _foundNode; }

private:
    std::string _name;
    osg::Node *_foundNode;
};

class FindPATsVisitor : public osg::NodeVisitor {
public:
    FindPATsVisitor()
            : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}

    virtual void apply(osg::Node &node) {
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(&node);
        if (pat && !pat->getName().empty()) {
            _mPATs.push_back(pat);
        }
        // 继续遍历子节点
        traverse(node);
    }

    std::vector<osg::PositionAttitudeTransform *> getNodes() { return _mPATs; }

private:
    std::vector<osg::PositionAttitudeTransform *> _mPATs;
};


class OsgWidget : public QOpenGLWidget {
public:
    OsgWidget(QWidget *parent = 0);

    virtual ~OsgWidget() {}

    void createRobotScene();

    osg::ref_ptr<osg::Node> readQFile(QString fp);

    // 创建一个应用颜色和材质的函数
    void applyColorAndMaterial(osg::Node *node, const osg::Vec4 &color) {
        if (!node) return;

        osg::StateSet *stateSet = node->getOrCreateStateSet();

        // 创建材质并设置颜色
        osg::ref_ptr<osg::Material> material = new osg::Material;
        material->setDiffuse(osg::Material::FRONT_AND_BACK, color);
        material->setAmbient(osg::Material::FRONT_AND_BACK, color * 0.05); // 设置环境光颜色为漫反射颜色的20%
//        material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); // 设置高光颜色为白色
//        material->setShininess(osg::Material::FRONT_AND_BACK, 50.0f); // 设置高光系数

        // 将材质应用到节点
        stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);

        // 启用颜色数组，如果存在
        osg::Geode *geode = node->asGeode();
        if (geode) {
            for (unsigned int i = 0; i < geode->getNumDrawables(); ++i) {
                osg::Geometry *geometry = geode->getDrawable(i)->asGeometry();
                if (geometry) {
                    osg::Vec4Array *colors = dynamic_cast<osg::Vec4Array *>(geometry->getColorArray());
                    if (colors) {
                        stateSet->setMode(GL_COLOR_MATERIAL, osg::StateAttribute::ON);
                    }
                }
            }
        }
    }

    osg::Node *findNodeInGroup(const std::string &nodeName) {
        OsgNodeVisitor findNodeVisitor(nodeName);
        dynamic_cast<osg::Group *>(_mViewer->getSceneData())->accept(findNodeVisitor);

        return findNodeVisitor.getFoundNode();
    }

    void updateJoint(std::string nodeName, float var);

    void updateJoints(const std::vector<double> &v);

    void updateSceneDisplay(std::string nodeName, bool f);

//    osg::ref_ptr<osg::Camera> createHUDText();

    QWidget *createTextRenderWidget();

public slots:

    void setRendText(QString x, QString y, QString z, QString rx, QString ry, QString rz);

protected:

    virtual void paintGL();

    virtual void resizeGL(int width, int height);

    virtual void initializeGL();

    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);

    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void wheelEvent(QWheelEvent *event);

    virtual bool event(QEvent *event);

public slots:

    void changeObservationSlot(OBSERVATION_ENUM);

private:

    osgGA::EventQueue *getEventQueue() const {
        osgGA::EventQueue *eventQueue = _mGraphicsWindow->getEventQueue();
        return eventQueue;
    }

    osg::ref_ptr<osgViewer::Viewer> _mViewer;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;
    qreal m_scale;

    std::map<std::string, osg::Quat> _mJointInitMap;

    std::map<std::string, osg::Node *> _mCoordMap;

//    osg::ref_ptr<osgText::Text> _mRenderText;

    QWidget *_mRenderWidget ;
};


#endif //ROBOTSIMULATION_OSGWIDGET_H

