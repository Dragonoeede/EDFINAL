/**
 * @ file tablerografico.h Describe las clases MyButton y TableroGrafico
 * */
#ifndef TABLEROGRAFICO_H
#define TABLEROGRAFICO_H


#include <QWidget>
#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QEventLoop>
#include <QThread>


/**
 * @brief  MyButton class. Un objeto de la clase MyButton es una boton grafico que permite ser clickeado
* TaBleroGrafioco class. Un objeto de la clase TableroGrafico es una representación grafica de una tabla
* con n filas y n casillas. Cada casilla puede almacenar o no una imagen.
* Author: Rosa Mª Rodríguez Sánchez
 * Date : 31/07/2024
 */


class MyButton : public QPushButton
{
    Q_OBJECT

public:
    /**
     * @brief MyButton Constructor por defecto
     * @param parent Puntero al objeto padre
     */
    MyButton(QWidget *parent = Q_NULLPTR);

signals:
    /**
     * @brief btnRightClicked Emite la señala de boton pulsado
     */
    void btnRightClicked();
};



/**
 * @brief The TableroGrafico class
 */

class TableroGrafico:public QWidget
{
    Q_OBJECT
private:

    QGridLayout *tabla; // una reticula donde poner las casillas
    bool pulsada_casilla, // flag para controlar si se ha pulsado sobre una casilla
        active_seleccion; // flag para controlar que el usuario debe seleccionar una casilla
    int index_casilla; // indice de la casilla seleccionada
    int h,w; //alto y ancho del tablero grafico
    int rows,cols; // numero de filas y columnas del tablero grafico
    int posrow,poscol; // posición de la fila y columna seleccionada

    QSize icon_size;
public:
    /**
     * @brief TableroGrafico constructor con parametros
     * @param height altura del tablero
     * @param weight ancho del tablero
     * @param nrow numero de filas
     * @param ncol numero de columnas
     * @param parent objeto padre del tablero grafico propio de Qt.
     */
    TableroGrafico(int height=400, int  width=800,int nrow=3,int ncol=3, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~TableroGrafico();
    /**
     * @brief activeSeleccion
     * @param ac variable que activa (true) la seleccion o desactiva la seleccion. Cuando ac es true el usuario puede seleccionar una casilla
     * cuando ac es false aunque pulse no se recoge la información de la casilla seleccionada. Debe activarse con true preio a que el usuario escoja casilla
     */
    void activeSeleccion(bool ac){
        active_seleccion=ac;
    }

    /**
     * @brief setPulsadaCasilla modifica el estado de si fue o no pulsada una casilla
     * @param pc true para indicar que la casilla ha sido pulsada. False en caso contrario. Para iniciar el proceso de que el usuario escoja casilla se debe poner a false.
     */
    void setPulsadaCasilla(bool pc){
        pulsada_casilla=pc;
    }

    /**
     * @brief getPulsadaCasilla Devuelve si ha sido o no pulsada una casilla
     * @return true en caso de haber pulsado una casill
     */
    bool getPulsadaCasilla()const{
        return pulsada_casilla;
    }


    /**
     * @brief getCasilla obtiene las coordenadas de la casilla pulsada
     * @param r fila de la casilla pulsada. Es MODIFICADO
     * @param c columna de la casilla pulsada. Es MODIFICADO
     */
    void getCasilla(int &r,int &c){

        r=posrow;
        c=poscol;
    }

    /**
     * @brief Espera hasta que una casilla se ha pulsado
     * @param r: fila de la casilla pulsada
     * @param c: columna de la casilla pulsada
    */
    void getCasillaElegida(int &r,int &c);

    /**
     * @brief Pone en la casilla una imagen
     * @param r: fila de la casilla
     * @param c: columna de la casilla
     * @param nameimg: nombre del fichero en disco con la imagen
     */
      void putImagen(int r,int c,const char * nameimg);


    /**
     * @brief Rellena toda la reticula con blancos
     * @param name: nombre del fichero en disco de la imagen blanco
     * @pre en disco debe existir una imagen con nombre ./images/B.png
     */
    void PonerEnBlanco(const char *name);

    /**
     * @brief Obtiene el numero de filas
     */
    int getNrows()const;

    /**
     * @brief Obtiene el numero de columnas
     */
    int getNcols()const;


public slots:

    /**
     * @brief Gestiona el doble click sobre una casilla
     * @param event: evento ocurrido
     */
    void mouseDoubleClickEvent(QMouseEvent *event);

    /**
     * @brief Captura la posicion de la casilla pulsada
     */
    void pulsaCasilla();


signals:
    /**
     * @brief Señal que se emite cuando se pulsa una casilla
     */
    void btnRightClicked();

};



#endif // TABLEROGRAFICO_H
