#include "../include/quienesquien.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <algorithm>
#include <fstream>

QuienEsQuien::QuienEsQuien(){
}

QuienEsQuien::QuienEsQuien(const QuienEsQuien &quienEsQuien){
}

QuienEsQuien& QuienEsQuien::operator= (const QuienEsQuien &quienEsQuien){
     return *this;
}

QuienEsQuien::~QuienEsQuien(){
     this->limpiar() ;
}

void QuienEsQuien::limpiar(){
     this->personajes.clear();
     this->atributos.clear();
     this->personajes_images.clear();
     for(vector<vector<bool>>::iterator it_tablero = this->tablero.begin();
         it_tablero != this->tablero.end();
         it_tablero++){
          (*it_tablero).clear();
     }
     this->tablero.clear();
     this->arbol.clear() ;
}

template <typename T>
std::ostream& operator<<  (std::ostream& out, const std::vector<T>& v) {
     if ( !v.empty() ) {
          out <<  '[' ;
          std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
          out <<  "\b\b]";
     }

     return out;
}

void QuienEsQuien::mostrar_estructuras_leidas(){
     cout <<  "personajes: " <<  (this->personajes) <<  endl;
     cout <<  "atributos: " <<  (this->atributos) <<  endl;
     cout <<  "tablero:    " <<  endl;
     // Escribe la cabecera del tablero
     for(vector<string>::iterator it_atributos = this->atributos.begin() ;
         it_atributos != this->atributos.end() ;
         it_atributos++){
          cout <<  *it_atributos << "\t";
     }
     cout << endl;

     int indice_personaje = 0;
     for(vector<vector<bool>>::iterator it_tablero_atributos = tablero.begin();
         it_tablero_atributos!= tablero.end();
         it_tablero_atributos++){
          string personaje = this->personajes[indice_personaje];
          int indice_atributo = 0;
          for(vector<bool>::iterator it_atributos_personaje = (*it_tablero_atributos).begin();
              it_atributos_personaje != (*it_tablero_atributos).end() ;
              it_atributos_personaje++){
               cout <<  *it_atributos_personaje<< "\t";
               indice_atributo++;
          }
          cout <<  personaje <<  endl;
          indice_personaje++;
     }
}

/**
  * @brief Devuelve una copia de la cadena original sin las subcadenas no deseadas.
  *
  * @param cadena_original Cadena de la que se eliminan las subcadenas no deseadas.
  * @param cadena_a_eliminar Subcadena que se busca y se elimina.
  *
  * @return Copia de la cadena original sin las subcadenas no deseadas.
  */


string limpiar_string(string cadena_original,string cadena_a_eliminar){
     string linea(cadena_original);
     while(linea.find_first_of(cadena_a_eliminar) != std::string::npos){
          linea.erase(linea.find_first_of(cadena_a_eliminar),cadena_a_eliminar.length());
     }

     return linea;
}

