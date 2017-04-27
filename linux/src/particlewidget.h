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

#ifndef PARTICLEWIDGET_H
#define PARTICLEWIDGET_H

#include "errors.h"
#include <QWidget>
#include <QOpenGLContext>
#include <QVector3D>

#define ESCALA .1
#define TRUE 1
#define FALSE 0

class ParticleWidget: public QWidget
{
    Q_OBJECT

public:
    //Constructores y destructor
    ParticleWidget(QWidget *parent = 0);
    ParticleWidget(QString file_name, QWidget *parent = 0)throw(error);
    ParticleWidget(const ParticleWidget &particle, QWidget *parent = 0);
    virtual ~ParticleWidget(void){}

    //Funciones necesarias
    QVector<GLfloat> constData() const;
    QVector<GLfloat> constData(float xDim, float yDim, float zDim) const;
    int length() const{return data.length();}
    QString getName()const {return name;}
    QVector<std::string> getElement() const {return elem;}
    QVector3D getCenter() const {return center;}
    QVector3D getDimension() const {return dimension;}
    QVector<GLfloat> getData() const{return data;}
    ParticleWidget& operator=(const ParticleWidget&);

private:
    //Funciones privadas
    void setName();

    //Propiedades de la clase
    QVector<GLfloat> data;
    QVector<std::string> elem;
    QString name;
    int count;
    QVector3D center;
    QVector3D dimension;

};

#endif // PARTICLEWIDGET_H
