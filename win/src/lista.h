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

#ifndef LISTA_H
#define LISTA_H

#include "errors.h"
#include <QListView>
#include <QStringListModel>
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>

class Lista: public QWidget
{
    Q_OBJECT

public:
    //Constructor y Destructor
    Lista(QWidget *parent = 0);

    //Declaracion de funciones virtuales a definir
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;


public slots:
    void agregar(const QString nano)throw(error);
    void aviso(QModelIndex activado);
    void borrar();

signals:
    void activar(uint activo);

private:
    int index;
    QStringListModel *lista;
    QStringList *strings;
    QListView *muestra;
};

#endif // LISTA_H
