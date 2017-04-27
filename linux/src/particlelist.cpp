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

#include "particlelist.h"

#include <QFileDialog>
#include <sstream>
#include <string>


ParticleList::ParticleList(QWidget *parent)
    :QWidget(parent),
      index(-1)
{
    listModel = new QStringListModel;
    view = new QListView(parent);
    strList = new QStringList;

    connect(view, SIGNAL(activated(QModelIndex)), this, SLOT(activeChanged(QModelIndex)));
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(activeChanged(QModelIndex)));
}

void ParticleList::activeChanged(QModelIndex activated)
{
    index = activated.row();

    emit activate(index);
}

QSize ParticleList::sizeHint() const
{
    int w = QApplication::desktop()->width();
    int h = QApplication::desktop()->height();
    return QSize(w * 0.3, h * 0.225);
}

void ParticleList::addParticle(const QString particle) throw(error)
{
    QStringList::Iterator ii;
    for(ii = strList->begin(); ii < strList->end(); ii++)
    {
        if(*ii == particle)
            throw(name_error);
    }
    strList->push_back(particle);
    listModel->setStringList(*strList);
    view->setModel(listModel);
    index = strList->length() - 1;
}

void ParticleList::resizeEvent(QResizeEvent* event)
{
    view->resize(event->size());
}

void ParticleList::eraseParticle()
{
    if(index >= 0)
    {
        strList->erase(strList->begin() + index);
        listModel->setStringList(*strList);
        view->setModel(listModel);
        if(index == strList->length() && index)
        {
            emit activate(index);
        }
        index--;
    }
}
