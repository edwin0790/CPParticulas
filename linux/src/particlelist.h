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
