#include "consola.h"
#include <iostream>
Consola::Consola(QWidget *parent) : QPlainTextEdit(parent),
    userPrompt(QString("> ")),
    locked(false)

{

    
    //el texto no es ajustado
    setLineWrapMode(NoWrap);
    //insertamos texto en la actual posicion del cursor
    insertPlainText(userPrompt);
    connect(this, SIGNAL(command(QString)), this, SLOT(result(QString)));

}

/****************************************************************/



void Consola::keyPressEvent(QKeyEvent *e) {
    // si locked es true no se considera ninguna entrada

    if(locked) return;

    switch(e->key()) {
    case Qt::Key_Return: // enter
        handleEnter();
        break;
    case Qt::Key_Backspace:
        handlerDel(e);
        break;
    case Qt::Key_Up:
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    case Qt::Key_Down:
        break;
    default:
        QPlainTextEdit::keyPressEvent(e);
        break;
    }
}
/****************************************************************/
// Enter
void Consola::handleEnter() {
    
    QString cmd = getCommand();

    moveToEndOfLine();

    if(cmd.length() > 0) {
        //locked = true;
        setFocus();
        insertPlainText("\n");
        emit command(cmd);
    } else {
        //insertPlainText("\n");
        //insertPlainText(userPrompt);
        //asegura que el cursor es visible añadiendo un scroll si fuese necesario
        ensureCursorVisible();
    }
}

/****************************************************************/
void Consola::result(QString result) {
    entrada = result;
    //asegura que el cursor es visible añadienso un scroll si fuese necesario
    ensureCursorVisible();
    locked = true;
}
/****************************************************************/
void Consola::handlerDel(QKeyEvent *e){
    QTextCursor cur = this->textCursor();
    cur.select(QTextCursor::LineUnderCursor);
    QString text = cur.selectedText();
    if (text==userPrompt) return;
    //text.remove(text.length()-1,1)
    QPlainTextEdit::keyPressEvent(e);
    
}
/****************************************************************/
//borra lo que haya antes del cursor
void Consola::clearLine() {
    QTextCursor c = this->textCursor();
    c.select(QTextCursor::LineUnderCursor);
    c.removeSelectedText();
    this->insertPlainText(userPrompt);
}

/****************************************************************/
// Devuelve la entrada del usuaio sin prompt

QString Consola::getCommand()  {
    QTextCursor c = this->textCursor();
    c.select(QTextCursor::LineUnderCursor);

    QString text = c.selectedText();
    text.remove(0, userPrompt.length());

    return text;
}
/****************************************************************/
void Consola::moveToEndOfLine() {
    QPlainTextEdit::moveCursor(QTextCursor::EndOfLine);
}
/****************************************************************/
void Consola::setPrompt(const std::string &str) {
    QString prompt =QString::fromStdString(str);
    userPrompt = prompt;
    clearLine();
}
/****************************************************************/
QString Consola::prompt() const {
    return userPrompt;
}

/****************************************************************/
void Consola::WriteText(const std::string & str){
    locked = true;
    QString txt =QString::fromStdString(str);
    clearLine();
    insertPlainText(txt);
    insertPlainText("\n");
    insertPlainText(userPrompt);
    //asegura que el cursor es visible añadienso un scroll si fuese necesario
    ensureCursorVisible();

}
/****************************************************************/
void Consola::WriteVector(const vector<string> &v){
     locked = true;
    clearLine();
    QString txt= QString::fromStdString("[");
    insertPlainText(txt);
    auto last_pos=v.end(); --last_pos;
    for (auto it =v.begin();it!=v.end();++it){
       if (it!=last_pos)
        txt = QString::fromStdString( (*it)+",");
       else 
        txt = QString::fromStdString( (*it));
       insertPlainText(txt);
    }
    
    txt = QString::fromStdString("]\n");
    insertPlainText(txt);
    insertPlainText(userPrompt);
    ensureCursorVisible();
}
/****************************************************************/
std::string Consola::ReadInput(){
    locked = true;
    //hilo que se ocupa de comprobar cuando el usuario ha introducido algo
    QThread *thread = QThread::create([this](){
        this->locked=false;
        while(this->locked==false);

    });
    thread->start();
    //bucle que espera hasta que hilo termine
    QEventLoop loop;
    connect(thread,&QThread::finished, &loop, &QEventLoop::quit);
    loop.exec();
    while( thread->isFinished()==false);
    //limpiamos
    clearLine();
    return entrada.toStdString();
}
/****************************************************************/
char  Consola::ReadChar(){
    std::string aux = this->ReadInput();
    if (aux.size()>0)
        return aux[0];
    else return ' ';
}

/****************************************************************/
std::string Consola::ReadString(){
    return  this->ReadInput();

}

/****************************************************************/
int Consola::ReadInt(){
    std::string aux = this->ReadInput();

    try{
       return stoi(aux);

    }
    catch(int e)
    {
        return  std::numeric_limits<int>::max();
    }

}
/****************************************************************/
float Consola::ReadFloat(){
    std::string aux = this->ReadInput();

    try{
        return stof(aux);

    }
    catch(int e)
    {
        return  std::numeric_limits<float>::max();
    }
}
/****************************************************************/
void Consola::cerrar(){
	close();
}	
