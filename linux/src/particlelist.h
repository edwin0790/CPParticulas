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

#ifndef PARTICLELIST_H
#define PARTICLELIST_H

#include "errors.h"
#include <QListView>
#include <QStringListModel>
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>

class ParticleList: public QWidget
{
    Q_OBJECT

public:
    //Constructor y Destructor
    ParticleList(QWidget *parent = 0);

    //Declaracion de funciones virtuales a definir
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;


public slots:
    void addParticle(const QString particle)throw(error);
    void activeChanged(QModelIndex activated);
    void eraseParticle();

signals:
    void activate(uint active);

private:
    int index;
    QStringListModel *listModel;
    QStringList *strList;
    QListView *view;
};

#endif // PARTICLELIST_H
