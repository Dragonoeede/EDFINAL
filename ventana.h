/**
 * @ file TDA VEntana
 */

#ifndef VENTANA_H
#define VENTANA_H

#include <QWidget>
#include "tablerografico.h"
#include "consola.h"
#include <iostream>
#include <QMessageBox>
#include <cstdlib>
#include <QHBoxLayout>
#include <string>

/**
 * @brief The Ventana class. Un objeto de la clase Ventana esta compuesta por dos objetos uno de tipo
 * consola y otro de tipo TableroGrafico.
 *
 * Author: Rosa Mª Rodríguez Sánchez
 * Date : 31/07/2024
 */

class Ventana : public QWidget
{
    //meta object compiler for QT (moc)
    Q_OBJECT //una macro para poder hacer conexiones Signal-Slot

private:
    TableroGrafico * tg; //puntero al tablero grafico
    Consola * con; //puntero a la consola
    bool terminar_juego; //controla si se termina el juego

public:
    /**
     * @brief Constructor por defecto y parámetros
     * @param tg: puntero al tablero grafico
     * @param C: puntero a la consola
     * @param str: nombre de la ventana grafica
     * @param parent: Padre grafico de la ventana
     */
    explicit Ventana(TableroGrafico *tg, Consola *C,std::string str, QWidget *parent = nullptr);



    /**
     * @brief devuelve el tablero grafico
     * @return un puntero al tablero
     */
    TableroGrafico * getTableroGrafico();



    /**
     * @brief devuelve la consola
     * @return un puntero a la consola
     */
    Consola * getConsola();


    /**
     * @brief cierra la ventana para finalizar el programa
     */
    void cerrar();


    /**
     * @brief Gestiona el cierre de la aplicación.
     * @param event: puntero al evento
     */
    void closeEvent (QCloseEvent * event);


    void delay(int ms){
        QThread::msleep(ms);
    }
};

#endif // VENTANA_H

