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
