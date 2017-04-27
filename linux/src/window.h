#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>

#include "particlelist.h"
#include "particlewidget.h"

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void newParticle();
    void searchParticle(QString name);
    void xChanged(double value);
    void yChanged(double value);
    void zChanged(double value);
    void xRotaSliderChanged(int value);
    void xRotaSlPressed(void);
    void xRotaSlReleased(void);
    void yRotaSliderChanged(int value);
    void yRotaSlPressed(void);
    void yRotaSlReleased(void);
    void zRotaSliderChanged(int value);
    void zRotaSlPressed(void);
    void zRotaSlReleased(void);
    void xRotaSBChanged(double value);
    void yRotaSBChanged(double value);
    void zRotaSBChanged(double value);
    void xTraslaSlChange(double value);
    void xTraslaSBChange(int value);
    void yTraslaSlChange(double value);
    void yTraslaSBChange(int value);
    void zTraslaSlChange(double value);
    void zTraslaSBChange(int value);

signals:
    void addParticle(ParticleWidget newParticle);
    void changeParticle(int index);
    void definedName(QString name);
    void sizeChanged(double x, double y, double z);

private:
    bool repitedName(QString);


    float xFactor, yFactor, zFactor;

    QLabel *xSizeLabel;
    QDoubleSpinBox *xSizeSB;

    QLabel *ySizeLabel;
    QDoubleSpinBox *ySizeSB;

    QLabel *zSizeLabel;
    QDoubleSpinBox *zSizeSB;

    QPushButton *addBtn;
    QPushButton *deleteBtn;

    QRadioButton *XZView;
    QRadioButton *XYView;
    QRadioButton *YZView;
    ParticleList *list;

    QLabel *xRotaLabel;
    QSlider *xRotaSlider;
    QDoubleSpinBox *xRotaSB;

    QLabel *yRotaLabel;
    QSlider *yRotaSlider;
    QDoubleSpinBox *yRotaSB;

    QLabel *zRotaLabel;
    QSlider *zRotaSlider;
    QDoubleSpinBox *zRotaSB;

    QLabel *xTraslaLabel;
    QSlider *xTraslaSlider;
    QDoubleSpinBox *xTraslaSB;

    QLabel *yTraslaLabel;
    QSlider *yTraslaSlider;
    QDoubleSpinBox *yTraslaSB;

    QLabel *zTraslaLabel;
    QSlider *zTraslaSlider;
    QDoubleSpinBox *zTraslaSB;

    QPushButton *finish;

    GLWidget *glWidget;

    MainWindow *mainWindow;
    QVector<ParticleWidget> particle;
};

#endif
