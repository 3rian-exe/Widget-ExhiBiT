#include "led-matrix.h"
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <vector>

using namespace std;
using rgb_matrix :: RGBMatrix;
using rgb_matrix :: Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler (int signo)
{
    interrupt_received = true;
}

static void DrawOnCanvas (Canvas*);
void drawBackgroundBasic(Canvas*, int, int, int, int, int);
void printText (Canvas*, char, string, int, int, int, int, int);
int drawSmallChar (Canvas*, char, int, int, int, int, int);
int drawMediumChar (Canvas*, char, int, int, int, int, int);
int drawLargeChar (Canvas*, char, int, int, int, int, int);

int main (int argc, char *argv[])
{
    // RGB matrix set up.
    RGBMatrix :: Options defaults;
    defaults.hardware_mapping = "adafruit-hat-pwm";
    defaults.rows = 64;
    defaults.cols = 64;
    defaults.chain_length = 2;
    defaults.parallel = 1;
    defaults.show_refresh_rate = true;
    defaults.brightness = 25;

    Canvas* canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
    if (canvas == NULL)
        return 1;

    // Setup of signal handler for exiting cleanly with CTRL-C.
    signal (SIGTERM, InterruptHandler);
    signal (SIGINT, InterruptHandler);

    // Using the canvas.
    DrawOnCanvas (canvas);    

    // Animation finished. Shut down the RGB matrix.
    canvas -> Clear();
    delete canvas;
    
    return 0;
}

static void DrawOnCanvas (Canvas *canvas) 
{
    // Let's create a simple animation. We use the canvas to draw
    // pixels. We wait between each step to have a slower animation.

    short width = canvas -> width();
    short height = canvas -> height();

    int r = 255, g = 255, b = 255, bkgndR = 112, bkgndG = 154, bkgndB = 209, i = 0;

    while (true)
    {
        if (interrupt_received)
            return;


        drawBackgroundBasic (canvas, width, height, bkgndR, bkgndG, bkgndB);

        int x = 1, y = 3, x1 = 1, y1 = 14;
        vector <string> list = {"abcdef", "ghijkl", "mnopqr", "stuvw", "xyz"};
        if (i == 5)
            i = 0;
        
        printText (canvas, 's', list[i], x, y, r, g, b);
        printText (canvas, 'm', list[i], x1, y1, r, g, b);
        usleep(2000000);

        i++;
    }
}

void drawBackgroundBasic(Canvas* canvas, int width, int height, int bkgndR, int bkgndG, int bkgndB)
{
    for (int col = 0; col < width; col++)
    {
        for (int row = 0; row < height; row++)
        {
            // Sets the background color.
            canvas -> SetPixel(col, row, bkgndR, bkgndG, bkgndB);

            // Draws a dark gray rectangle for the MTA countdown. The bottom left 
            // corner of this rectangle is at col = 55, row = 29 (0 indexed).
            if ((col > 54) && (row < 30))
            canvas -> SetPixel(col, row, 70, 70, 70);

            // Draws a light gray border around the previously drawn dark gray 
            // rectangle and curves the bottom left corner. 
            if ((col == 54) && (row < 26))
            canvas -> SetPixel(col, row, 180, 180, 180);
      
            if ((col == 55) && (row > 21) && (row < 27))
                canvas -> SetPixel(col, row, 180, 180, 180);

            if ((col == 56) && (row > 24) && (row < 29))
                canvas -> SetPixel(col, row, 180, 180, 180);

            if ((col == 57) && (row > 26) && (row < 29))
                canvas -> SetPixel(col, row, 180, 180, 180);

            if (((col == 58) || (col == 59)) && (row > 27) && (row < 30))
                canvas -> SetPixel(col, row, 180, 180, 180);

            if ((col > 59) && (col < 63) && (row == 29))
                canvas -> SetPixel(col, row, 180, 180, 180);

            if ((col > 59) && (col < 128) && (row == 30))
                canvas -> SetPixel(col, row, 180, 180, 180);

            // Get rid of the 5 dark grey (70, 70, 70) pixels making a corner on the bottom left of the box. 
            if ((col == 55) && ((row == 27) || (row == 28) || (row == 29)))
                canvas -> SetPixel(col, row, 112, 154, 209);

            if (((col == 56) || (col == 57)) && (row == 29))
                canvas -> SetPixel(col, row, 112, 154, 209);
        }
    }
}

