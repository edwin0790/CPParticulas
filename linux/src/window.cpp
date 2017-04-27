#include "glwidget.h"
#include "window.h"
#include "mainwindow.h"
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QErrorMessage>

Window::Window(MainWindow *mw)
    : mainWindow(mw),
      particle(0)
{
    //Lattice Config
    QGroupBox *latticeConfig = new QGroupBox("Informacion de red");

    //X_size: Label, SpinBox
    QHBoxLayout *size = new QHBoxLayout(latticeConfig);

    xSizeLabel = new QLabel("Ancho", latticeConfig);
    xSizeSB = new QDoubleSpinBox(latticeConfig);
    xSizeSB->setRange(1,10000);
    xSizeSB->setSingleStep(.1);

    size->addWidget(xSizeLabel, 0, Qt::AlignLeft);
    size->addWidget(xSizeSB, 1, Qt::AlignLeft);

    //Y_Size: Label, SpinBox

    ySizeLabel = new QLabel("Alto", latticeConfig);
    ySizeSB = new QDoubleSpinBox(latticeConfig);
    ySizeSB->setRange(1, 10000);
    ySizeSB->setSingleStep(.1);

    size->addWidget(ySizeLabel, 0, Qt::AlignLeft);
    size->addWidget(ySizeSB, 1, Qt::AlignLeft);

//    //Z_Size: Label, SpinBox

    zSizeLabel = new QLabel("Profundidad",latticeConfig);
    zSizeSB = new QDoubleSpinBox(latticeConfig);
    zSizeSB->setRange(1, 10000);
    zSizeSB->setSingleStep(.1);

    size->addWidget(zSizeLabel, 0, Qt::AlignLeft);
    size->addWidget(zSizeSB, 1, Qt::AlignLeft);

    //Selector de Nanopartículas;

    QGroupBox *selector = new QGroupBox("Particulas");
    QVBoxLayout *selectLO = new QVBoxLayout(selector);

    //Botonera
    QWidget *buttonsTopWidget = new QWidget(selector);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsTopWidget);

    addBtn = new QPushButton(tr("Agregar Partícula"),buttonsTopWidget);
    deleteBtn = new QPushButton(tr("Eliminar Partícula"),buttonsTopWidget);

    //Vista
    QGroupBox *view = new QGroupBox(tr("Vista"), buttonsTopWidget);
    QHBoxLayout *viewLayout = new QHBoxLayout(view);

    XYView = new QRadioButton(tr("XY"), view);
    XYView->toggle();
    XZView = new QRadioButton(tr("XZ"), view);
    YZView = new QRadioButton(tr("YZ"), view);

    viewLayout->addWidget(XYView, 0, Qt::AlignLeft);
    viewLayout->addWidget(XZView, 0, Qt::AlignLeft);
    viewLayout->addWidget(YZView, 0, Qt::AlignLeft);

    buttonsLayout->addWidget(addBtn, 0, Qt::AlignLeft);
    buttonsLayout->addWidget(deleteBtn, 0, Qt::AlignLeft);
    buttonsLayout->addWidget(view, 0 , Qt::AlignLeft);

    //lista
    QWidget *listDisplay = new QWidget;
    QHBoxLayout *listLayout = new QHBoxLayout(listDisplay);

    list = new ParticleList(listDisplay);
    listLayout->addWidget(list, 0, Qt::AlignCenter);

    selectLO->addWidget(buttonsTopWidget, 0, Qt::AlignCenter);
    selectLO->addWidget(listDisplay, 0, Qt::AlignCenter);

    //Rotaciones y Traslaciones
    QGroupBox *modifs = new QGroupBox(tr("Posición"));
    QHBoxLayout *interact = new QHBoxLayout(modifs);

    //x_Rotacion = Cabeceo;
    QWidget *pitchWidget = new QWidget(modifs);
    QVBoxLayout *pitchLayout = new QVBoxLayout(pitchWidget);
    xRotaLabel = new QLabel("α");

    xRotaSlider = new QSlider(Qt::Vertical, pitchWidget);
    xRotaSlider->setRange(0, 360 * 2);
    xRotaSlider->setSingleStep(1);
    xRotaSlider->setPageStep(15 * 2);
    xRotaSlider->setTickInterval(30 * 2);
    xRotaSlider->setTickPosition(QSlider::TicksRight);

    xRotaSB = new QDoubleSpinBox(pitchWidget);
    xRotaSB->setRange(0, 360);
    xRotaSB->setSingleStep(.1);
    xRotaSB->setValue(0);

    pitchLayout->addWidget(xRotaLabel, 0 , Qt::AlignCenter);
    pitchLayout->addWidget(xRotaSlider);
    pitchLayout->addWidget(xRotaSB);

    //y_Rotacion = Viraje
    QWidget *yawWidget = new QWidget(modifs);
    QVBoxLayout *yawLayout = new QVBoxLayout(yawWidget);

    yRotaLabel = new QLabel("β");

    yRotaSlider = new QSlider(Qt::Vertical, yawWidget);
    yRotaSlider->setRange(0, 360 * 2);
    yRotaSlider->setSingleStep(1);
    yRotaSlider->setPageStep(15 * 2);
    yRotaSlider->setTickInterval(30 * 2);
    yRotaSlider->setTickPosition(QSlider::TicksRight);

    yRotaSB = new QDoubleSpinBox(yawWidget);
    yRotaSB->setRange(0, 360);
    yRotaSB->setSingleStep(.5);
    yRotaSB->setValue(0);

    yawLayout->addWidget(yRotaLabel, 0, Qt::AlignCenter);
    yawLayout->addWidget(yRotaSlider);
    yawLayout->addWidget(yRotaSB);

    //z_Rotacion = Inclinacion
    QWidget *rollWidget = new QWidget(modifs);
    QVBoxLayout *rollLayout = new QVBoxLayout(rollWidget);

    zRotaLabel = new QLabel("γ");

    zRotaSlider = new QSlider(Qt::Vertical, rollWidget);
    zRotaSlider->setRange(0, 360 * 2);
    zRotaSlider->setSingleStep(1);
    zRotaSlider->setPageStep(15 * 2);
    zRotaSlider->setTickInterval(30 * 2);
    zRotaSlider->setTickPosition(QSlider::TicksRight);

    zRotaSB = new QDoubleSpinBox(rollWidget);
    zRotaSB->setRange(0, 360);
    zRotaSB->setSingleStep(.5);
    zRotaSB->setValue(0);

    rollLayout->addWidget(zRotaLabel, 0, Qt::AlignCenter);
    rollLayout->addWidget(zRotaSlider);
    rollLayout->addWidget(zRotaSB);

    //x_Traslacion
    QWidget *straightWidget = new QWidget(modifs);
    QVBoxLayout *straightLayout = new QVBoxLayout(straightWidget);

    xTraslaLabel = new QLabel("X");

    xTraslaSlider = new QSlider(Qt::Vertical, straightWidget);
    xTraslaSlider->setRange(-500, 500);
    xTraslaSlider->setSingleStep(1);
    xTraslaSlider->setPageStep(10);
    xTraslaSlider->setTickInterval(50);
    xTraslaSlider->setTickPosition(QSlider::TicksRight);

    xTraslaSB = new QDoubleSpinBox(straightWidget);
    xTraslaSB->setRange(-xSizeSB->value()/2, xSizeSB->value()/2);
    xTraslaSB->setSingleStep(xSizeSB->value()/1000);
    xTraslaSB->setValue(0);

    straightLayout->addWidget(xTraslaLabel, 0, Qt::AlignCenter);
    straightLayout->addWidget(xTraslaSlider);
    straightLayout->addWidget(xTraslaSB);

    //y_Traslacion
    QWidget *upWidget = new QWidget(modifs);
    QVBoxLayout *upLayout = new QVBoxLayout(upWidget);

    yTraslaLabel = new QLabel("Y");

    yTraslaSlider = new QSlider(Qt::Vertical, upWidget);
    yTraslaSlider->setRange(-500, 500);
    yTraslaSlider->setSingleStep(1);
    yTraslaSlider->setPageStep(10);
    yTraslaSlider->setTickInterval(50);
    yTraslaSlider->setTickPosition(QSlider::TicksRight);

    yTraslaSB = new QDoubleSpinBox(upWidget);
    yTraslaSB->setRange(-ySizeSB->value()/2, ySizeSB->value()/2);
    yTraslaSB->setSingleStep(ySizeSB->value()/1000);
    yTraslaSB->setValue(0);

    upLayout->addWidget(yTraslaLabel, 0, Qt::AlignCenter);
    upLayout->addWidget(yTraslaSlider);
    upLayout->addWidget(yTraslaSB);

    //z_Traslacion
    QWidget *depthWidget = new QWidget(modifs);
    QVBoxLayout *depthLayout = new QVBoxLayout(depthWidget);

    zTraslaLabel = new QLabel("Z");

    zTraslaSlider = new QSlider(Qt::Vertical, depthWidget);
    zTraslaSlider->setRange(-500, 500);
    zTraslaSlider->setSingleStep(1);
    zTraslaSlider->setPageStep(10);
    zTraslaSlider->setTickInterval(50);
    zTraslaSlider->setTickPosition(QSlider::TicksRight);

    zTraslaSB = new QDoubleSpinBox(depthWidget);
    zTraslaSB->setRange(-zSizeSB->value()/2, zSizeSB->value()/2);
    zTraslaSB->setSingleStep(zSizeSB->value()/100);
    zTraslaSB->setValue(0);

    depthLayout->addWidget(zTraslaLabel, 0, Qt::AlignCenter);
    depthLayout->addWidget(zTraslaSlider);
    depthLayout->addWidget(zTraslaSB);

    //union
    interact->addWidget(pitchWidget);
    interact->addWidget(yawWidget);
    interact->addWidget(rollWidget);
    interact->addWidget(straightWidget);
    interact->addWidget(upWidget);
    interact->addWidget(depthWidget);


    //Entramado final


    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;
    QWidget *w = new QWidget;
    finish = new QPushButton(tr("Finalizar"),w);

    glWidget = new GLWidget;

    mainLayout->addWidget(latticeConfig);
    mainLayout->addWidget(selector);
    mainLayout->addWidget(modifs);
    mainLayout->addWidget(finish);
    w->setLayout(mainLayout);

    container->addWidget(w, 1);
    container->addWidget(glWidget, 1);
    setLayout(container);
    setWindowTitle(tr("Hello GL"));


    //Conecciones entre slots y señales
    //Sliders y SpinBoxes
    connect(xSizeSB, SIGNAL(valueChanged(double)), this, SLOT(xChanged(double)));
    connect(ySizeSB, SIGNAL(valueChanged(double)), this, SLOT(yChanged(double)));
    connect(zSizeSB, SIGNAL(valueChanged(double)), this, SLOT(zChanged(double)));
    xSizeSB->setValue(10);
    ySizeSB->setValue(10);
    zSizeSB->setValue(10);
    connect(xRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(xRotaSliderChanged(int)));
    connect(xRotaSlider, SIGNAL(sliderPressed()), this, SLOT(xRotaSlPressed()));
    connect(xRotaSlider, SIGNAL(sliderReleased()), this, SLOT(xRotaSlReleased()));
    connect(xRotaSB, SIGNAL(valueChanged(double)), this, SLOT(xRotaSBChanged(double)));

    connect(yRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(yRotaSliderChanged(int)));
    connect(yRotaSlider, SIGNAL(sliderPressed()), this, SLOT(yRotaSlPressed()));
    connect(yRotaSlider, SIGNAL(sliderReleased()), this, SLOT(yRotaSlReleased()));
    connect(yRotaSB, SIGNAL(valueChanged(double)), this, SLOT(yRotaSBChanged(double)));

    connect(zRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(zRotaSliderChanged(int)));
    connect(zRotaSlider, SIGNAL(sliderPressed()), this, SLOT(zRotaSlPressed()));
    connect(zRotaSlider, SIGNAL(sliderReleased()), this, SLOT(zRotaSlReleased()));
    connect(zRotaSB, SIGNAL(valueChanged(double)), this, SLOT(zRotaSBChanged(double)));

    connect(xTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(xTraslaSBChange(int)));
    connect(xTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(xTraslaSlChange(double)));

    connect(yTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(yTraslaSBChange(int)));
    connect(yTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(yTraslaSlChange(double)));

    connect(zTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(zTraslaSBChange(int)));
    connect(zTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(zTraslaSlChange(double)));

    //Valores y glWidget
    connect(xRotaSB, SIGNAL(valueChanged(double)), glWidget, SLOT(setXRotation(double)));
    connect(yRotaSB, SIGNAL(valueChanged(double)), glWidget, SLOT(setYRotation(double)));
    connect(zRotaSB, SIGNAL(valueChanged(double)), glWidget, SLOT(setZRotation(double)));
    connect(glWidget, SIGNAL(xRotationChanged(double)), xRotaSB, SLOT(setValue(double)));
    connect(glWidget, SIGNAL(yRotationChanged(double)), yRotaSB, SLOT(setValue(double)));
    connect(glWidget, SIGNAL(zRotationChanged(double)), zRotaSB, SLOT(setValue(double)));
    connect(xTraslaSB, SIGNAL(valueChanged(double)), glWidget, SLOT(setXTraslation(double)));
    connect(yTraslaSB, SIGNAL(valueChanged(double)), glWidget, SLOT(setYTraslation(double)));
    connect(zTraslaSB, SIGNAL(valueChanged(double)), glWidget, SLOT(setZTraslation(double)));
    connect(glWidget, SIGNAL(xTraslationChanged(double)), xTraslaSB, SLOT(setValue(double)));
    connect(glWidget, SIGNAL(yTraslationChanged(double)), yTraslaSB, SLOT(setValue(double)));
    connect(glWidget, SIGNAL(zTraslationChanged(double)), zTraslaSB, SLOT(setValue(double)));
    connect(XZView, SIGNAL(toggled(bool)), glWidget, SLOT(zView(bool)));
    connect(YZView, SIGNAL(toggled(bool)), glWidget, SLOT(yView(bool)));

    //ParticleWidgets y glWidget
    connect(this, SIGNAL(addParticle(ParticleWidget)), glWidget, SLOT(addParticle(ParticleWidget)));
    connect(this, SIGNAL(sizeChanged(double,double,double)), glWidget, SLOT(setSpace(double,double,double)));

    //ParticleWidgets y lista
    connect(this, SIGNAL(definedName(QString)), list, SLOT(addParticle(QString)));

    //Botones
    connect(addBtn, SIGNAL(clicked()), this, SLOT(newParticle()));
    connect(deleteBtn, SIGNAL(clicked()), glWidget, SLOT(eraseParticle()));
    connect(deleteBtn, SIGNAL(clicked()), list, SLOT(eraseParticle()));
    connect(finish, SIGNAL(clicked()), glWidget, SLOT(outFileGenerator()));

    //lista y glWidget
    connect(list, SIGNAL(activate(uint)), glWidget, SLOT(selectParticle(uint)));


    mw->setMaximumSize(this->size());

}

