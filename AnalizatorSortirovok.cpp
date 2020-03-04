#include "TXLib.h"
#include <cstdlib>
#include <ctime>

const int SIZEX = 800,
          SIZEY = 600,
          LENX  = 350,
          LENY  = 400;

const int BUTTONWIDTH = 120,
          BUTTONHEIGHT = 40;

const int ARRAYSIZE = 100,
          STEP = 10;

enum BUTTONSNAME { BUBBLESORT, SELECTIONSORT, INSERTIONSORT };

struct Button {
    int posX_, posY_;
    COLORREF color_;
    char* buttonName_;
    bool CheckCoords ( int x, int y ) const ;
    void DrawButton ();
};

struct Coords {
    double centerX_, centerY_;
    double lenX_, lenY_;
    COLORREF color_;
    Coords ( double cX, double cY, double lx, double ly, COLORREF col_ = TX_WHITE ) {
        centerX_ = cX;
        centerY_ = cY;
        lenX_ = lx;
        lenY_ = ly;
        color_ = col_;
    }
    void DrawAxes () const {
        txSetFillColor ( TX_ORANGE );
        txLine ( centerX_ - 20, centerY_, centerX_ + lenX_, centerY_ );
        txLine ( centerX_, centerY_ + 20, centerX_, centerY_ - lenY_ );
    }
    void DrawCircle ( double x, double y, COLORREF cirColor) const {
        txSetColor ( cirColor );
        txSetFillColor ( cirColor );
        txCircle ( centerX_ + x, centerY_ - y, 3 );
    }
};

struct Elem {
    int elCount_, comp_, swap_;
    Elem () { elCount_ = 0; comp_ = 0; swap_ = 0; }
};

int PressButton ( void );
void BackGround ( void );
std::pair < int, int > BubbleSort ( int Mas [], int arraySize );
std::pair < int, int > SelectionSort ( int Mas [], int arraySize );
std::pair < int, int > InsertionSort ( int Mas [], int arraySize );
std::pair < int, int > QuickSort ( int Mas [], int left, int right );
void RandomFill ( int [], int arraySize );
void SortedFill ( int [], int arraySize, bool );
void Swap ( int* a, int* b ) { int temp = *a; *a = *b; *b = temp; }
void OutputArray ( int [], int arraySize );
void DrawGraphics ( Elem [], int arraySize, COLORREF color );
void FillArraySwapCmpCount ( Elem SwapCompCount [], int sortAlg );
void isSorted ( int Mas [], int arraySize );

