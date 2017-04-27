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

#include "particlewidget.h"
#include <fstream>
#include <iostream>
#include <QInputDialog>
#include <QtGui>



using namespace std;

ParticleWidget::ParticleWidget(QWidget *parent)
    :QWidget(parent)
{

}

ParticleWidget::ParticleWidget(const ParticleWidget &particle, QWidget *parent)
    :QWidget(parent)
{
    *this = particle;
}

ParticleWidget::ParticleWidget(QString file_name, QWidget *parent)throw(error)
    :QWidget(parent)
{
    string element;
    float x, y, z;
    QVector<float> max, min;
    max.resize(3);
    min.resize(3);
    max.fill(0);
    min.fill(0);
    ifstream input;
    string lpm = file_name.toStdString();
    input.open(lpm.c_str());

    if(!input)
    {
        cerr << "Error al abrir el archivo";
        throw empty_file;
    }

    input >> count;
    for(int i = 0; i < count; i++)
    {
        input >> element;
        input >> x;
        input >> y;
        input >> z;
        if(x > max[0])
            max[0] = x;
        else if(x < min[0])
            min[0] = x;
        if(y > max[1])
            max[1] = y;
        else if(y < min[1])
            min[1] = y;
        if(z > max[2])
            max[2] = z;
        else if(z < min[2])
            min[2] = z;
        elem.push_back(element);
        data.push_back(x);
        data.push_back(y);
        data.push_back(z);
        dimension = QVector3D(max[0] - min[0], max[1] - min [1], max[2] - min[2]);
        center = QVector3D((max[0] + min[0])*.5, (max[1] + min[1])*.5, (max[2]+min[2])*.5);
        }

    input.close();
    setName();
    if(name.isEmpty())
        throw name_error;
}

QVector<GLfloat> ParticleWidget::constData() const
{
    QVector<GLfloat> normalized;
    QVector<GLfloat>::iterator norm_it;
    QVector<GLfloat>::iterator data_it;
    double module = dimension.length();
    normalized.resize(data.length());
    norm_it = normalized.begin();
    for(data_it = (QVector<GLfloat>::iterator) data.begin(); data_it < data.end(); data_it++)
    {
        *norm_it = ESCALA * (*data_it - center.x())/module;
        norm_it++;
        data_it++;
        *norm_it = ESCALA * (*data_it - center.y())/module;
        norm_it++;
        data_it++;
        *norm_it = ESCALA * (*data_it - center.z())/module;
        norm_it++;
    }
    return normalized;//.begin();
}

QVector<GLfloat> ParticleWidget::constData(float xDim, float yDim, float zDim) const
{
    QVector<GLfloat> normalized;
    QVector<GLfloat>::iterator norm_it;
    QVector<GLfloat>::iterator data_it;
    normalized.resize(data.length());
    norm_it = normalized.begin();
    for(data_it = (QVector<GLfloat>::iterator) data.begin(); data_it < data.end(); data_it++)
    {
        *norm_it = ESCALA * 2 * (*data_it - center.x())/xDim;
        norm_it++;
        data_it++;
        *norm_it = ESCALA * 2 * (*data_it - center.y())/yDim;
        norm_it++;
        data_it++;
        *norm_it = ESCALA * 2 * (*data_it - center.z())/zDim;
        norm_it++;
    }
    return normalized;//.begin();
}

void ParticleWidget::setName()
{
    QString titulo = tr("Nombre de la Nanopartícula"), etiqueta = tr("Ingrese Nombre de la Nanopartícula");
    name = QInputDialog::getText(0, titulo, etiqueta);
}

ParticleWidget& ParticleWidget::operator=(const ParticleWidget& particle)
{
    this->elem.resize(particle.length());
    this->elem = particle.getElement();
    this->data.resize(particle.length());
    this->data = particle.getData();
    this->name = particle.getName();
    this->count = particle.length();
    this->center = particle.getCenter();
    this->dimension = particle.getDimension();

    return *this;
}