void Window::xChanged(double value)
{
    xTraslaSB->setRange(-xSizeSB->value()/2, xSizeSB->value()/2);
    xTraslaSB->setSingleStep(value/1000);
    xFactor = xSizeSB->value() / 1000;
    emit sizeChanged(xSizeSB->value(), ySizeSB->value(), zSizeSB->value());
}

void Window::yChanged(double value)
{
    yTraslaSB->setRange(-ySizeSB->value()/2, ySizeSB->value()/2);
    yTraslaSB->setSingleStep(value/1000);
    yFactor = ySizeSB->value() / 1000;
    emit sizeChanged(xSizeSB->value(), ySizeSB->value(), zSizeSB->value());
}

void Window::zChanged(double value)
{
    zTraslaSB->setRange(-zSizeSB->value()/2, zSizeSB->value()/2);
    zTraslaSB->setSingleStep(value/1000);
    zFactor = zSizeSB->value() / 1000;
    emit sizeChanged(xSizeSB->value(), ySizeSB->value(), zSizeSB->value());
}

void Window::xRotaSliderChanged(int value)
{
    disconnect(xRotaSB, SIGNAL(valueChanged(double)), this, SLOT(xRotaSBChanged(double)));
    xRotaSB->setValue(double(value) / 2);
    connect(xRotaSB, SIGNAL(valueChanged(double)), this, SLOT(xRotaSBChanged(double)));
}

