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