int main()
    {
    const int buttonsCount = 4;
    srand ( time(NULL) );
    txCreateWindow (SIZEX, SIZEY);
    Button Buttons[buttonsCount] = { { 50, 50, TX_BLUE,   "Bubble_Sort\n" },
                                     { 190, 50, TX_GREEN,  "Selection_Sort\n" },
                                     { 330, 50, TX_RED,    "Insertion_Sort\n" },
                                     { 470, 50, TX_ORANGE, "Quick_sort\n" }};

    for ( int i = 0; i < buttonsCount; i++ ) {
        Buttons[i].DrawButton ();
    }

    Elem SwapCompCount [ARRAYSIZE / STEP] = {};
    if ( !GetKeyState ( VK_SCROLL ) ) BackGround ( );

    /*
    RandomFill ( MyArray, ArraySize );
    OutputArray ( MyArray, ArraySize );
    BubbleSort ( MyArray, ArraySize );
    OutputArray ( MyArray, ArraySize );
    */
    while ( !txGetAsyncKeyState ( VK_ESCAPE ) )
    {
        if ( txMouseButtons() & 1 )
        {
            int i = 0, sortAlg = -1;
            while ( i < buttonsCount )
            {
                if ( Buttons[i].CheckCoords( txMouseX(), txMouseY() ) ) { sortAlg = i; break; }
                i ++;
            }
            if ( sortAlg > -1 )
            {
                FillArraySwapCmpCount ( SwapCompCount, sortAlg );
                if ( !GetKeyState ( VK_SCROLL ) ) DrawGraphics ( SwapCompCount, ARRAYSIZE / STEP, Buttons[sortAlg].color_ );
            }
        }
        txSleep (0);
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

void FillArraySwapCmpCount ( Elem SwapCompCount [], int sortAlg ) {

    int MyArray [ARRAYSIZE] = {};
    std::pair < int, int > Count = { 0, 0 };
    int posAr = 0;
    for ( int len = 2; len < ARRAYSIZE; len += STEP ) {
        RandomFill ( MyArray, ARRAYSIZE );
        switch ( sortAlg ) {
            case 0: Count = BubbleSort ( MyArray, len );
                    if ( GetKeyState ( VK_SCROLL ) )
                    {
                        OutputArray ( MyArray, len );
                        isSorted ( MyArray, len );
                    }
                    break;
            case 1: Count = SelectionSort ( MyArray, len );
                    if ( GetKeyState ( VK_SCROLL ) )
                    {
                        OutputArray ( MyArray, len );
                        isSorted ( MyArray, len );
                    }
                    break;
            case 2: Count = InsertionSort ( MyArray, len );
                    if ( GetKeyState ( VK_SCROLL ) )
                    {
                        OutputArray ( MyArray, len );
                        isSorted ( MyArray, len );
                    }
                    break;
            case 3: Count = QuickSort ( MyArray, 0, len - 1 );
                    if ( GetKeyState ( VK_SCROLL ) )
                    {
                        OutputArray ( MyArray, len );
                        isSorted ( MyArray, len );
                    }
                    break;
        }
        SwapCompCount[posAr].elCount_ = len;
        SwapCompCount[posAr].comp_ = Count.first;
        SwapCompCount[posAr].swap_ = Count.second;
        //printf ( "Elem count = %d, Comp count = %d, swap count = %d\n", len, SwapCompCount[posAr].comp_, SwapCompCount[posAr].swap_);
        posAr ++;
    }
}

//-----------------------------------------------------------------------------

void BackGround ( void ) {
    txSetColor ( TX_WHITE );
    txSetFillColor ( TX_LIGHTGREY );
    txRectangle ( 50, 120, SIZEX - 50, 150 );
    txLine ( SIZEX / 2, 120, SIZEX / 2, 150 );
    txTextOut ( 50 + LENX / 4, 130, "SWAPS COUNT" );
    txTextOut ( SIZEX / 2 + LENX / 4, 130, "COMPARES COUNT" );
    txRectangle ( 50, 150, SIZEX - 50, SIZEY - 50 + 3 ); // sizeY - 50 - 150
    txLine ( SIZEX / 2, SIZEY / 4, SIZEX / 2, SIZEY - 50 );
    POINT trian[3] = { { SIZEX - 10, SIZEY / 2 }, { SIZEX - 20, SIZEY / 2 + 20 }, { SIZEX - 20, SIZEY / 2 - 20 }};
    txPolygon ( trian, 3 );
}

//-----------------------------------------------------------------------------

std::pair < int, int > QuickSort ( int Mas [], int left, int right ) {
      int posL = left, posR = right;
      int mid = Mas[ ( left + right ) / 2 ];
      std::pair < int, int > Count = { 0, 0 };
      while ( posL <= posR ) {
            Count.first ++;
            while ( Mas[posL] < mid ) { posL++; Count.first ++; }
            Count.first ++;
            while ( Mas[posR] > mid ) { posR--; Count.first ++; }
            Count.first ++;
            if ( posL <= posR ) {
                  Count.second ++;
                  Swap ( &Mas[posL], &Mas[posR] );
                  posL++;
                  posR--;
            }
      };
      std::pair < int, int > LeftCount = { 0, 0 }, RightCount = { 0, 0 };
      if ( left < posR ) {
            std::pair < int, int > LeftCount = QuickSort ( Mas, left, posR );
      }
      if ( posL < right ) {
            std::pair < int, int > RightCount = QuickSort ( Mas, posL, right );
      }
      Count.first += LeftCount.first + RightCount.first;
      Count.second += LeftCount.second + RightCount.second;

      return Count;
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

std::pair < int, int > SelectionSort ( int Mas [], int arraySize ) {

    std::pair < int, int > Count = { 0, 0 };

    for ( int i = 0; i < arraySize; i++ ) {
        int posMax = 0;
        for ( int j = 1; j < arraySize - i; j++ ) {
            Count.first ++;
            if ( Mas [posMax] < Mas [j] ) posMax = j;

        }
        Count.second ++;
        Swap ( &Mas[posMax], &Mas[arraySize - 1 - i] );
    }
    return Count;
}

//-----------------------------------------------------------------------------

std::pair < int, int > InsertionSort ( int Mas [], int arraySize ) {

    std::pair < int, int > Count = { 0, 0 };

    for ( int i = 1; i < arraySize; i++ ) {
        int pos = i;
        Count.first ++;
        while ( pos > 0 && Mas[pos] < Mas[pos - 1] ) {
            Swap ( &Mas[pos], &Mas[pos - 1] );
            Count.second ++;
            pos --;
        }
    }
    //OutputArray ( Mas, arraySize );
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
         $d printf ( "%d ", Mas[i] );
     }
     //printf ("\n");
}

//-----------------------------------------------------------------------------

void DrawGraphics ( Elem Mas [], int arraySize, COLORREF color ) {
    txSetColor (color);
    txSetFillColor (color); //elCount_, comp_, swap_
    int MaxElemCmp = Mas[arraySize - 1].comp_ + 1000, MaxElemSwp = Mas[arraySize - 1].swap_ + 1000;

    Coords Point1 ( SIZEX / 2, SIZEY - 50, LENX, LENY );
    Coords Point2 ( 50, SIZEY - 50, LENX, LENY );
    //Point.DrawAxes ( );
    //Point.DrawCircle (200, 100, TX_GREEN);

    for ( int i = 0; i < arraySize; i++ ) {
        double xPos = Mas[i].elCount_, yPos1 = Mas[i].comp_, yPos2 = Mas[i].swap_;
        Point1.DrawCircle ( xPos / ARRAYSIZE * LENX, yPos1 / MaxElemCmp * LENY, color );
        Point2.DrawCircle ( xPos / ARRAYSIZE * LENX, yPos2 / MaxElemSwp * LENY, color );
    }
}

//-----------------------------------------------------------------------------

void SortedFill ( int Mas [], int arraySize, bool incS) {
    for ( int i = 0; i < arraySize; i++ ) {
        if ( incS ) Mas [i] = i;
        else Mas [i] = arraySize - 1 - i;
    }
}

//-----------------------------------------------------------------------------

void isSorted ( int Mas [], int arraySize ) {
    bool isSort = true;
    for ( int i = 1; i < arraySize; i++ ) if ( Mas[i - 1] > Mas[i] ) isSort = false;
    if ( !isSort ) {
        $r    printf ( "\n[FAILED] Array with length %d not sorted.\n\n", arraySize );
    }
    else {
        $g    printf ( "\n[PASSED] Array with length %d is sorted.\n\n", arraySize );
    }
}

//-----------------------------------------------------------------------------