void Window::xRotaSBChanged(double value)
{
    disconnect(xRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(xRotaSliderChanged(int)));
    xRotaSlider->setValue(int(value * 2));
    connect(xRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(xRotaSliderChanged(int)));
}

void Window::yRotaSliderChanged(int value)
{
    disconnect(yRotaSB, SIGNAL(valueChanged(double)), this, SLOT(yRotaSBChanged(double)));
    yRotaSB->setValue(double(value) / 2);
    connect(yRotaSB, SIGNAL(valueChanged(double)), this, SLOT(yRotaSBChanged(double)));
}

void Window::yRotaSBChanged(double value)
{
    disconnect(yRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(yRotaSliderChanged(int)));
    yRotaSlider->setValue(int(value * 2));
    connect(yRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(yRotaSliderChanged(int)));
}

void Window::zRotaSliderChanged(int value)
{
    disconnect(zRotaSB, SIGNAL(valueChanged(double)), this, SLOT(zRotaSBChanged(double)));
    zRotaSB->setValue(double(value) / 2);
    connect(zRotaSB, SIGNAL(valueChanged(double)), this, SLOT(zRotaSBChanged(double)));
}

void Window::zRotaSBChanged(double value)
{
    disconnect(zRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(zRotaSliderChanged(int)));
    zRotaSlider->setValue(int(value * 2));
    connect(zRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(zRotaSliderChanged(int)));
}

