#include "ventana.h"


Ventana::Ventana(TableroGrafico * tg, Consola *con,std::string str,QWidget *parent)
    : QWidget{parent}
{
    QString qstr = QString::fromStdString(str.c_str());
    this->setObjectName(qstr);
    QHBoxLayout * bl = new QHBoxLayout(this);
    if (con!=nullptr)
      bl->addWidget(con);

    if (tg!=nullptr)
      bl->addWidget(tg);
    terminar_juego=false;
}
/****************************************************************/
TableroGrafico * Ventana::getTableroGrafico(){
    return tg;
}

/****************************************************************/
Consola * Ventana::getConsola(){
    return con;
}
/****************************************************************/
void Ventana::cerrar(){
    terminar_juego=true;
    close();
}

/****************************************************************/
void Ventana::closeEvent (QCloseEvent * event)
{

    if (terminar_juego==true){
        event->accept();
        exit(1);
    }
    else{
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "3raya",
                                                               tr("Estas seguro?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
        } else {
            event->accept();
            exit(1);
        }
    }
}