void printText (Canvas* canvas, char textSize, string text, int xOrigin, 
                                    int yOrigin, int r, int g, int b)
{
    int spacing = 0, textLength = text.length();
    if (textSize == 'S' || textSize == 's')
    {
        for (int smallStrIndex = 0; smallStrIndex < textLength; smallStrIndex++)
        {
            spacing += drawSmallChar(canvas, text[smallStrIndex], xOrigin + spacing, yOrigin, r, g, b);
        }
    }

    else if (textSize == 'M' || textSize == 'm')
    {
        for (int mediumStrIndex = 0; mediumStrIndex < textLength; mediumStrIndex++)
        {
            spacing += drawMediumChar(canvas, text[mediumStrIndex], xOrigin + spacing, yOrigin, r, g, b);
        }
    }

    else if (textSize == 'L' || textSize == 'l')
    {
        for (int largeStrIndex = 0; largeStrIndex < textLength; largeStrIndex++)
        {
            spacing += drawLargeChar(canvas, text[largeStrIndex], xOrigin + spacing, yOrigin, r, g, b);
        }
    }

    else 
        return;
    
}


//*********************************************************
//      * * * D R A W  S M A L L  C H A R A C T E R * * *
//*********************************************************


// Return an int value for character spacing. 
int drawSmallChar (Canvas *canvas, char letter, int xOrigin, int yOrigin, 
                                                        int r, int g, int b)
{

    // *** TO DO ***
    // If the char passed exceeds the width of the display, then scroll.

    switch (letter)
    {
        case ' ':
            return 2;

        case 'a':
        case 'A':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);

            // Spacing each character.   
            return 5;
        }

        case 'b':
        case 'B':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 5;
        }

        case 'c':
        case 'C':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 5;
        }
        
        case 'd':
        case 'D':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            // Spacing each character.   
            return 5;
        }

        case 'e':
        case 'E':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);

            // Spacing each character.   
            return 5;
        }

        case 'f':
        case 'F':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);

            return 5;
        }

        case 'g':
        case 'G':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 5;
        }

        case 'h':
        case 'H':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);

            // Spacing each character.   
            return 5;
        }

        case 'i':
        case 'I':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 4;
        }

        case 'j':
        case 'J':
        {
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            
            return 5;
        }

        case 'k':
        case 'K':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);

            return 5;
        }

        case 'l':
        case 'L':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
        
            // Spacing each character.   
            return 5;
        }

        case 'm':
        case 'M':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);

            return 6;
        }

        case 'n':
        case 'N':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);

            return 5;
        }

        case 'o':
        case 'O':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
        
            // Spacing each character.   
            return 5;
        }

        case 'p':
        case 'P':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);

            return 5;
        }

        case 'q':
        case 'Q':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            
            return 5;
        }

        case 'r':
        case 'R':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);

            // Spacing each character.   
            return 5;
        }

        case 's':
        case 'S':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 5;
        }

        case 't':
        case 'T':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 6;
        }

        case 'u':
        case 'U':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
        
            return 5;
        }

        case 'v':
        case 'V':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
        
            return 6;
        }

        case 'w':
        case 'W':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            
            // Spacing each character.   
            return 6;
        }

        case 'x':
        case 'X':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
        
            return 5;
        }

        case 'y':
        case 'Y':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
        
            return 6;
        }

        case 'z':
        case 'Z':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
        
            return 5;
        }

        case '0':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 5;
        }

        case '1':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 4;
        }

        case '2':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);

            return 5;
        }

        case '3':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            
            return 5;
        }

        case '4':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 5;
        }

        case '5':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 5;
        }

        case '6':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);    

            return 5;
        }

        case '7':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);

            return 5;
        }

        case '8':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            
            return 5;
        }

        case '9':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            
            return 5;
        }

        default:
            return 0;
    }
}


//*********************************************************
//      * * * D R A W  M E D I U M  C H A R A C T E R * * *
//*********************************************************