void Window::searchParticle(QString name)
{
    QVector<ParticleWidget>::iterator ptr;
    int count = 0, index = 0;
    for(ptr = particle.begin(); ptr < particle.end(); ptr++)
    {
        if(name == ptr->getName())
            index = count;
        count++;
    }
    emit changeParticle(index);
}


void Window::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_8:
        yTraslaSlider->setValue(yTraslaSlider->value() + 10);
        break;
    case Qt::Key_2:
        yTraslaSlider->setValue(yTraslaSlider->value() - 10);
        break;
    case Qt::Key_6:
        xTraslaSlider->setValue(xTraslaSlider->value() + 10);
        break;
    case Qt::Key_4:
        xTraslaSlider->setValue(xTraslaSlider->value() - 10);
        break;
    case Qt::Key_Plus:
        zTraslaSlider->setValue(zTraslaSlider->value() + 10);
        break;
    case Qt::Key_Minus:
        zTraslaSlider->setValue(zTraslaSlider->value() - 10);
        break;
    QWidget::keyPressEvent(e);
    }
}

void Window::newParticle()
{
    ParticleWidget *aux;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Abrir"),0,tr("Archivos XYZ(*.xyz)"));

    if(!fileName.isEmpty())
    {
        try
        {
            aux = new ParticleWidget(fileName);
            emit definedName(aux->getName());

            particle.push_back(*aux);
            emit addParticle(*aux);
        }
        catch(error)
        {
            QErrorMessage *message = new QErrorMessage(this);
            message->showMessage("Error al definir el nombre de la particula."
                                 "Ingresela nuevamente");
        }
    }

}