istream& operator >>  (istream& is, QuienEsQuien &quienEsQuien) {
     quienEsQuien.limpiar();
     if(is.good()){ //Lee la linea con los nombres de las columnas
          string linea;
          getline(is, linea, '\n');
          linea = limpiar_string(linea,"\r");
          bool salir=false;
          while(!salir && linea.find('\t') != string::npos ){
               string atributo = linea.substr(0,linea.find('\t'));
               quienEsQuien.atributos.push_back(atributo);
               linea = linea.erase(0,linea.find('\t')+1);
               if (linea.substr(0,linea.find('\t'))=="Nombre personaje")
                   salir=true;
          }
     }
     //LEE los atributos para cada personaje
     while( is.good()) {
          string linea;
          getline(is, linea, '\n');
          linea = limpiar_string(linea,"\r");

          //Si la linea contiene algo extrae el personaje.
          //Si no la ignora.
          if(linea != ""){;
               vector<bool> atributos_personaje;
               int indice_atributo=0;
               bool salir=false;
               while(!salir && linea.find('\t') != string::npos){
                   string valor = linea.substr(0,linea.find('\t'));
                   if (valor == "0" || valor == "1"){
                    bool valor_atributo = valor == "1";
                    atributos_personaje.push_back(valor_atributo);
                    linea = linea.erase(0,linea.find('\t')+1) ;
                    indice_atributo++;
                   }
                   else{
                       salir=true;
                   }
               }
               string nombre_personaje;
               string imagen_personaje;
               if (linea.find('\t') == string::npos){
                nombre_personaje = linea;
                imagen_personaje=""; //no tiene imagen
               }
               else{
                nombre_personaje=linea.substr(0,linea.find('\t'));
                linea = linea.erase(0,linea.find('\t')+1) ;
                imagen_personaje=linea;

               }
               quienEsQuien.personajes.push_back(nombre_personaje);
               quienEsQuien.tablero.push_back(atributos_personaje);
               quienEsQuien.personajes_images.push_back(imagen_personaje);
          }
     }

     quienEsQuien.mostrar_estructuras_leidas();
     quienEsQuien.arbol = quienEsQuien.crear_arbol();
     quienEsQuien.tg=nullptr; //puntero al tablero grafico
     quienEsQuien.con=nullptr; //puntero a la consola
     if (quienEsQuien.modo_graph){
        int nper = quienEsQuien.personajes.size();
        int nrows,ncols=3;
        if (nper<8)
           nrows = nper/ncols;
        else{
            ncols=4;
            nrows = nper/ncols;
        }
        //se crea un tablero grafico
        quienEsQuien.tg = new TableroGrafico(400,400,nrows,ncols);
        //ponemos en el tablero grafico las im�genes
        for (int i=0;i<nper;i++){

            quienEsQuien.tg->putImagen(i/ncols,i%ncols,quienEsQuien.personajes_images[i].c_str());
        }
        //creamos la consola
        quienEsQuien.con=new Consola();
     }
     return is;
}

ostream& operator << (ostream& os, const QuienEsQuien &quienEsQuien){
     //Escribimos la cabecera, que contiene los atributos y al final una columna para el nombre

     cout<<"NUmero de Atributos "<<quienEsQuien.atributos.size()<<endl;
     for(vector<string>::const_iterator it_atributos = quienEsQuien.atributos.begin();
         it_atributos != quienEsQuien.atributos.end();
         it_atributos++){
          os << *it_atributos << "\t";
     }

     os << "Nombre personaje" << endl;
     
     //Rellenamos con ceros y unos cada linea y al final ponemos el nombre del personaje.
     for(int indice_personaje=0;indice_personaje<quienEsQuien.personajes.size();indice_personaje++){
          for(int indice_atributo=0;indice_atributo<quienEsQuien.atributos.size();indice_atributo++){
               os << quienEsQuien.tablero[indice_personaje][indice_atributo] << "\t";
          }
          os << quienEsQuien.personajes[indice_personaje] << endl;
     }
     
     return os;
}

/**
* @brief Convierte un numero a un vector de bool que corresponde
*        con su representadon en binario con un numero de digitos
*              f i j o.
*
* @param n Numero a convertir en binario.
* @param digitos Numero de digitos de la representaddn binaria.
*
* @return Vector de booleanos con la representadon en binario de @e n
*      con el numero de elementos especificado por @e digitos.
*/
vector<bool> convertir_a_vector_bool(int n, int digitos) {
     vector<bool> ret;
     while(n) {
          if (n&1)
               ret.push_back(true);
          else
               ret.push_back(false);

          n>>=1;
     }

     while(ret.size()<digitos)
          ret.push_back(false);

     reverse(ret.begin(),ret.end());
     return ret;
}

