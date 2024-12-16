/**
 * @ file consola.h. Definición del TDA Consola
 *
 */
#ifndef CONSOLA_H
#define CONSOLA_H
#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QTextCursor>
#include <string>
#include <QEventLoop>
#include <QThread>
#include <vector>
using namespace std;


/**
 * @brief The Consola class. Un objeto de tipo Consola permite mostrar texto y leer
 *  texto desde una ventana grafica.
 * Author: Rosa Mª Rodríguez Sánchez
 * Date : 31/07/2024
 */
class Consola : public QPlainTextEdit
{
    Q_OBJECT

protected:
    /** @brief
     *  Se filtran todo los eventos del teclado. De forma que se tratan
     *  con métodos dentro de la clase.
     *  param e: puntero al evento de tipo tecla
     */
    void keyPressEvent(QKeyEvent *e);

    // NO se admite ninguno de los siguiente eventos


    void mousePressEvent(QMouseEvent *)       { /* Ignore */ }
    void mouseDoubleClickEvent(QMouseEvent *) { /* Ignore */ }
    void mouseMoveEvent(QMouseEvent *)        { /* Ignore */ }
    void mouseReleaseEvent(QMouseEvent *)     { /* Ignore */ }

private:
    /**
     * @brief Procesa el texto cuando el usuario introduce enter
     */
    void handleEnter();
    /**
     * @brief Procesa el texto cuando el usuario introduce del
     */
    void handlerDel(QKeyEvent *e);

    /**
     * @brief El cursor se situa al final de la linea
     */
    void moveToEndOfLine();

    /**
     * @brief Se borra toda la linea
     */
    void clearLine();

    /**
     * @brief Devuelve el texto introducido
     * @return cadena con el texto
     */
    QString getCommand();

    /**
     * @brief Lee la entrada insertada por el usuario
     * @return devuelve la entrada capturada en la consola.
     */
    std::string ReadInput();

    QString userPrompt;//texto antes del cursor en una linea vacia
    bool locked; //estado para permitir o denegar la entrada
    QString entrada; //entrada capturada


signals:
    /**
     * @brief Señal que se emite cuando se inserta un comando
     * @param command: entrada capturada
     */
    void command(QString command);



private slots:
    /**
     * @brief Actualiza la entrada capturada
     * @param result:  entrada capturada.
     */
    void result(QString result);


public:
    /**
     * @brief Constructor
     * @param parent: objeto grafico padre
     */
    Consola(QWidget *parent = 0);

    /**
     * @brief Devuelve el prompt
     */
    QString prompt() const;

    /**
     * @brief Modifica el prompt
     * @param str: nuevo prompt
     */
    void setPrompt(const std::string &str);

    /**
     * @brief Escribe un texto en la consola
     * @param str: texto a escribir
     */
    void WriteText(const std::string & str);

    /**
     * @brief Escribe los elementos del vector en la consola
     * @param v: array con los cadenas
     */
    void WriteVector(const vector<string> &v);
    /**
     * @brief Lee un caracter introducido por el usuario en  la consola
     * @return el caracter leido
     */
    char ReadChar();

    /**
     * @brief Lee una cadena de caracteres introducida por el usuario en  la consola
     * @return la cadena de caracteres leida
     */

    std::string ReadString();

    /**
     * @brief Lee un entero  introducido por el usuario en  la consola
     * @return el entero leido
     */
    int ReadInt();

    /**
     * @brief Lee un numero real  introducido por el usuario en  la consola
     * @return el numero real
     */
    float ReadFloat();
    
    
    /**
     * @brief Cierra la ventana grafica

     */
    void cerrar();
};




#endif // CONSOLA_H
