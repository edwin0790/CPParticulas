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

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QFileDialog>
#include <QOpenGLFunctions>
#include <fstream>
#include <math.h>

#define POINT_SIZE 1.5f

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      end(FALSE),
      index(0),
      spaceSize(10, 10, 10),
      m_program(0)
{
    offset.push_back(0);
    size.push_back(0);
    rotation.push_back(QVector3D(0,0,0));
    traslation.push_back(QVector3D(0,0,0));
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
//    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
//    if (m_transparent)
//        setAttribute(Qt::WA_TranslucentBackground);
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return sizeHint();
}

QSize GLWidget::sizeHint() const
{
    int h = QApplication::desktop()->height();
    return QSize(h * .7, h * .7);
}

static void qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360;
    while (angle > 360)
        angle -= 360;
}

void GLWidget::cleanup()
{
    makeCurrent();
    particleVbo.destroy();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

void GLWidget::addParticle(const ParticleWidget newParticle)
{
    //Tamaño para normalización
    float x = spaceSize.x(), y = spaceSize.y(), z = spaceSize.z();

    //Si no existen particulas, reinicializo valores y variables
    if(!index)
    {
        element.clear();
        atom.clear();
        offset.clear();
        size.clear();
        rotation.clear();
        traslation.clear();
    }

    //carga de datos
    offset.push_back(atom.length());
    index = offset.length();
    size.push_back(newParticle.length());

    element.append(newParticle.getElement());
    atom.append(newParticle.constData(x, y, z));

    rotation.push_back(QVector3D(1,1,1));
    traslation.push_back(QVector3D(1,1,1));

    setXRotation(0);
    setXTraslation(0);
    setYRotation(0);
    setYTraslation(0);
    setZRotation(0);
    setZTraslation(0);

    makeCurrent();
    particleVbo.destroy();
    particleVbo.create();
    particleVbo.bind();
    particleVbo.allocate(atom.length() * sizeof(GLfloat));
    setupVertexAttribs();
    doneCurrent();
    update();
}

void GLWidget::eraseParticle()
{
    if(index)
    {
        QVector<GLfloat>::iterator firstAtom;
        QVector<std::string>::iterator firstElement;
        firstAtom = atom.begin() + offset[index - 1];
        firstElement = element.begin() + offset[index - 1] / 3;
        element.erase(firstElement, firstElement + (size[index - 1] / 3));
        atom.erase(firstAtom, firstAtom + size[index - 1]);
        size.erase(size.begin() + index - 1);
        for(uint indx = index;int (indx) < (offset.length()); indx++)
        {
                offset[indx] =  offset[indx - 1] + size[indx - 1];
                rotation[indx - 1] = rotation [indx];
                traslation[indx - 1] = traslation[indx];
        }
        offset.erase(offset.end() - 1);
        rotation.erase((rotation.end() - 1));
        traslation.erase((traslation.end() - 1));
        if(int(index) > offset.length())
            index = offset.length();

        makeCurrent();
        particleVbo.destroy();
        particleVbo.create();
        particleVbo.bind();
        particleVbo.allocate(atom.length() * sizeof(GLfloat));
        setupVertexAttribs();
        doneCurrent();
        update();
    }
}

static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "uniform mat4 projMatrix;\n/*"
    "void main() {\n"
    "   gl_Position = projMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "void main() {\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   fragColor = vec4(color, 1.0);\n"
    "}\n";

static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    "uniform mat4 projMatrix;\n"
    "void main() {\n"
    "   gl_Position = projMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "void main() {\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

// verde fosforescente .39, 1, 0

void GLWidget::initializeGL()
{
//    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);//241 241 241 || 236 239 241

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->link();

    m_program->bind();
    m_projLoc = m_program->uniformLocation("projMatrix");

    // Setup our vertex buffer object.
    particleVbo.create();
    particleVbo.bind();
//    particleVbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    ParticleWidget init;
    particleVbo.allocate(init.length());

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    m_program->release();
}

void GLWidget::setupVertexAttribs()
{
    particleVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    particleVbo.release();
}

void GLWidget::setPointsToDraw()
{
    QVector<GLfloat>::iterator ii, jj;

    drawPoint.resize(atom.length());
    jj = drawPoint.begin();
    ii = atom.begin();

    for(int indx = 0; indx < offset.length(); indx++)
    {
        QMatrix4x4 rotationMatrix, traslationMatrix;
        rotationMatrix.setToIdentity();
        rotationMatrix.rotate(rotation[indx].x(), 1, 0, 0);
        rotationMatrix.rotate(rotation[indx].y(), 0, 1, 0);
        rotationMatrix.rotate(rotation[indx].z(), 0, 0 ,1);
        traslationMatrix.setToIdentity();
        traslationMatrix.translate(traslation[indx]/spaceSize*2);

        for(uint i = offset[indx]; i < offset[indx] + size[indx]; i += 3)
        {
            QVector4D multiplicand(*ii, *(ii+1), *(ii+2), 1);
            QVector4D product;
            product = rotationMatrix * multiplicand;
            product = traslationMatrix * product;
            product = world * product;
            *jj = product.x();
            jj ++;
            *jj = product.y();
            jj ++;
            *jj = product.z();
            jj ++;
            ii += 3;
        }
    }
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    setPointsToDraw();

    if(drawPoint.length())
    {
        particleVbo.bind();
        particleVbo.write(0, drawPoint.constData(), drawPoint.length() * sizeof(GLfloat));
    }

    m_program->bind();
    m_program->setUniformValue(m_projLoc, m_proj*.5);

    glPointSize(POINT_SIZE);
    glDrawArrays(GL_POINTS, particleVbo.bufferId(), drawPoint.length());

    m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
//    m_proj.perspective(45.0f, float(w)/h * .5, 0.001f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(rotation.at(index - 1).x() + dy);
        setYRotation(rotation.at(index - 1).y() + dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(rotation.at(index - 1).x() + dy);
        setZRotation(rotation.at(index - 1).z() + dx);
    }
    m_lastPos = event->pos();
}

void GLWidget::setXRotation(double angle)
{
    qNormalizeAngle(angle);
    if (angle != rotation[index - 1].x())
    {
        rotation[index - 1].setX(angle);
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(double angle)
{
    qNormalizeAngle(angle);
    if (angle !=rotation[index - 1].y())
    {
        rotation[index - 1].setY(angle);
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(double angle)
{
    qNormalizeAngle(angle);
    if (angle != rotation[index - 1].z())
    {
        rotation[index - 1].setZ(angle);
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::setXTraslation(double dist)
{
    if (dist != traslation[index - 1].x())
    {
        traslation[index - 1].setX(dist);
        emit xTraslationChanged(dist);
        update();
    }
}

void GLWidget::setYTraslation(double dist)
{
    if (dist != traslation[index - 1].y())
    {
        traslation[index - 1].setY(dist);
        emit yTraslationChanged(dist);
        update();
    }
}

void GLWidget::setZTraslation(double dist)
{
    if (dist != traslation[index - 1].z())
    {
        traslation[index - 1].setZ(dist);
        emit zTraslationChanged(dist);
        update();
    }
}

void GLWidget::selectParticle(uint idx)
{
    index = idx;
    index++;
    emit xRotationChanged(int (rotation[idx].x()));
    emit yRotationChanged(int (rotation[idx].y()));
    emit zRotationChanged(int (rotation[idx].z()));
    emit xTraslationChanged(double (traslation[idx].x()));
    emit yTraslationChanged(double (traslation[idx].y()));
    emit zTraslationChanged(double (traslation[idx].z()));
    update();
}

void GLWidget::setSpace(double x, double y, double z)
{
    QVector3D lastSpaceSize = spaceSize;
    spaceSize = QVector3D(x,y,z);
    renormalize(lastSpaceSize);
    emit xTraslationChanged(traslation[index - 1].x());
    emit yTraslationChanged(traslation[index - 1].y());
    emit zTraslationChanged(traslation[index - 1].z());
    update();
}

void GLWidget::renormalize(QVector3D lastSpaceSize)
{
    QVector<GLfloat>::iterator ii;
    vector<QVector3D>::iterator jj;
    QVector3D factor = lastSpaceSize/spaceSize;
    for(ii = atom.begin(); ii < atom.end(); ii++)
    {
        *ii = factor.x() * (*ii);
        ii++;
        *ii = factor.y() * (*ii);
        ii++;
        *ii = factor.z() * (*ii);
    }
    for(jj = traslation.begin(); jj < traslation.end(); jj++)
        *jj = (*jj) * factor;
}

void GLWidget::zView(bool xz)
{
    world.setToIdentity();
    if(xz)
        world.rotate(-90, 1, 0, 0);
    update();
}

void GLWidget::yView(bool yz)
{
    world.setToIdentity();
    if(yz)
    {
        world.rotate(-90, 0, 1, 0);
        world.rotate(-90, 1, 0, 0);
    }
    update();
}

void GLWidget::outFileGenerator()
{
    using namespace std;

    QString fileName;
    int i;
    ofstream file;

    world.setToIdentity();
    end = TRUE;
    setPointsToDraw();
    fileName = QFileDialog::getSaveFileName(0, tr("Guardar Archivo"), 0, tr("Archivos de texto sin formato(*.txt)"));
    if(!fileName.isEmpty())
    {
        QVector3D max = QVector3D(0, 0, 0), min = QVector3D(0, 0, 0);
        for(QVector<GLfloat>::iterator jj = drawPoint.begin(); jj < drawPoint.end(); jj++)
        {
            if(*jj > max.x())
                max.setX(*jj);
            if(*jj < min.x())
                min.setX(*jj);
            jj++;
            if(*jj > max.y())
                max.setY(*jj);
            if(*jj < min.y())
                min.setY(*jj);
            jj++;
            if(*jj > max.z())
                max.setZ(*jj);
            if(*jj < min.z())
                min.setZ(*jj);
        }

        QVector3D newSpace = QVector3D(spaceSize.x(), spaceSize.y(), ((max.z() - min.z()) * spaceSize.z() * 0.55));

        file.open(fileName.toStdString().c_str(), ios_base::trunc);
        for(i = 0; i < 6; i++)
        {
            file << "lattice\t|\t";
            file << i;
            file << "\t|\t";
            switch(i)
            {
            case 0:
                file << newSpace.x();
                break;
            case 1:
                file << newSpace.y();
                break;
            case 2:
                file << newSpace.z();
                break;
            default:
                file << 90.0;
                break;
            }
            file << endl << endl;
        }

        file << "rps|0|\tx\t,\ty\t,\tz"<< endl << endl;

        file.precision(3);
        file << fixed;

        i = 0;

        for(QVector<GLfloat>::iterator jj = drawPoint.begin(); jj < drawPoint.end(); jj++)
        {
            file << "atom\t|\t" << i << "\t|\t";
            file << element[i];
            file << "\t,\ta\t,\t";
            file << (*jj + 1) / 2;
            file << "\t,\t";
            jj++;
            file << (*jj + 1) / 2;
            file << "\t,\t";
            jj++;
            file << (*jj - (min.z() * 1.1)) * spaceSize.z() / 2 / newSpace.z();
            file << "\t,\t" << 0.005;//debye-waller
            file << "\t,\t" << 1;//occupancy
            file << "\t,\t" << 0.1;//absorption
            file << "\t,\tDef\t,\t" << 0;//no se que es Def, y aff
            file << endl << endl;
            i++;
        }


        file.close();
    }
    end = FALSE;
}