/**
* @brief Esta es una propuesta de cabecera de la funcion para crear el arbol.
*/
//TODO :D:D
bintree<Pregunta> QuienEsQuien::crear_arbol( vector<string> atributos,
                                             int indice_atributo,
                                             vector<string> personajes,
                                             vector<bool> personajes_restantes,
                                             vector<vector<bool>> tablero){
     //para el caso que solo quede un personaje
     int num_restantes = std::count(personajes_restantes.begin(), personajes_restantes.end(), true);  
     if (num_restantes == 1) {
        int idx = std::distance(personajes_restantes.begin(),
                                std::find(personajes_restantes.begin(), personajes_restantes.end(), true));
        Pregunta hoja(personajes[idx], 1);
        return bintree<Pregunta>(hoja);
    }

    //calculamos la entropia de cada atributo
      // Usaremos una cola para almacenar las entropías de cada atributo
    std::queue<std::pair<double, int>> entropias;

    for (size_t i = 0; i < atributos.size(); ++i) {
        int si_count = 0, no_count = 0;

        // Recorremos personajes restantes para contar respuestas sí/no
        for (size_t j = 0; j < personajes_restantes.size(); ++j) {
            if (personajes_restantes[j]) {
                if (tablero[j][i]) si_count++;
                else no_count++;
            }
        }

        // Calculamos las probabilidades
        double p_si = static_cast<double>(si_count) / num_restantes;
        double p_no = static_cast<double>(no_count) / num_restantes;
        double entropia = 0.0;

        // Calculamos la entropía
        if (p_si > 0) entropia -= p_si * log2(p_si);
        if (p_no > 0) entropia -= p_no * log2(p_no);

        // Guardamos la entropía y el índice del atributo en la cola
        entropias.push({entropia, i});
    }

    // Elegimos el atributo con mayor entropía
    double max_entropia = -1.0;
    int mejor_atributo = -1;

    while (!entropias.empty()) {
        auto [entropia, atributo] = entropias.front();
        entropias.pop();
        if (entropia > max_entropia) {
            max_entropia = entropia;
            mejor_atributo = atributo;
        }
    }

    // Dividimos personajes en subconjuntos según la mejor pregunta
    vector<bool> personajes_si(personajes_restantes.size(), false);
    vector<bool> personajes_no(personajes_restantes.size(), false);

    for (size_t j = 0; j < personajes_restantes.size(); ++j) {
        if (personajes_restantes[j]) {
            if (tablero[j][mejor_atributo]) personajes_si[j] = true;
            else personajes_no[j] = true;
        }
    }

    // Creamos el nodo para la mejor pregunta
    Pregunta nodo(atributos[mejor_atributo], num_restantes);
    bintree<Pregunta> arbol(nodo);

    // Construimos subárboles recursivamente
    auto subarbol_si = crear_arbol(atributos, indice_atributo + 1, personajes, personajes_si, tablero);
    auto subarbol_no = crear_arbol(atributos, indice_atributo + 1, personajes, personajes_no, tablero);

    // Insertamos los subárboles en el nodo actual
    arbol.insert_left(arbol.root(), subarbol_si);
    arbol.insert_right(arbol.root(), subarbol_no);

    return arbol;
    
}

bintree<Pregunta> QuienEsQuien::crear_arbol(){
     int indice_atributo = 0;
     vector<bool> personajes_restantes;
     for(vector<string>::iterator it = personajes.begin();it!=personajes.end();it++){
          personajes_restantes.push_back(true);
     }

     return crear_arbol( this->atributos, indice_atributo, this->personajes,
                         personajes_restantes, this->tablero);
}

void QuienEsQuien::usar_arbol(bintree<Pregunta> arbol_nuevo){
     arbol = arbol_nuevo;
}

void QuienEsQuien::iniciar_juego() {
    Ventana v(tg, con, "WisW");
    if (modo_graph) {
        v.show();
    }

    char jugar_otra_vez;
    do {
        jugada_actual = arbol.root();

        while (!jugada_actual.null() && (*jugada_actual).es_pregunta()) {
            try {
                std::string pregunta = "¿" + (*jugada_actual).obtener_pregunta() + "? (si/no): ";
                if (modo_graph) {
                    con->WriteText(pregunta);
                } else {
                    std::cout << pregunta;
                }

                std::string respuesta;
                if (modo_graph) {
                    respuesta = con->ReadString();
                } else {
                    std::cin >> respuesta;
                }

                if (respuesta == "si") {
                    jugada_actual = jugada_actual.left();
                } else if (respuesta == "no") {
                    jugada_actual = jugada_actual.right();
                } else {
                    if (modo_graph) {
                        con->WriteText("Respuesta inválida. Responde 'si' o 'no'.\n");
                    } else {
                        std::cout << "Respuesta inválida. Responde 'si' o 'no'.\n";
                    }
                    continue; // Volver a pedir la respuesta si no es válida
                }

                std::string descripcion = preguntas_formuladas(jugada_actual);
                if (modo_graph) {
                    con->WriteText("El personaje oculto tiene las siguientes características:\n" + descripcion + "pero aún no sé cuál es.\n");
                } else {
                    std::cout << "El personaje oculto tiene las siguientes características:\n" << descripcion;
                    std::cout << "pero aún no sé cuál es." << std::endl;
                }

                // Depuración adicional
                cout << "Depuración: Pregunta actual = " << (*jugada_actual).obtener_pregunta() << endl;
                cout << "Depuración: Nodo actual es pregunta = " << (*jugada_actual).es_pregunta() << endl;

            } catch (const std::runtime_error& e) {
                // Capturar la excepción y manejar el caso de adivinanza del personaje
                std::cout << "Error: " << e.what() << std::endl;
                std::cout << "Parece que solo queda un personaje. Intentando adivinar...\n";
                break;
            }
        }

        if (!jugada_actual.null() && (*jugada_actual).es_personaje()) {
            std::string resultado = "¡Ya lo sé! Tu personaje es " + (*jugada_actual).obtener_personaje() + ".\n";
            if (modo_graph) {
                con->WriteText(resultado);
            } else {
                std::cout << resultado;
            }

            // Depuración adicional
            cout << "Depuración: Se ha adivinado el personaje " << (*jugada_actual).obtener_personaje() << endl;
        }

        if (modo_graph) {
            con->WriteText("¿Quieres jugar otra vez? (s/n): ");
            jugar_otra_vez = con->ReadChar();
        } else {
            std::cout << "¿Quieres jugar otra vez? (s/n): ";
            std::cin >> jugar_otra_vez;
        }

    } while (jugar_otra_vez == 's');

    if (modo_graph) {
        v.cerrar();
    }
}

