#include "TXLib.h"
#include <cstdlib>
#include <ctime>

const int SIZEX = 800, SIZEY = 600;
const int BUTTONWIDTH = 130, BUTTONHEIGHT = 40;
enum BUTTONSNAME { BUBBLESORT, SELECTIONSORT, INSERTIONSORT };

struct Button {
    int posX_, posY_;
    COLORREF color_;
    char* buttonName_;
    bool CheckCoords ( int x, int y ) const ;
    void DrawButton ();
};

struct Elem {
    int elCount_, comp_, swap_;
    Elem () { elCount_ = 0; comp_ = 0; swap_ = 0; }
};

int PressButton ( void );
void BackGround ( void );
std::pair < int, int > BubbleSort ( int [], int arraySize );
void RandomFill ( int [], int arraySize );
void Swap ( int* a, int* b ) { int temp = *a; *a = *b; *b = temp; }
void OutputArray ( int [], int arraySize );
void DrawGraphics ( Elem [], int arraySize, COLORREF color );

int main()
    {
    const int buttonsCount = 3;
    srand ( time(NULL) );
    txCreateWindow (SIZEX, SIZEY);
    Button Buttons[buttonsCount] = { { 100, 50, TX_BLUE,  "Bubble_Sort\n" },
                                     { 300, 50, TX_GREEN, "Selection_Sort\n" },
                                     { 500, 50, TX_RED,   "Insertion_Sort\n" }};

    for ( int i = 0; i < buttonsCount; i++ ) {
        Buttons[i].DrawButton ();
    }
    const int ArraySize = 200,
              Step = 10;
    int MyArray [ArraySize] = {};
    Elem SwapCompCount [ArraySize / Step] = {};
    int PosAr = 0;

    for ( int len = 2; len < ArraySize; len += Step) {
        RandomFill ( MyArray, ArraySize );
        std::pair < int, int > Count = BubbleSort ( MyArray, len ); // elCount_, comp_, swap
        SwapCompCount[PosAr].elCount_ = len;
        SwapCompCount[PosAr].comp_ = Count.first;
        SwapCompCount[PosAr].comp_ = Count.second;
        printf ( "Elem count = %d, Comp count = %d, swap count = %d\n", len, SwapCompCount[PosAr].comp_, SwapCompCount[PosAr].swap_);
        PosAr ++;
    }
    DrawGraphics ( SwapCompCount, ArraySize / Step, TX_RED );
    BackGround ();
    /*
    RandomFill ( MyArray, ArraySize );
    OutputArray ( MyArray, ArraySize );
    BubbleSort ( MyArray, ArraySize );
    OutputArray ( MyArray, ArraySize );
    */
    while ( !txGetAsyncKeyState (VK_ESCAPE) ) {
        if ( txMouseButtons() & 1 ) {
            int i = 0;
            while ( i < buttonsCount ) {
                if ( Buttons[i].CheckCoords( txMouseX(), txMouseY() ) ) { std::cout << "TRUE"; }
                i ++;
            }
        }
        txSleep ();
    }
    return 0;
    }

//-----------------------------------------------------------------------------

bool Button::CheckCoords ( int x, int y ) const {
    return x >= posX_ && x <= ( posX_ + BUTTONWIDTH ) && y >= posY_ && y <= ( posY_ + BUTTONHEIGHT );
}

//-----------------------------------------------------------------------------

void Button::DrawButton () {
    txSetColor ( color_ );
    txSetFillColor ( color_ );
    txRectangle ( posX_, posY_, posX_ + BUTTONWIDTH, posY_ + BUTTONHEIGHT );
    txSetColor ( TX_WHITE );
    //txDrawText  ( posX_ + 2, posY_ + 2, posX_ + BUTTONWIDTH - 2, posY_ + BUTTONHEIGHT - 2, buttonName_ );
    txTextOut ( posX_ + 5, posY_ + 10, buttonName_ );
}

//-----------------------------------------------------------------------------

int PressButton ( void ) {
    if ( txGetAsyncKeyState ('0') ) return 0;
    if ( txGetAsyncKeyState ('1') ) return 1;
    if ( txGetAsyncKeyState ('2') ) return 2;
    return -1;
}

//-----------------------------------------------------------------------------

void BackGround ( void ) {
    txSetColor ( TX_WHITE );
    txSetFillColor ( TX_LIGHTBLUE );
    txRectangle ( 50, 150, SIZEX - 50, SIZEY - 50 ); // sizeY - 50 - 150
    txLine ( SIZEX / 2, SIZEY / 4, SIZEX / 2, SIZEY - 50 );
    POINT trian[3] = { { SIZEX - 10, SIZEY / 2 }, { SIZEX - 20, SIZEY / 2 + 20 }, { SIZEX - 20, SIZEY / 2 - 20 }};
    txPolygon ( trian, 3 );
}

//-----------------------------------------------------------------------------

std::pair < int, int > BubbleSort ( int Mas [], int arraySize ) {
    //Elem Count ();
    std::pair < int, int > Count = { 0, 0 };
    for ( int i = 0; i < arraySize; i++ ) {
        bool isSorted = true;
        for ( int j = 0; j < arraySize - i - 1; j++ ) {
            //Count.comp_ ++;
            Count.first ++;
            if ( Mas[j] > Mas[j + 1] ) {
                //Count.swap_ ++;
                Count.second ++;
                Swap ( &Mas[j], &Mas[j + 1] );
                isSorted = false;
            }
        }
        if ( isSorted ) break;
    }
    return Count;
}

//-----------------------------------------------------------------------------

void RandomFill ( int Mas [], int arraySize ){
    for ( int i = 0; i < arraySize; i++ ){
        Mas [i] = rand();
    }
}

//-----------------------------------------------------------------------------

void OutputArray ( int Mas [], int arraySize ) {
     for ( int i = 0; i < arraySize; i++ ) {
         printf ( "%d ", Mas[i] );
     }
     printf ("\n");
}

//-----------------------------------------------------------------------------

void DrawGraphics ( Elem Mas [], int arraySize, COLORREF color ) {
    txSetColor (color);
    txSetFillColor (color); //elCount_, comp_, swap_
    int MaxElement = Mas[arraySize - 1].comp_;
    printf (" --- %d ----", MaxElement);
    for ( int i = 0; i < arraySize; i++ ) {
        double xPos = Mas [i].elCount_, yPos = Mas [i].comp_ / MaxElement * ( SIZEY - 50 - 150 );
        //printf (" ---- %d ----\n", yPos);
        txCircle (SIZEX / 2 + xPos, SIZEY - 50 - yPos, 3); //SIZEY - 50 - 150
    }
}

//-----------------------------------------------------------------------------
