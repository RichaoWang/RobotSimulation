#include "stlfileloader.h"
//#include <GL/glu.h>
#include <QDebug>
#include <QFile>
#include <fstream>
#include <iostream>
#include <QtOpenGL>

STLFileLoader::STLFileLoader(QString filename, float ratio) {
    mRatio = ratio;
    model.clear();
    loadStl(filename);
}

STLFileLoader::~STLFileLoader() {

}

void STLFileLoader::loadStl(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray arr;
        arr = file.read(5);
        file.close();
        if (arr == "solid") {
            loadTextStl(filename);
        } else {
            loadBinaryStl(filename);
        }
    } else {
        qDebug() << filename << u8"不存在";
    }
}

//加载ASCII格式STL文件
void STLFileLoader::loadTextStl(const QString &filename) {
    qDebug() << "load text file:" << filename;
    model.clear(); //清除模型
    QList <QVector3D> triangle;
    STLTriangle tSTLTriangle;
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line = file.readLine().trimmed();
            QStringList words = line.split(' ', Qt::SplitBehavior::enum_type::SkipEmptyParts);

            if (words[0] == "facet") {
                triangle.clear();
                tSTLTriangle.reset();
                tSTLTriangle.setNormal(words[2].toFloat(), words[3].toFloat(), words[4].toFloat());
            } else if (words[0] == "vertex") {
                triangle.append(QVector3D(words[1].toFloat(), words[2].toFloat(), words[3].toFloat()));
            } else if (words[0] == "endloop") {
                if (triangle.length() == 3) {
                    for (int i = 0; i < 3; ++i) {
                        tSTLTriangle.setVertex(i, triangle[i]);
                    }
                    model.append(tSTLTriangle);
                }
            }
        }
        file.close();
    }
}

//加载二进制格式STL文件，速度更快
void STLFileLoader::loadBinaryStl(const QString &filename) {
    qDebug() << "load Binary file:" << filename;
    model.clear();          //清除模型
    QList <QVector3D> triangle;
    STLTriangle tSTLTriangle;

    QFile STL_file(filename);

    int fileSize = STL_file.size();
    char *buf = (char *) malloc(sizeof(char) * fileSize);

    bool isOk = STL_file.open(QIODevice::ReadOnly);
    if (!isOk) return;

    QDataStream stream(&STL_file);
    stream.readRawData(buf, fileSize);
    STL_file.close();

    const char *p = buf;
    char name[80];          //起始80个字节 文件名
    int triangle_num;       //4个字节 三角形个数
    float n1, n2, n3;       //法向量
    float v1, v2, v3;       //定点

    memcpy(name, p, 80);                        //记录文件名
    p += 80;                                    //跳过文件名

    memcpy(&triangle_num, p, 4);                //记录三角形个数
    p += 4;                                     //跳过个数标识

    for (int i = 0; i < triangle_num; i++) {     //读取法向量
        memcpy(&n1, p, 4);
        p += 4;
        memcpy(&n2, p, 4);
        p += 4;
        memcpy(&n3, p, 4);
        p += 4;

        triangle.clear();
        tSTLTriangle.reset();
        tSTLTriangle.setNormal(n1, n2, n3);

        for (int j = 0; j < 3; j++) {             //读取顶点信息
            memcpy(&v1, p, 4);
            p += 4;
            memcpy(&v2, p, 4);
            p += 4;
            memcpy(&v3, p, 4);
            p += 4;
            triangle.append(QVector3D(v1, v2, v3));
        }
        if (triangle.length() == 3) {
            for (int i = 0; i < 3; ++i) {
                tSTLTriangle.setVertex(i, triangle[i]);
            }
            model.append(tSTLTriangle);
        }
        p += 2;//跳过尾部标志 两字节
    }
    free(buf);
}

void STLFileLoader::draw() {
    QList <STLTriangle> triangles = model;
    QVector3D normal;
    QVector3D vertex;
    glBegin(GL_TRIANGLES); // 绘制一个或多个三角形
    foreach(STLTriangle tri, triangles) {
        normal = tri.getNormal();
        glNormal3f(mRatio * normal.x(), mRatio * normal.y(), mRatio * normal.z());
        for (int j = 0; j < 3; ++j) {
            vertex = tri.getVertex(j);
            glVertex3f(mRatio * vertex.x(), mRatio * vertex.y(), mRatio * vertex.z());
        }
    }
    glEnd();
}

STLTriangle::STLTriangle() {
    reset();
}

void STLTriangle::setVertex(int index, QVector3D point3D) {
    if (!checkVertexIndex(index)) {
        return;
    }
    vector_vertex[index] = point3D;
}

QVector3D STLTriangle::getVertex(int index) {
    if (!checkVertexIndex(index)) {
        return QVector3D();
    }
    return vector_vertex[index];
}

void STLTriangle::setNormal(float nx, float ny, float nz) {
    vector_normal = QVector3D(nx, ny, nz);
}

QVector3D STLTriangle::getNormal() {
    return vector_normal;
}

void STLTriangle::reset() {
    vector_normal = QVector3D(0.f, 0.f, 0.f);
    for (int i = 0; i < 3; ++i) {
        vector_vertex[i] = QVector3D(0.f, 0.f, 0.f);
    }
}

bool STLTriangle::checkVertexIndex(int index) {
    if (index < 0 || index > 2) {
        qDebug() << "CRITICAL: invalid index provided to STLTriangle::SetVertex()!";
        return false;
    }
    return true;
}