set<string> QuienEsQuien::informacion_jugada(bintree<Pregunta>::node jugada_actual){
     set<string> personajes_levantados;

    if (jugada_actual.null()) {
        return personajes_levantados;
    }

    if ((*jugada_actual).es_personaje()) {
        personajes_levantados.insert((*jugada_actual).obtener_personaje());
        return personajes_levantados;
    }

    auto personajes_izquierda = informacion_jugada(jugada_actual.left());
    auto personajes_derecha = informacion_jugada(jugada_actual.right());

    personajes_levantados.insert(personajes_izquierda.begin(), personajes_izquierda.end());
    personajes_levantados.insert(personajes_derecha.begin(), personajes_derecha.end());

    return personajes_levantados;
}

void escribir_esquema_arbol(ostream& ss, const bintree <Pregunta>& a,
                            bintree<Pregunta>::node n, string& pre){
     if (n.null())
          ss <<  pre <<  "-- x" <<  endl;
     else {
          ss <<  pre <<  "-- " <<  (*n) <<  endl;
          if ( !n.right().null() || !n.left().null()) {// Si no es una hoja
               pre += "   |";
               escribir_esquema_arbol(ss,a, n.right(), pre);
               pre.replace(pre.size()-4, 4, "    ");
               escribir_esquema_arbol (ss,a, n.left(), pre);
               pre.erase(pre.size()-4, 4);
          }
     }
}

void QuienEsQuien::escribir_arbol_completo() const{
     string pre = "";
     escribir_esquema_arbol(cout,this->arbol,this->arbol.root(),pre);
}


void QuienEsQuien::eliminar_nodos_redundantes() {
    std::function<void(bintree<Pregunta>::node)> simplificar = [&](bintree<Pregunta>::node n) {
        if (n.null() || (n.left().null() && n.right().null())) { 
            return;
        }

        auto left_child = n.left();
        auto right_child = n.right();

        // Simplificar los subárboles recursivamente
        simplificar(left_child);
        simplificar(right_child);

        // Verificar si el nodo actual es redundante
        if (!left_child.null() && right_child.null()) {
            // Nodo tiene solo hijo izquierdo
            arbol.replace_subtree(n, arbol, left_child);
            left_child.remove();
        } else if (!right_child.null() && left_child.null()) {
            // Nodo tiene solo hijo derecho
            arbol.replace_subtree(n, arbol, right_child);
            right_child.remove();
        }
    };

    simplificar(arbol.root());
}

float QuienEsQuien::profundidad_promedio_hojas() {
    int total_hojas = 0;
    int suma_profundidades = 0;

    std::function<void(bintree<Pregunta>::node, int)> calcular_profundidades = [&](bintree<Pregunta>::node n, int profundidad) {
        if (n.null()) {
            return;
        }

        if (n.left().null() && n.right().null()) { 
            total_hojas++;
            suma_profundidades += profundidad;
            return;
        }

        calcular_profundidades(n.left(), profundidad + 1);
        calcular_profundidades(n.right(), profundidad + 1);
    };

    calcular_profundidades(arbol.root(), 0);

    if (total_hojas == 0) {
        return 0.0; // Evitar división por cero
    }

    return static_cast<float>(suma_profundidades) / total_hojas;
}


