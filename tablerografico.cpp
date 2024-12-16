#include "tablerografico.h"
#include <iostream>
using namespace std;
MyButton::MyButton(QWidget * parent) :
    QPushButton(parent)
{
}


/****************************************************************/
TableroGrafico::TableroGrafico(int height, int  width,int nrows,int ncols,QWidget *parent): QWidget{parent}
{
    rows = nrows;
    cols = ncols;
    w=width;
    h =height;
    int w_cell = (w)/cols;
    int h_cell = (h)/rows;
    const QSize btnSize = QSize(w_cell,h_cell);
    MyButton *btn[rows*cols];
    for(int i = 0; i < rows*cols; i++) {
        btn[i] = new MyButton(this);
        btn[i]->setFixedSize(btnSize);
    }
    //reticula de botones
    tabla = new QGridLayout(this);
    for(int i = 0; i <rows; i++) {
        for(int j = 0; j < cols; j++) {
            tabla->addWidget(btn[j + i * cols], 0 + i, j);
            tabla->setSpacing(0);

        }

    }
    //Si alguna casilla se pulsa se gestiona
    connect(this, &TableroGrafico::btnRightClicked,
            this,      &TableroGrafico::pulsaCasilla);
    pulsada_casilla=false;
    active_seleccion=false;


}

/****************************************************************/
TableroGrafico::~TableroGrafico(){
    if (tabla!=nullptr)
        delete tabla;
}


/****************************************************************/
void TableroGrafico::pulsaCasilla(){

    if (active_seleccion){
        active_seleccion=false;
        pulsada_casilla=true;
        posrow = index_casilla/cols;
        poscol= index_casilla%cols;
    }

}

/****************************************************************/
void TableroGrafico::mouseDoubleClickEvent(QMouseEvent *event)
{


        QWidget * const widget = childAt(event->pos());

        if (widget) {

            const int index = this->layout()->indexOf(widget);

            if (index >=0 && active_seleccion) {
                index_casilla=index;

                emit btnRightClicked();
            }
        }

}

/****************************************************************/
void TableroGrafico::getCasillaElegida(int &r,int &c){
    this->pulsada_casilla=false;
    this->activeSeleccion(true);
    QThread *thread = QThread::create([this,&r,&c](){
        while(1)
            if (this->getPulsadaCasilla()==true){

                this->getCasilla(r,c);
                this->activeSeleccion(false);
                break;
            }

    });
    thread->start();
    QEventLoop loop;
    connect( thread, &QThread::finished, &loop, &QEventLoop::quit );
    loop.exec();

}



/****************************************************************/
void TableroGrafico::putImagen(int r,int c,const char * nameimg){
    QThread *thread = QThread::create([this,&r,&c,&nameimg](){
        QPixmap pixmap(nameimg);
        QIcon ButtonIcon(pixmap);
        QLayoutItem* item = tabla->itemAtPosition(r, c);
        QWidget* widget = item->widget();
        QPushButton* button = dynamic_cast<QPushButton*>(widget);
        button->setIcon(ButtonIcon);
        button->setIconSize(pixmap.rect().size()/3);
    });
    thread->start();
    QEventLoop loop;
    connect( thread, &QThread::finished, &loop, &QEventLoop::quit );
    loop.exec();
    //este bucle es necesario para refrescar la visualizacion
    while( thread->isFinished()==false);
}


/****************************************************************/
void TableroGrafico::PonerEnBlanco(const char *name){
    //const char * name ="./images/B.png";
    for (int i=0;i<rows;i++)
        for (int c=0;c<cols;c++)
            this->putImagen(i,c,name);


}
/****************************************************************/
int TableroGrafico::getNrows()const{
    return rows;
}

/****************************************************************/
int TableroGrafico::getNcols()const{
    return cols;
}

