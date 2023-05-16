#include <iostream>
#include "led-matrix.h"
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>


// uisngs
using namespace std;
using rgb_matrix :: RGBMatrix;
using rgb_matrix :: Canvas;

// Interrupt handler
volatile bool interrupt_received = false;
static void InterruptHandler (int signo)
{
    interrupt_received = true;
}

// Function prototypes
static void draw_canvas (Canvas*);
void draw_background (Canvas*, int, int, int, int, int);
void draw_icon_large(Canvas*, int, int, int);
//void draw_icon_small(Canvas*, int, int, string);


// Main
int main (int argc, char *argv[])
{
    // RGB matrix set up.
    RGBMatrix :: Options defaults;
    defaults.hardware_mapping = "adafruit-hat-pwm";
    defaults.rows = 64;
    defaults.cols = 64;
    defaults.chain_length = 2;
    defaults.parallel = 1;
    defaults.show_refresh_rate = false;
    defaults.brightness = 25;

    Canvas* canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
    if (canvas == NULL)
        return 1;

    // Setup of signal handler for exiting cleanly with CTRL-C.
    signal (SIGTERM, InterruptHandler);
    signal (SIGINT, InterruptHandler);

    // Using the canvas.
    draw_canvas (canvas);    

    // Animation finished. Shut down the RGB matrix.
    canvas -> Clear();
    delete canvas;
    
    return 0;
}


static void draw_canvas (Canvas* canvas)
{
    // Background R G B color.
    int bkgndR = 0, bkgndG = 0, bkgndB = 0;
    
    short width = canvas -> width();
    short height = canvas -> height();
    //string temp, humidity, windSpeed;

    while (true)
    {
        if (interrupt_received) 
            return;    

        draw_background (canvas, width, height, bkgndR, bkgndG, bkgndB);

       

        draw_icon_large(canvas, 50, 1, 1);
        draw_icon_large(canvas, 90, 1, 2);
        // draw each piece one at a time

       /*  // Print the current temp
        printText (canvas, 'l', temp, 2, 8, 255, 255, 255);
        // Humidity
        printText (canvas, 'm', humidity, 46, 1, 255, 255, 255);
        // Windspeed
        printText (canvas, 'm', windSpeed, 89, 1, 255, 255, 255); */

        // Wait 5 seconds
        usleep(5000000);
    }
}

void draw_background (Canvas* canvas, int width, int height, int r, int g, int b)
{
    for (int col = 0; col < width; col++)
    {
        for (int row = 0; row < height; row++)
        {
            canvas -> SetPixel (col, row, r, g, b);
        }
    }
}