/**
* @brief Genera numero enteros positivos aleatorios en el rango [min,max].
**/
int generaEntero(int min, int max){
     int tam= max-min;
     return ((rand( )%tam)+min) ;
}

void QuienEsQuien::tablero_aleatorio(int numero_de_personajes){
     srand(0);
     this->limpiar();
     float log_2_numero_de_personajes = log(numero_de_personajes)/log(2);
     int numero_de_atributos = ceil(log_2_numero_de_personajes);

     cout <<  "Peticion para generar " <<  numero_de_personajes << " personajes ";
     cout <<  "con " << numero_de_atributos <<  " atributos" << endl;
     cout <<  "Paso 1: generar " <<  pow(2, numero_de_atributos) <<  " personajes" << endl;

     //Fase 1: completar el tablero con todos los personajes posibles
     //Complete el tablero y los nombres de personajes a la vez
     for(int indice_personaje=0;indice_personaje< pow(2,numero_de_atributos);indice_personaje++){
          vector<bool> atributos_personaje =
                                    convertir_a_vector_bool(indice_personaje,numero_de_atributos);
          string nombre_personaje = "Personaje_"+to_string(indice_personaje);

          this->personajes.push_back(nombre_personaje);
          this->tablero.push_back(atributos_personaje);
     }

     // Completo los nombres de los atributos.
     for( int indice_atributo=0;indice_atributo<numero_de_atributos;indice_atributo++){
          string nombre_atributo = "Atributo_"+to_string(indice_atributo);
          this->atributos.push_back(nombre_atributo);
     }
     cout <<  "Paso 2: eliminar " << (pow(2,numero_de_atributos)-numero_de_personajes) <<  "personajes" << endl;

     //Fase 2. Borrar personajes aleatoriamente hasta que quedan solo los que hemos pedido.
     while(personajes.size()>numero_de_personajes){
          int personaje_a_eliminar = generaEntero(0,personajes.size());
          personajes.erase(personajes.begin()+personaje_a_eliminar);
          tablero.erase(tablero.begin()+personaje_a_eliminar);
     }
}
void QuienEsQuien::ocultar_personajes_graph(const set<string> &personajes_activos){
    //ocultamos los personajes
    int idx=0;
    int ncols=tg->getNcols();
    for (auto it=personajes.begin();it!=personajes.end();++it,idx++){
        if ( personajes_activos.find(*it)==personajes_activos.end())
            tg->putImagen(idx/ncols,idx%ncols,imagen_ocultar.c_str());
    }
}


void QuienEsQuien::setImagenOcultar(const char * n){
    imagen_ocultar=n;
}

void QuienEsQuien::setModoGraph(bool m){
    modo_graph=m;
}

std::string QuienEsQuien::preguntas_formuladas(bintree<Pregunta>::node jugada) {
    std::ostringstream descripcion;
    auto root = arbol.root(); 

    while (jugada != root) {

        std::string pregunta = (*jugada).obtener_pregunta();
        
        std::string respuesta = (jugada.parent().left() == jugada) ? "No" : "Sí";
        
        descripcion << pregunta << ": " << respuesta << "\n";
        
        jugada = jugada.parent();
    }
    
    return descripcion.str();
}

