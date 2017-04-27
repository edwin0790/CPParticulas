/*
*    Copyright (C) 2016  Edwin Barragán
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QApplication>
#include <QDesktopWidget>
#include "particlewidget.h"

using namespace std;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void setSpace(double x, double y, double z);
    void setXRotation(double angle);
    void setYRotation(double angle);
    void setZRotation(double angle);
    void setXTraslation(double dist);
    void setYTraslation(double dist);
    void setZTraslation(double dist);
    void cleanup();
    void addParticle(ParticleWidget newParticle);
    void selectParticle(uint idx);
    void zView(bool xz);
    void yView(bool yz);
    void eraseParticle();
    void outFileGenerator();

signals:
    void xRotationChanged(double angle);
    void yRotationChanged(double angle);
    void zRotationChanged(double angle);
    void xTraslationChanged(double dist);
    void yTraslationChanged(double dist);
    void zTraslationChanged(double dist);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void setupVertexAttribs();
    void renormalize(QVector3D lastSpaceSize);
    void setPointsToDraw();

    bool m_core;
    bool end;
    uint index;
    QVector3D spaceSize;
    QVector<uint> offset;
    QVector<uint> size;
    vector<QVector3D> rotation;
    vector<QVector3D> traslation;
    QPoint m_lastPos;
    QVector<std::string> element;
    QVector<GLfloat> atom;
    QVector<GLfloat> drawPoint;
    QOpenGLBuffer particleVbo;
    QOpenGLShaderProgram *m_program;
    int m_projLoc;
    QMatrix4x4 m_proj;
    QMatrix4x4 world;
};

#endif