void Window::xTraslaSlChange(double value)
{
    disconnect(xTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(xTraslaSBChange(int)));
    xTraslaSlider->setValue(int(value / xFactor));
    connect(xTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(xTraslaSBChange(int)));
}

void Window::xTraslaSBChange(int value)
{
    disconnect(xTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(xTraslaSlChange(double)));
    xTraslaSB->setValue(double(value) * xFactor);
    connect(xTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(xTraslaSlChange(double)));
}

void Window::yTraslaSlChange(double value)
{
    disconnect(yTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(yTraslaSBChange(int)));
    yTraslaSlider->setValue(int(value / yFactor));
    connect(yTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(yTraslaSBChange(int)));
}

void Window::yTraslaSBChange(int value)
{
    disconnect(yTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(yTraslaSlChange(double)));
    yTraslaSB->setValue(double(value) * yFactor);
    connect(yTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(yTraslaSlChange(double)));
}

void Window::zTraslaSlChange(double value)
{
    disconnect(zTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(zTraslaSBChange(int)));
    zTraslaSlider->setValue(int(value / zFactor));
    connect(zTraslaSlider, SIGNAL(valueChanged(int)), this, SLOT(zTraslaSBChange(int)));
}

void Window::zTraslaSBChange(int value)
{
    disconnect(zTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(zTraslaSlChange(double)));
    zTraslaSB->setValue(double(value) * zFactor);
    connect(zTraslaSB, SIGNAL(valueChanged(double)), this, SLOT(zTraslaSlChange(double)));
}

void Window::xRotaSlPressed()
{
    disconnect(xRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(xRotaSliderChanged(int)));
}

void Window::xRotaSlReleased()
{
    connect(xRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(xRotaSliderChanged(int)));
    xRotaSliderChanged(xRotaSlider->value());
}

void Window::yRotaSlPressed()
{
    disconnect(yRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(yRotaSliderChanged(int)));
}

void Window::yRotaSlReleased()
{
    connect(yRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(yRotaSliderChanged(int)));
    yRotaSliderChanged(yRotaSlider->value());
}

void Window::zRotaSlPressed()
{
    disconnect(zRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(zRotaSliderChanged(int)));
}

void Window::zRotaSlReleased()
{
    connect(zRotaSlider, SIGNAL(valueChanged(int)), this, SLOT(zRotaSliderChanged(int)));
    zRotaSliderChanged(zRotaSlider->value());
}
