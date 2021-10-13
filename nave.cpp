#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <wincon.h>
#include <stdlib.h>
#include <list>
using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy(int x, int y){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;

    SetConsoleCursorPosition(hCon, dwPos);

}

void OcultarCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CURSOR;
    CURSOR.dwSize = 1;
    CURSOR.bVisible = FALSE; 

    SetConsoleCursorInfo(hCon, &CURSOR);

}

void pintarLimites(){
    for(int i=2; i<78;i++){
        gotoxy(i,3); printf("%c",205);
        gotoxy(i,33); printf("%c",205);
    }
    for(int i=4; i<33;i++){
        gotoxy(2,i); printf("%c",186);
        gotoxy(77,i); printf("%c",186);
    }
    gotoxy(2,3); printf("%c",201);
    gotoxy(2,33); printf("%c",200);
    gotoxy(77,3); printf("%c",187);
    gotoxy(77,33); printf("%c",188);

}

class NAVE{
    int x, y;
    int corazones;
    int vidas;
public:
    NAVE(int _x, int _y, int _corazones, int _vidas): x(_x), y(_y), corazones(_corazones), vidas (_vidas) {/* //pasando los datos al constructor */}
    int X(){return x;}//Las cordenadas de la nave son privadas entonces necesitamos pasarlas por este metodo
    int Y(){return y;}//Las cordenadas de la nave son privadas entonces necesitamos pasarlas por este metodo
    void pintar();
    void borrar();
    void mover();
    void pintar_corazones();
    void morir();
    void COR(){ corazones--; };
};

class AST{
    int x,y;
public:
    AST(int _x, int _y): x(_x), y(_y){/* //se pasan los datos al cosntructor */}
    void pintar();
    void mover();
    void choque(class NAVE &N);
};

class PROYECTIL{
    int x, y;
public:
    PROYECTIL(int _x, int _y): x(_x), y(_y){/* //Se pasan los datos al consturctor */}
    int X(){return x;}
    int Y(){return y;}
    void mover ();
    bool fuera();// funcion para verificar si el proyectil llego a los limites del entorno del video juego
};

void NAVE::pintar(){
    gotoxy(x,y);   printf("  %c",30);
    gotoxy(x,y+1); printf(" %c%c%c ",40,207,41);
    gotoxy(x,y+2); printf("%c%c%c %c",30,190,190,30);
}

void NAVE::borrar(){ 
    gotoxy(x,y);   printf("            ");
    gotoxy(x,y+1); printf("            ");
    gotoxy(x,y+2); printf("            ");
}

void NAVE::mover(){

    if(kbhit()){
            char tecla = getch();
            borrar();
            if(tecla == IZQUIERDA && x>3)x--;
            if(tecla == DERECHA && x+6 < 77)x++;
            if(tecla == ARRIBA && y>4 )y--;
            if(tecla == ABAJO && y+3 < 33)y++;
            if(tecla == 'e') corazones--;//testeo de disminuicion de corazones.
            pintar();
            OcultarCursor();
            pintar_corazones();
        }

}

void NAVE::pintar_corazones(){
    gotoxy(50,2); printf("Vidas: %d",vidas);
    gotoxy(64,2); printf("Salud:");
    gotoxy(70,2); printf("      ");
    for(int i=0;i<corazones; i++){
        

        gotoxy(70+i,2); printf("%c",3);
    }
}

void NAVE::morir(){
    if(corazones == 0){
        borrar();
        gotoxy(x,y);  printf("   **    ");
        gotoxy(x,y+1);printf(" ******  ");
        gotoxy(x,y+2);printf("   **    ");
        Sleep(200);

        borrar();
        gotoxy(x,y);  printf("*   **    *");
        gotoxy(x,y+1);printf("  *******  ");
        gotoxy(x,y+2);printf("*   **    *");
        Sleep(200);


        vidas --;
        corazones = 3;
        pintar_corazones();
        borrar();
        pintar();
        
        
    }
}

void AST::pintar(){
    gotoxy(x,y);printf("%c",184);
}

void AST::mover(){
    gotoxy(x,y); printf(" ");
    y++;
    if(y>32){
        x = rand() % 71 +4;
        y = 4;
    }
    pintar(); 
}

void AST::choque(class NAVE &N){
    if( x >= N.X() && x < N.X()+6 && y >= N.Y() && N.Y()+2)/* Tomamos el ancho y alto  de la nave pasados por parametro mediante la clase NAVE &N */{

        N.COR();//Llamamos al metodo que resta corazones en caso de que el asteroide choque con la nave 
        N.borrar();
        N.pintar();
        N.pintar_corazones();
        x = rand() % 71 +4;
        y = 4;

    }
}

void PROYECTIL::mover(){
    gotoxy(x,y); printf(" ");
    y--;
    gotoxy(x,y); printf("^");
}

bool PROYECTIL::fuera(){
    if(y == 4 ) return true;
    return false;
}
int main(){
    pintarLimites();
    OcultarCursor();
    NAVE N(37,30,3,3);
    N.pintar();
    N.pintar_corazones();

    list<AST*> A;
    list<AST*>::iterator itA;
    for(int i=0; i<5;i++){

        A.push_back(new AST(rand()%75 +3 , rand()%5+4));

    }
      
    

    list<PROYECTIL*> B; //Creamos lista con elementos de la clase bala pasados con punteros llamada B
    list<PROYECTIL*>::iterator it; // Creamos iterador para recorrer toda la lista B


    
    

    bool gameover = false ;

    while(!gameover){

        if(kbhit()){
            char tecla = getch();
            if(tecla == 'a')
            B.push_back(new PROYECTIL( N.X()+2 , N.Y()-1 ));
            
        }
        for(it = B.begin(); it != B.end() ; it ++){

            (*it)->mover();/* Desreferenciamos el puntero y movemos los proyectiles */
            if((*it)->fuera()){ /* Caso en que la bala llega al limite del videojuego */
                gotoxy((*it)->X(),(*it)->Y()); printf(" ");
                delete (*it);// Eliminamos un elemento de la lista con el operador 'delete'
                it = B.erase(it); // Toma el siguiente valor de la lista B para seguir verificando las operaciones del condicional.
            }

        }

        for(itA= A.begin(); itA != A.end(); itA++){
            (*itA)->mover();
            (*itA)->choque(N);
        }
        
        N.morir();
        N.mover();
        Sleep(35);
    }


    return  0;
}