void QuienEsQuien::aniade_personaje(std::string nombre, std::vector<bool> caracteristicas, std::string nombre_imagen_personaje) {
    cout << "Inicio de aniade_personaje" << endl;

    // Verificar que el número de características coincide con el número de atributos
    if (caracteristicas.size() != atributos.size()) {
        cerr << "Error: Número de características proporcionadas no coincide con el número de atributos" << endl;
        return;
    }

    // Crear un nodo para el nuevo personaje
    Pregunta nuevo_personaje(nombre, 1);
    bintree<Pregunta> arbol_nuevo_personaje(nuevo_personaje);

    // Iniciar en la raíz del árbol
    bintree<Pregunta>::node nodo_actual = arbol.root();
    bintree<Pregunta>::node nodo_padre = nodo_actual;
    bool lado_izquierdo = false;

    // Recorrer el árbol hasta encontrar una posición para el nuevo personaje
    for (size_t i = 0; i < caracteristicas.size(); ++i) {
        nodo_padre = nodo_actual;
        lado_izquierdo = caracteristicas[i];

        if (caracteristicas[i]) {
            if (!nodo_actual.left().null()) {
                nodo_actual = nodo_actual.left();
            } else {
                break;
            }
        } else {
            if (!nodo_actual.right().null()) {
                nodo_actual = nodo_actual.right();
            } else {
                break;
            }
        }
    }

    cout << "Después de recorrer el árbol" << endl;

    // Aquí pedimos al usuario la nueva pregunta diferenciadora y su respuesta
    string nueva_pregunta_texto;
    bool respuesta_para_nuevo_personaje;

    cout << "Introduce una nueva pregunta que diferencie a " << nombre << " del personaje existente: ";
    cin.ignore();
    getline(cin, nueva_pregunta_texto);

    cout << "¿La respuesta a esa pregunta para " << nombre << " es sí o no? (1: sí, 0: no): ";
    cin >> respuesta_para_nuevo_personaje;

    // Crear una nueva pregunta para diferenciar el personaje nuevo del existente
    Pregunta nueva_pregunta(nueva_pregunta_texto, 2);
    bintree<Pregunta> arbol_nueva_pregunta(nueva_pregunta);

    // Insertar nuevo personaje y pregunta en subárbol
    if (respuesta_para_nuevo_personaje) {
        cout << "Insertando nuevo personaje a la izquierda del nodo actual" << endl;
        arbol_nueva_pregunta.insert_left(arbol_nueva_pregunta.root(), arbol_nuevo_personaje);
        arbol_nueva_pregunta.insert_right(arbol_nueva_pregunta.root(), *nodo_actual);
    } else {
        cout << "Insertando nuevo personaje a la derecha del nodo actual" << endl;
        arbol_nueva_pregunta.insert_left(arbol_nueva_pregunta.root(), *nodo_actual);
        arbol_nueva_pregunta.insert_right(arbol_nueva_pregunta.root(), arbol_nuevo_personaje);
    }

    cout << "Vinculando el subárbol al árbol principal" << endl;

    // Reemplazar el nodo hoja anterior por la nueva pregunta
    if (!nodo_padre.null()) {
        if (lado_izquierdo) {
            cout << "Reemplazando el nodo izquierdo del nodo padre" << endl;
            arbol.insert_left(nodo_padre, arbol_nueva_pregunta);
        } else {
            cout << "Reemplazando el nodo derecho del nodo padre" << endl;
            arbol.insert_right(nodo_padre, arbol_nueva_pregunta);
        }
    } else {
        cerr << "Error: Nodo padre es nulo." << endl;
        return;
    }

    cout << "Después de vincular el subárbol" << endl;

    // Añadir el nombre de la imagen del nuevo personaje a personajes_images
    if (!nombre_imagen_personaje.empty()) {
        string ruta_imagen = "datos/caras_graph/" + nombre_imagen_personaje;
        ifstream f(ruta_imagen.c_str());
        if (!f) {
            cerr << "Error: La imagen " << ruta_imagen << " no existe." << endl;
            return;
        }
        personajes_images.push_back(nombre_imagen_personaje);
    } else {
        personajes_images.push_back("");
    }

    // Añadir el nuevo personaje al vector de personajes
    personajes.push_back(nombre);

    // Incrementar el contador de personajes
    cout << "Depuración: num_personajes = " << personajes.size() << endl;

    cout << "Personaje " << nombre << " añadido exitosamente al árbol." << endl;
    cout << "Fin de aniade_personaje" << endl;
}


void QuienEsQuien::interactuar_agregar_personaje() {
    string nombre;
    cout << "Introduce el nombre del personaje: ";
    cin >> nombre;

    vector<bool> caracteristicas;
    bool caracteristica;
    for (const auto& atributo : atributos) {
        cout << "¿Tiene el atributo " << atributo << "? (1: sí, 0: no): ";
        cin >> caracteristica;
        caracteristicas.push_back(caracteristica);
    }

    string imagen_personaje;
    cout << "Introduce el nombre del archivo de imagen del personaje (debe estar en datos/caras_graph/): ";
    cin >> imagen_personaje;

    aniade_personaje(nombre, caracteristicas, imagen_personaje);
}