void draw_icon_large(Canvas* canvas, int x_origin, int y_origin, int icon_num)
{
    switch (icon_num)
    {
        case 1:
        {
            for (int i = 7; i <= 13; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 0, 255, 255, 0);

            for (int i = 5; i <= 15; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 1, 255, 255, 0);

            for (int i = 4; i <= 16; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 2, 255, 255, 0);

            for (int i = 3; i <= 17; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 3, 255, 255, 0);

            for (int i = 2; i <= 18; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 4, 255, 255, 0);

            for (int i = 1; i <= 19; i++)
            {
                canvas -> SetPixel (x_origin + i, y_origin + 5, 255, 255, 0);
                canvas -> SetPixel (x_origin + i, y_origin + 6, 255, 255, 0);
            }

            for (int i = 0; i <= 20; i++)
            {
                canvas -> SetPixel (x_origin + i, y_origin + 7, 255, 255, 0);
                canvas -> SetPixel (x_origin + i, y_origin + 8, 255, 255, 0);
                canvas -> SetPixel (x_origin + i, y_origin + 9, 255, 255, 0);
                canvas -> SetPixel (x_origin + i, y_origin + 10, 255, 255, 0);
                canvas -> SetPixel (x_origin + i, y_origin + 11, 255, 255, 0);
            }

            for (int i = 1; i <= 19; i++)
            {
                canvas -> SetPixel (x_origin + i, y_origin + 12, 255, 255, 0);
                canvas -> SetPixel (x_origin + i, y_origin + 13, 255, 255, 0);
            }

            for (int i = 2; i <= 18; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 14, 255, 255, 0);

            for (int i = 3; i <= 17; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 15, 255, 255, 0);

            for (int i = 4; i <= 16; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 16, 255, 255, 0);

            for (int i = 5; i <= 15; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 17, 255, 255, 0);

            for (int i = 7; i <= 13; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 18, 255, 255, 0);

            break;
        }
        case 2:
        {
            for (int i = 7; i <= 13; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 0, 90, 90, 90);

            for (int i = 5; i <= 15; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 1, 90, 90, 90);

            for (int i = 4; i <= 16; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 2, 90, 90, 90);

            for (int i = 3; i <= 17; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 3, 90, 90, 90);

            for (int i = 2; i <= 18; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 4, 90, 90, 90);

            for (int i = 1; i <= 19; i++)
            {
                canvas -> SetPixel (x_origin + i, y_origin + 5, 90, 90, 90);
                canvas -> SetPixel (x_origin + i, y_origin + 6, 90, 90, 90);
            }

            for (int i = 0; i <= 20; i++)
            {
                canvas -> SetPixel (x_origin + i, y_origin + 7, 90, 90, 90);
                canvas -> SetPixel (x_origin + i, y_origin + 8, 90, 90, 90);
                canvas -> SetPixel (x_origin + i, y_origin + 9, 90, 90, 90);
                canvas -> SetPixel (x_origin + i, y_origin + 10, 90, 90, 90);
                canvas -> SetPixel (x_origin + i, y_origin + 11, 90, 90, 90);
            }

            for (int i = 1; i <= 19; i++)
            {
                canvas -> SetPixel (x_origin + i, y_origin + 12, 90, 90, 90);
                canvas -> SetPixel (x_origin + i, y_origin + 13, 90, 90, 90);
            }

            for (int i = 2; i <= 18; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 14, 90, 90, 90);

            for (int i = 3; i <= 17; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 15, 90, 90, 90);

            for (int i = 4; i <= 16; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 16, 90, 90, 90);

            for (int i = 5; i <= 15; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 17, 90, 90, 90);

            for (int i = 7; i <= 13; i++)
                canvas -> SetPixel (x_origin + i, y_origin + 18, 90, 90, 90);

            break;
        }
        case 3:
        {
            canvas -> SetPixel (x_origin + 17, y_origin + 0, 255, 255, 0);
            canvas -> SetPixel (x_origin + 18, y_origin + 0, 255, 255, 0);
            canvas -> SetPixel (x_origin + 19, y_origin + 0, 255, 255, 0);
            canvas -> SetPixel (x_origin + 20, y_origin + 0, 255, 255, 0);
            canvas -> SetPixel (x_origin + 21, y_origin + 0, 255, 255, 0);

            canvas -> SetPixel (x_origin + 15, y_origin + 1, 255, 255, 0);
            canvas -> SetPixel (x_origin + 16, y_origin + 1, 255, 255, 0);
            canvas -> SetPixel (x_origin + 17, y_origin + 1, 255, 255, 0);
            canvas -> SetPixel (x_origin + 18, y_origin + 1, 255, 255, 0);
            canvas -> SetPixel (x_origin + 19, y_origin + 1, 255, 255, 0);
            canvas -> SetPixel (x_origin + 20, y_origin + 1, 255, 255, 0);
            canvas -> SetPixel (x_origin + 21, y_origin + 1, 255, 255, 0);
            canvas -> SetPixel (x_origin + 22, y_origin + 1, 255, 255, 0);
            canvas -> SetPixel (x_origin + 23, y_origin + 1, 255, 255, 0);

            canvas -> SetPixel (x_origin + 14, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 15, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 16, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 17, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 18, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 19, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 20, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 21, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 22, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 23, y_origin + 2, 255, 255, 0);
            canvas -> SetPixel (x_origin + 24, y_origin + 2, 255, 255, 0);

            canvas -> SetPixel (x_origin + 9, y_origin + 3, 255, 255, 255);
            canvas -> SetPixel (x_origin + 10, y_origin + 3, 255, 255, 255);
            canvas -> SetPixel (x_origin + 11, y_origin + 3, 255, 255, 255);
            canvas -> SetPixel (x_origin + 12, y_origin + 3, 255, 255, 255);
            canvas -> SetPixel (x_origin + 13, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 14, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 15, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 16, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 17, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 18, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 19, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 20, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 21, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 22, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 23, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 24, y_origin + 3, 255, 255, 0);
            canvas -> SetPixel (x_origin + 25, y_origin + 3, 255, 255, 0);

            canvas -> SetPixel (x_origin + 8, y_origin + 4, 255, 255, 255);
            canvas -> SetPixel (x_origin + 9, y_origin + 4, 255, 255, 255);
            canvas -> SetPixel (x_origin + 10, y_origin + 4, 255, 255, 255);
            canvas -> SetPixel (x_origin + 12, y_origin + 4, 255, 255, 255);
            canvas -> SetPixel (x_origin + 13, y_origin + 4, 255, 255, 255);
            canvas -> SetPixel (x_origin + 14, y_origin + 4, 255, 255, 255);
            

            break;
        }
        case 4:
        {

            break;
        }
        case 5:
        {

            break;
        }
        case 6:
        {

            break;
        }
        case 7:
        {

            break;
        }
        case 8:
        {

            break;
        }
        case 9:
        {

            break;
        }
        case 10:
        {

            break;
        }
        case 11:
        {

            break;
        }
        case 12:
        {

            break;
        }
    }
    return;
}








/* void draw_icon_small(Canvas* canvas, int x_origin, int y_origin, string icon_name)
{
    switch (icon_name)
    {
        case "clear sky":
        {

            break;
        }
        case "clear sky night":
        {

            break;
        }
        case "few clouds":
        {

            break;
        }
        case "few clouds night":
        {

            break;
        }
        case "scattered clouds":
        {

            break;
        }
        case "broken clouds":
        {

            break;
        }
        case "shower rain":
        {

            break;
        }
        case "rain":
        {

            break;
        }
        case "rain night":
        {

            break;
        }
        case "thunderstorm":
        {

            break;
        }
        case "snow":
        {

            break;
        }
        case "mist":
        {

            break;
        }
    }
} */