int drawMediumChar (Canvas *canvas, char letter, int xOrigin, int yOrigin, 
                                                        int r, int g, int b)
{
    switch (letter)
    {
        case ' ':
            return 4;

        case 'a':
        case 'A':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);

            return 10;
        }

        case 'b':
        case 'B':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
            
            return 10;
        }

        case 'c':
        case 'C':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);

            return 10;
        }
        
        case 'd':
        case 'D':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);

            return 10;
        }

        case 'e':
        case 'E':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);
   
            return 10;
        }

        case 'f':
        case 'F':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            
            return 10;
        }

        case 'g':
        case 'G':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);

            return 10;
        }

        case 'h':
        case 'H':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);
  
            return 10;
        }

        case 'i':
        case 'I':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
            
            return 8;
        }

        case 'j':
        case 'J':
        {
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
            
            return 10;
        }

        case 'k':
        case 'K':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);

            return 10;
        }

        case 'l':
        case 'L':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);
              
            return 10;
        }

        case 'm':
        case 'M':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 9, r, g, b);

            return 12;
        }

        case 'n':
        case 'N':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);

            return 10;
        }

        case 'o':
        case 'O':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
           
            return 10;
        }

        case 'p':
        case 'P':
        {            
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);

            return 10;
        }

        case 'q':
        case 'Q':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);

            return 10;
        }

        case 'r':
        case 'R':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);
            
            return 10;
        }

        case 's':
        case 'S':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);

            return 10;
        }

        case 't':
        case 'T':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);

            return 12;
        }

        case 'u':
        case 'U':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
        
            return 10;
        }

        case 'v':
        case 'V':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);

            return 12;
        }

        case 'w':
        case 'W':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 9, r, g, b);

            return 12;
        }

        case 'x':
        case 'X':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);
        
            return 10;
        }

        case 'y':
        case 'Y':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 8, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 9, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
            
            return 12;
        }

        case 'z':
        case 'Z':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 9, r, g, b);
        
            return 10;
        }

        case '0':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 6, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 7, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);
           
            return 10;
        }

        case '1':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 1, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 2, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 3, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 4, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 5, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 6, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 7, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 8, r, g, b);
            canvas -> SetPixel(xOrigin + 0, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 1, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 2, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 3, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 4, yOrigin + 9, r, g, b);
            canvas -> SetPixel(xOrigin + 5, yOrigin + 9, r, g, b);

            return 8;
        }

        case '2':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case '3':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            
            
            return 0;
        }

        case '4':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case '5':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case '6':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
              

            return 0;
        }

        case '7':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
           

            return 0;
        }

        case '8':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
           
            
            return 0;
        }

        case '9':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            
            
            return 0;
        }

        default:
            return 0;
    }

    
}

//*********************************************************
//      * * * D R A W  L A R G E  C H A R A C T E R * * *
//*********************************************************

int drawLargeChar (Canvas *canvas, char letter, int xOrigin, int yOrigin, 
                                                        int r, int g, int b)
{
    switch (letter)
    {
        case ' ':
            return 4;

        case 'a':
        case 'A':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'b':
        case 'B':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'c':
        case 'C':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            

            return 0;
        }
        
        case 'd':
        case 'D':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            // Spacing each character.   
            return 0;
        }

        case 'e':
        case 'E':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            // Spacing each character.   
            return 0;
        }

        case 'f':
        case 'F':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'g':
        case 'G':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'h':
        case 'H':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
           

            // Spacing each character.   
            return 0;
        }

        case 'i':
        case 'I':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'j':
        case 'J':
        {
            canvas -> SetPixel(xOrigin + 3, yOrigin + 0, r, g, b);
            
            
            return 0;
        }

        case 'k':
        case 'K':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'l':
        case 'L':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            
        
            // Spacing each character.   
            return 0;
        }

        case 'm':
        case 'M':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'n':
        case 'N':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'o':
        case 'O':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            
        
            // Spacing each character.   
            return 0;
        }

        case 'p':
        case 'P':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'q':
        case 'Q':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            
            
            return 0;
        }

        case 'r':
        case 'R':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            // Spacing each character.   
            return 0;
        }

        case 's':
        case 'S':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 't':
        case 'T':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case 'u':
        case 'U':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            
        
            return 0;
        }

        case 'v':
        case 'V':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            
        
            return 0;
        }

        case 'w':
        case 'W':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            
            
            // Spacing each character.   
            return 0;
        }

        case 'x':
        case 'X':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            
        
            return 0;
        }

        case 'y':
        case 'Y':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            
        
            return 0;
        }

        case 'z':
        case 'Z':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            
        
            return 0;
        }

        case '0':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case '1':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case '2':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case '3':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            
            
            return 0;
        }

        case '4':
        {
            canvas -> SetPixel(xOrigin + 2, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case '5':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
            

            return 0;
        }

        case '6':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
              

            return 0;
        }

        case '7':
        {
            canvas -> SetPixel(xOrigin + 0, yOrigin + 0, r, g, b);
           

            return 0;
        }

        case '8':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
           
            
            return 0;
        }

        case '9':
        {
            canvas -> SetPixel(xOrigin + 1, yOrigin + 0, r, g, b);
            
            
            return 0;
        }

        default:
            return 0;
    }
}
