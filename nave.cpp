#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <wincon.h>

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

class NAVE{
    int x, y;
public:
    NAVE(int _x, int _y);
    void pintar();
    void borrar();
    void mover();
};

NAVE::NAVE(int _x, int _y): x(_x), y(_y){
    //Pasando los datos al constructor
}

void NAVE::pintar(){
    gotoxy(x,y);   printf("  %c",30);
    gotoxy(x,y+1); printf(" %c%c%c ",40,207,41);
    gotoxy(x,y+2); printf("%c%c%c %c",30,190,190,30);
}

void NAVE::borrar(){
    gotoxy(x,y);   printf("     ");
    gotoxy(x,y+1); printf("     ");
    gotoxy(x,y+2); printf("     ");
}

void NAVE::mover(){

    if(kbhit()){
            char tecla = getch();
            borrar();
            if(tecla == IZQUIERDA)x--;
            if(tecla == DERECHA)x++;
            if(tecla == ARRIBA)y--;
            if(tecla == ABAJO)y++;
            pintar();
        }

}




int main(){
    OcultarCursor();
    NAVE N(7,7);

    N.pintar();
    
    bool gameover = false ;
    while(!gameover){

        N.mover();
             Sleep(10);
    }


    
    
    return  0;
}