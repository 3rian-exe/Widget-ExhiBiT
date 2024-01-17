// Includes
#include <iostream>
#include "led-matrix.h"
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>


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
void draw_wind(Canvas*, int, int);
void draw_humidity(Canvas*, int, int);
void write_condition(Canvas*, string, int, int);
void draw_icon(Canvas*, string, int, int);

struct hourlyWeather
{
    string time, temp, condition;
};

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
    int bkgndR = 0, bkgndG = 0, bkgndB = 0, checkerror = 0;
    
    short width = canvas -> width();
    short height = canvas -> height();

    // Terminal command for running the python script
    // to make the http request.
    char cmd[] = "python3 store_weather_data.py";

    // Ensure that the data has had enough time to be retrieved and written.
    usleep(2000000);

    string temp, humidity, windSpeed, condition;

    while (true)
    {
        if (interrupt_received) 
            return;    

        draw_background (canvas, width, height, bkgndR, bkgndG, bkgndB);

        char currentWeatherFile[] = "current_weather_data.txt";
        // Running the python script and error checking.
        checkerror = system (cmd);
        if (checkerror == -1)
        {
            std :: cout << "system error\n";
            return;
        }

        // Getting the current weather data from the text file.
        ifstream current (currentWeatherFile);
        current >> temp
            >> condition
            >> humidity
            >> windSpeed;

        current.close();


        // Print the current temp
        printText (canvas, 'l', temp, 2, 3, 255, 255, 255);
        // Humidity
        printText (canvas, 's', humidity + "%", 98, 3, 255, 255, 255);
        // Windspeed
        printText (canvas, 's', windSpeed + "mph", 98, 13, 255, 255, 255);

        write_condition(canvas, condition, 48, 1);
        draw_wind(canvas, 80, 12);
        draw_humidity(canvas, 82, 2);



        for (int i = 0; i < 128; i++)
        {
            canvas -> SetPixel (i, 23, 255, 255, 255);
            canvas -> SetPixel (i, 24, 255, 255, 255);
        }

        hourlyWeather hourlyWeatherArray[5];
        char hourlyWeatherFile[] = "hourly_weather_data.txt";
        // Running the python script and error checking.
        checkerror = system (cmd);
        if (checkerror == -1)
        {
            std :: cout << "system error\n";
            return;
        }

        // Getting the current weather data from the text file.
        ifstream hourly (hourlyWeatherFile);

        for (int hour = 0; hour < 5; hour++)
        {
            hourly >> hourlyWeatherArray[hour].time
                >> hourlyWeatherArray[hour].temp
                >> hourlyWeatherArray[hour].condition;
        }
        hourly.close();

        // Print the hourly weather
        int h = 0; // Hourly weather array struct
        int x = 110;
        for (int index = 1; index < x; index += 26)
        {
            printText (canvas, 's', hourlyWeatherArray[h].time, index + 1, 29, 255, 255, 255);
            
            // Draw a line
            for (int k = 0; k < 21; k++)
                canvas -> SetPixel (index + k, 35, 255, 255, 255);

            printText (canvas, 's', hourlyWeatherArray[h].temp, index + 1, 37, 255, 255, 255);
            // Print the condition icon

            draw_icon(canvas, hourlyWeatherArray[h].condition, index + 1, 44);
            h++;
        }
        // draw each piece one at a time

        usleep(600000000);
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

void draw_wind(Canvas* canvas, int x_origin, int y_origin)
{
    canvas -> SetPixel (x_origin + 6, y_origin + 0, 255, 255, 255);
    canvas -> SetPixel (x_origin + 7, y_origin + 0, 255, 255, 255);

    canvas -> SetPixel (x_origin + 5, y_origin + 1, 255, 255, 255);
    canvas -> SetPixel (x_origin + 8, y_origin + 1, 255, 255, 255);

    canvas -> SetPixel (x_origin + 1, y_origin + 2, 255, 255, 255);
    canvas -> SetPixel (x_origin + 2, y_origin + 2, 255, 255, 255);
    canvas -> SetPixel (x_origin + 3, y_origin + 2, 255, 255, 255);
    canvas -> SetPixel (x_origin + 5, y_origin + 2, 255, 255, 255);
    canvas -> SetPixel (x_origin + 6, y_origin + 2, 255, 255, 255);
    canvas -> SetPixel (x_origin + 8, y_origin + 2, 255, 255, 255);

    canvas -> SetPixel (x_origin + 8, y_origin + 3, 255, 255, 255);

    canvas -> SetPixel (x_origin + 2, y_origin + 4, 255, 255, 255);
    canvas -> SetPixel (x_origin + 3, y_origin + 4, 255, 255, 255);
    canvas -> SetPixel (x_origin + 4, y_origin + 4, 255, 255, 255);
    canvas -> SetPixel (x_origin + 5, y_origin + 4, 255, 255, 255);
    canvas -> SetPixel (x_origin + 6, y_origin + 4, 255, 255, 255);
    canvas -> SetPixel (x_origin + 7, y_origin + 4, 255, 255, 255);

    canvas -> SetPixel (x_origin + 3, y_origin + 6, 255, 255, 255);
    canvas -> SetPixel (x_origin + 4, y_origin + 6, 255, 255, 255);
    canvas -> SetPixel (x_origin + 5, y_origin + 6, 255, 255, 255);
    canvas -> SetPixel (x_origin + 6, y_origin + 6, 255, 255, 255);
    canvas -> SetPixel (x_origin + 7, y_origin + 6, 255, 255, 255);
    canvas -> SetPixel (x_origin + 8, y_origin + 6, 255, 255, 255);

    canvas -> SetPixel (x_origin + 8, y_origin + 7, 255, 255, 255);



    return;
}

void draw_humidity(Canvas* canvas, int x_origin, int y_origin)
{
    canvas -> SetPixel (x_origin + 3, y_origin + 0, 0, 239, 183);

    canvas -> SetPixel (x_origin + 2, y_origin + 1, 0, 239, 183);
    canvas -> SetPixel (x_origin + 3, y_origin + 1, 0, 239, 183);
    canvas -> SetPixel (x_origin + 4, y_origin + 1, 0, 239, 183);

    canvas -> SetPixel (x_origin + 2, y_origin + 2, 0, 239, 183);
    canvas -> SetPixel (x_origin + 3, y_origin + 2, 0, 239, 183);
    canvas -> SetPixel (x_origin + 4, y_origin + 2, 0, 239, 183);

    canvas -> SetPixel (x_origin + 1, y_origin + 3, 0, 239, 183);
    canvas -> SetPixel (x_origin + 2, y_origin + 3, 0, 239, 183);
    canvas -> SetPixel (x_origin + 3, y_origin + 3, 0, 239, 183);
    canvas -> SetPixel (x_origin + 4, y_origin + 3, 45, 121, 252);
    canvas -> SetPixel (x_origin + 5, y_origin + 3, 0, 239, 183);

    canvas -> SetPixel (x_origin + 1, y_origin + 4, 0, 239, 183);
    canvas -> SetPixel (x_origin + 2, y_origin + 4, 0, 239, 183);
    canvas -> SetPixel (x_origin + 3, y_origin + 4, 0, 239, 183);
    canvas -> SetPixel (x_origin + 4, y_origin + 4, 45, 121, 252);
    canvas -> SetPixel (x_origin + 5, y_origin + 4, 0, 239, 183);

    canvas -> SetPixel (x_origin + 1, y_origin + 5, 0, 239, 183);
    canvas -> SetPixel (x_origin + 2, y_origin + 5, 0, 239, 183);
    canvas -> SetPixel (x_origin + 3, y_origin + 5, 45, 121, 252);
    canvas -> SetPixel (x_origin + 4, y_origin + 5, 45, 121, 252);
    canvas -> SetPixel (x_origin + 5, y_origin + 5, 0, 239, 183);

    canvas -> SetPixel (x_origin + 2, y_origin + 6, 0, 239, 183);
    canvas -> SetPixel (x_origin + 3, y_origin + 6, 0, 239, 183);
    canvas -> SetPixel (x_origin + 4, y_origin + 6, 0, 239, 183);
    return;
}

void write_condition(Canvas* canvas, string condition, int x_origin, int y_origin)
{
    if (condition == "clearsky")
    {
        printText (canvas, 's', "clear", x_origin, y_origin, 255, 255, 255);
        printText (canvas, 's', "sky", x_origin, y_origin + 7, 255, 255, 255);
    }
    else if (condition == "fewclouds")
    {
        printText (canvas, 's', "few", x_origin, y_origin, 255, 255, 255);
        printText (canvas, 's', "clouds", x_origin, y_origin + 7, 255, 255, 255);
    }
    else if (condition == "scatteredclouds")
    {
        printText (canvas, 's', "scattered", x_origin, y_origin, 255, 255, 255);
        printText (canvas, 's', "clouds", x_origin, y_origin + 7, 255, 255, 255);
    }
    else if (condition == "brokenclouds")
    {
        printText (canvas, 's', "broken", x_origin, y_origin, 255, 255, 255);
        printText (canvas, 's', "clouds", x_origin, y_origin + 7, 255, 255, 255);
    }
    else if (condition == "showerrain")
    {
        printText (canvas, 's', "rain", x_origin, y_origin, 255, 255, 255);
        printText (canvas, 's', "shower", x_origin, y_origin + 7, 255, 255, 255);
    }
    else if (condition == "rain")
    {
        printText (canvas, 's', "rain", x_origin, y_origin + 5, 255, 255, 255);
    }
    else if (condition == "thunderstorm")
    {
        printText (canvas, 's', "thunder", x_origin, y_origin, 255, 255, 255);
        printText (canvas, 's', "storm", x_origin, y_origin + 7, 255, 255, 255);
    }
    else if (condition == "snow")
    {
        printText (canvas, 's', "snow", x_origin, y_origin + 5, 255, 255, 255);
    }
    else
    {
        printText (canvas, 's', "mist", x_origin, y_origin + 5, 255, 255, 255);
    }
    return;
}

void draw_icon(Canvas* canvas, string condition, int x_origin, int y_origin)
{
    if (condition == "clearsky")
    {
        for (int i = 3; i < 8; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 0, 229, 122, 170);

        for (int i = 2; i < 9; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 1, 229, 122, 170);

        for (int i = 1; i < 10; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 2, 229, 122, 170);
        
        for (int i = 0; i < 11; i++)
        {
            canvas -> SetPixel (x_origin + i, y_origin + 3, 229, 122, 170);
            canvas -> SetPixel (x_origin + i, y_origin + 4, 229, 122, 170);
            canvas -> SetPixel (x_origin + i, y_origin + 5, 229, 122, 170);
            canvas -> SetPixel (x_origin + i, y_origin + 6, 229, 122, 170);
            canvas -> SetPixel (x_origin + i, y_origin + 7, 229, 122, 170);
        }
        
        for (int i = 1; i < 10; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 8, 229, 122, 170);

        for (int i = 2; i < 9; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 9, 229, 122, 170);

        for (int i = 3; i < 8; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 10, 229, 122, 170);

    }
    else if (condition == "fewclouds")
    {
        canvas -> SetPixel (x_origin + 9, y_origin + 0, 229, 122, 170);
        canvas -> SetPixel (x_origin + 10, y_origin + 0, 229, 122, 170);
        canvas -> SetPixel (x_origin + 11, y_origin + 0, 229, 122, 170);

        canvas -> SetPixel (x_origin + 5, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 1, 229, 122, 170);
        canvas -> SetPixel (x_origin + 9, y_origin + 1, 229, 122, 170);
        canvas -> SetPixel (x_origin + 10, y_origin + 1, 229, 122, 170);
        canvas -> SetPixel (x_origin + 11, y_origin + 1, 229, 122, 170);
        canvas -> SetPixel (x_origin + 12, y_origin + 1, 229, 122, 170);

        canvas -> SetPixel (x_origin + 3, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 2, 229, 122, 170);
        canvas -> SetPixel (x_origin + 12, y_origin + 2, 229, 122, 170);
        canvas -> SetPixel (x_origin + 13, y_origin + 2, 229, 122, 170);

        canvas -> SetPixel (x_origin + 2, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 3, 229, 122, 170);
        canvas -> SetPixel (x_origin + 13, y_origin + 3, 229, 122, 170);

        canvas -> SetPixel (x_origin + 1, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 4, 180, 180, 180);

        canvas -> SetPixel (x_origin + 0, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 1, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 5, 180, 180, 180);

        canvas -> SetPixel (x_origin + 1, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 6, 180, 180, 180);
    }
    else if (condition == "brokenclouds" || condition == "scatteredclouds")
    {
        canvas -> SetPixel (x_origin + 8, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 0, 180, 180, 180);

        canvas -> SetPixel (x_origin + 7, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 1, 180, 180, 180);

        canvas -> SetPixel (x_origin + 5, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 8, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 9, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 10, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 11, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 2, 180, 180, 180);

        canvas -> SetPixel (x_origin + 3, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 3, 255, 255, 255);
        canvas -> SetPixel (x_origin + 12, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 16, y_origin + 3, 180, 180, 180);

        canvas -> SetPixel (x_origin + 2, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 13, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 14, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 15, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 16, y_origin + 4, 180, 180, 180);

        canvas -> SetPixel (x_origin + 1, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 5, 255, 255, 255);
        canvas -> SetPixel (x_origin + 16, y_origin + 5, 180, 180, 180);

        canvas -> SetPixel (x_origin + 0, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 1, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 6, 180, 180, 180);

        canvas -> SetPixel (x_origin + 1, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 7, 180, 180, 180);
        
    }
    else if (condition == "showerrain")
    {
        canvas -> SetPixel (x_origin + 8, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 0, 180, 180, 180);

        canvas -> SetPixel (x_origin + 7, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 1, 180, 180, 180);

        canvas -> SetPixel (x_origin + 5, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 8, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 9, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 10, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 11, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 2, 180, 180, 180);

        canvas -> SetPixel (x_origin + 3, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 3, 255, 255, 255);
        canvas -> SetPixel (x_origin + 12, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 16, y_origin + 3, 180, 180, 180);

        canvas -> SetPixel (x_origin + 2, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 13, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 14, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 15, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 16, y_origin + 4, 180, 180, 180);

        canvas -> SetPixel (x_origin + 1, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 5, 255, 255, 255);
        canvas -> SetPixel (x_origin + 16, y_origin + 5, 180, 180, 180);

        canvas -> SetPixel (x_origin + 0, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 1, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 6, 180, 180, 180);

        canvas -> SetPixel (x_origin + 1, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 7, 180, 180, 180);

        canvas -> SetPixel (x_origin + 3, y_origin + 9, 0, 183, 239);
        canvas -> SetPixel (x_origin + 6, y_origin + 9, 0, 183, 239);
        canvas -> SetPixel (x_origin + 9, y_origin + 9, 0, 183, 239);
        canvas -> SetPixel (x_origin + 12, y_origin + 9, 0, 183, 239);

        canvas -> SetPixel (x_origin + 2, y_origin + 10, 0, 183, 239);
        canvas -> SetPixel (x_origin + 5, y_origin + 10, 0, 183, 239);
        canvas -> SetPixel (x_origin + 8, y_origin + 10, 0, 183, 239);
        canvas -> SetPixel (x_origin + 11, y_origin + 10, 0, 183, 239);

        canvas -> SetPixel (x_origin + 1, y_origin + 11, 0, 183, 239);
        canvas -> SetPixel (x_origin + 4, y_origin + 11, 0, 183, 239);
        canvas -> SetPixel (x_origin + 7, y_origin + 11, 0, 183, 239);
        canvas -> SetPixel (x_origin + 10, y_origin + 11, 0, 183, 239);

    }
    else if (condition == "rain")
    {
        canvas -> SetPixel (x_origin + 9, y_origin + 0, 229, 122, 170);
        canvas -> SetPixel (x_origin + 10, y_origin + 0, 229, 122, 170);
        canvas -> SetPixel (x_origin + 11, y_origin + 0, 229, 122, 170);

        canvas -> SetPixel (x_origin + 5, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 1, 229, 122, 170);
        canvas -> SetPixel (x_origin + 9, y_origin + 1, 229, 122, 170);
        canvas -> SetPixel (x_origin + 10, y_origin + 1, 229, 122, 170);
        canvas -> SetPixel (x_origin + 11, y_origin + 1, 229, 122, 170);
        canvas -> SetPixel (x_origin + 12, y_origin + 1, 229, 122, 170);

        canvas -> SetPixel (x_origin + 3, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 2, 229, 122, 170);
        canvas -> SetPixel (x_origin + 12, y_origin + 2, 229, 122, 170);
        canvas -> SetPixel (x_origin + 13, y_origin + 2, 229, 122, 170);

        canvas -> SetPixel (x_origin + 2, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 3, 229, 122, 170);
        canvas -> SetPixel (x_origin + 13, y_origin + 3, 229, 122, 170);

        canvas -> SetPixel (x_origin + 1, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 4, 180, 180, 180);

        canvas -> SetPixel (x_origin + 0, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 1, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 5, 180, 180, 180);

        canvas -> SetPixel (x_origin + 1, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 6, 180, 180, 180);

        canvas -> SetPixel (x_origin + 3, y_origin + 8, 0, 183, 239);
        canvas -> SetPixel (x_origin + 6, y_origin + 8, 0, 183, 239);
        canvas -> SetPixel (x_origin + 9, y_origin + 8, 0, 183, 239);
        canvas -> SetPixel (x_origin + 12, y_origin + 8, 0, 183, 239);

        canvas -> SetPixel (x_origin + 2, y_origin + 9, 0, 183, 239);
        canvas -> SetPixel (x_origin + 5, y_origin + 9, 0, 183, 239);
        canvas -> SetPixel (x_origin + 8, y_origin + 9, 0, 183, 239);
        canvas -> SetPixel (x_origin + 11, y_origin + 9, 0, 183, 239);

        canvas -> SetPixel (x_origin + 1, y_origin + 10, 0, 183, 239);
        canvas -> SetPixel (x_origin + 4, y_origin + 10, 0, 183, 239);
        canvas -> SetPixel (x_origin + 7, y_origin + 10, 0, 183, 239);
        canvas -> SetPixel (x_origin + 10, y_origin + 10, 0, 183, 239);
    }
    else if (condition == "thunderstorm")
    {
        canvas -> SetPixel (x_origin + 8, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 0, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 0, 180, 180, 180);

        canvas -> SetPixel (x_origin + 7, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 1, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 1, 180, 180, 180);

        canvas -> SetPixel (x_origin + 5, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 8, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 9, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 10, y_origin + 2, 255, 255, 255);
        canvas -> SetPixel (x_origin + 11, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 2, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 2, 180, 180, 180);

        canvas -> SetPixel (x_origin + 3, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 3, 255, 255, 255);
        canvas -> SetPixel (x_origin + 12, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 3, 180, 180, 180);
        canvas -> SetPixel (x_origin + 16, y_origin + 3, 180, 180, 180);

        canvas -> SetPixel (x_origin + 2, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 13, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 14, y_origin + 4, 255, 255, 255);
        canvas -> SetPixel (x_origin + 15, y_origin + 4, 180, 180, 180);
        canvas -> SetPixel (x_origin + 16, y_origin + 4, 180, 180, 180);

        canvas -> SetPixel (x_origin + 1, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 5, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 5, 255, 255, 255);
        canvas -> SetPixel (x_origin + 16, y_origin + 5, 180, 180, 180);

        canvas -> SetPixel (x_origin + 0, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 1, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 6, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 7, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 11, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 12, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 6, 180, 180, 180);
        canvas -> SetPixel (x_origin + 15, y_origin + 6, 180, 180, 180);

        canvas -> SetPixel (x_origin + 1, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 2, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 3, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 4, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 5, y_origin + 7, 255, 250, 189);
        canvas -> SetPixel (x_origin + 6, y_origin + 7, 255, 250, 189);
        canvas -> SetPixel (x_origin + 7, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 8, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 9, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 10, y_origin + 7, 255, 250, 189);
        canvas -> SetPixel (x_origin + 11, y_origin + 7, 255, 250, 189);
        canvas -> SetPixel (x_origin + 12, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 13, y_origin + 7, 180, 180, 180);
        canvas -> SetPixel (x_origin + 14, y_origin + 7, 180, 180, 180);

        canvas -> SetPixel (x_origin + 4, y_origin + 8, 255, 250, 189);
        canvas -> SetPixel (x_origin + 5, y_origin + 8, 255, 250, 189);
        canvas -> SetPixel (x_origin + 6, y_origin + 8, 255, 250, 189);
        canvas -> SetPixel (x_origin + 9, y_origin + 8, 255, 250, 189);
        canvas -> SetPixel (x_origin + 10, y_origin + 8, 255, 250, 189);
        canvas -> SetPixel (x_origin + 11, y_origin + 8, 255, 250, 189);

        canvas -> SetPixel (x_origin + 4, y_origin + 9, 255, 250, 189);
        canvas -> SetPixel (x_origin + 5, y_origin + 9, 255, 250, 189);
        canvas -> SetPixel (x_origin + 8, y_origin + 9, 255, 250, 189);
        canvas -> SetPixel (x_origin + 9, y_origin + 9, 255, 250, 189);
        canvas -> SetPixel (x_origin + 10, y_origin + 9, 255, 250, 189);

        canvas -> SetPixel (x_origin + 3, y_origin + 10, 255, 250, 189);
        canvas -> SetPixel (x_origin + 4, y_origin + 10, 255, 250, 189);
        canvas -> SetPixel (x_origin + 5, y_origin + 10, 255, 250, 189);
        canvas -> SetPixel (x_origin + 7, y_origin + 10, 255, 250, 189);
        canvas -> SetPixel (x_origin + 8, y_origin + 10, 255, 250, 189);
        canvas -> SetPixel (x_origin + 9, y_origin + 10, 255, 250, 189);

        canvas -> SetPixel (x_origin + 2, y_origin + 11, 255, 250, 189);
        canvas -> SetPixel (x_origin + 3, y_origin + 11, 255, 250, 189);
        canvas -> SetPixel (x_origin + 4, y_origin + 11, 255, 250, 189);
        canvas -> SetPixel (x_origin + 7, y_origin + 11, 255, 250, 189);
        canvas -> SetPixel (x_origin + 8, y_origin + 11, 255, 250, 189);

        canvas -> SetPixel (x_origin + 2, y_origin + 12, 255, 250, 189);
        canvas -> SetPixel (x_origin + 3, y_origin + 12, 255, 250, 189);
    }
    else if (condition == "snow")
    {
        canvas -> SetPixel (x_origin + 6, y_origin + 0, 153, 217, 234);

        canvas -> SetPixel (x_origin + 4, y_origin + 1, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 1, 153, 217, 234);
        canvas -> SetPixel (x_origin + 8, y_origin + 1, 153, 217, 234);

        canvas -> SetPixel (x_origin + 2, y_origin + 2, 153, 217, 234);
        canvas -> SetPixel (x_origin + 5, y_origin + 2, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 2, 153, 217, 234);
        canvas -> SetPixel (x_origin + 7, y_origin + 2, 153, 217, 234);
        canvas -> SetPixel (x_origin + 9, y_origin + 2, 153, 217, 234);

        canvas -> SetPixel (x_origin + 3, y_origin + 3, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 3, 153, 217, 234);
        canvas -> SetPixel (x_origin + 9, y_origin + 3, 153, 217, 234);

        canvas -> SetPixel (x_origin + 1, y_origin + 4, 153, 217, 234);
        canvas -> SetPixel (x_origin + 4, y_origin + 4, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 4, 153, 217, 234);
        canvas -> SetPixel (x_origin + 8, y_origin + 4, 153, 217, 234);
        canvas -> SetPixel (x_origin + 11, y_origin + 4, 153, 217, 234);

        canvas -> SetPixel (x_origin + 2, y_origin + 5, 153, 217, 234);
        canvas -> SetPixel (x_origin + 5, y_origin + 5, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 5, 153, 217, 234);
        canvas -> SetPixel (x_origin + 7, y_origin + 5, 153, 217, 234);
        canvas -> SetPixel (x_origin + 10, y_origin + 5, 153, 217, 234);

        canvas -> SetPixel (x_origin + 0, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 1, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 2, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 3, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 4, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 5, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 7, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 8, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 9, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 10, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 11, y_origin + 6, 153, 217, 234);
        canvas -> SetPixel (x_origin + 12, y_origin + 6, 153, 217, 234);

        canvas -> SetPixel (x_origin + 2, y_origin + 7, 153, 217, 234);
        canvas -> SetPixel (x_origin + 5, y_origin + 7, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 7, 153, 217, 234);
        canvas -> SetPixel (x_origin + 7, y_origin + 7, 153, 217, 234);
        canvas -> SetPixel (x_origin + 10, y_origin + 7, 153, 217, 234);

        canvas -> SetPixel (x_origin + 1, y_origin + 8, 153, 217, 234);
        canvas -> SetPixel (x_origin + 4, y_origin + 8, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 8, 153, 217, 234);
        canvas -> SetPixel (x_origin + 8, y_origin + 8, 153, 217, 234);
        canvas -> SetPixel (x_origin + 11, y_origin + 8, 153, 217, 234);

        canvas -> SetPixel (x_origin + 3, y_origin + 9, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 9, 153, 217, 234);
        canvas -> SetPixel (x_origin + 9, y_origin + 9, 153, 217, 234);

        canvas -> SetPixel (x_origin + 2, y_origin + 10, 153, 217, 234);
        canvas -> SetPixel (x_origin + 5, y_origin + 10, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 10, 153, 217, 234);
        canvas -> SetPixel (x_origin + 7, y_origin + 10, 153, 217, 234);
        canvas -> SetPixel (x_origin + 9, y_origin + 10, 153, 217, 234);

        canvas -> SetPixel (x_origin + 4, y_origin + 11, 153, 217, 234);
        canvas -> SetPixel (x_origin + 6, y_origin + 11, 153, 217, 234);
        canvas -> SetPixel (x_origin + 8, y_origin + 11, 153, 217, 234);

        canvas -> SetPixel (x_origin + 6, y_origin + 12, 153, 217, 234);
    }
    else // Mist
    {
        for (int i = 4; i < 10; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 0, 180, 180, 180);

        for (int i = 0; i < 12; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 2, 180, 180, 180);

        for (int i = 4; i < 15; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 4, 180, 180, 180);

        for (int i = 0; i < 11; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 6, 180, 180, 180);

        for (int i = 2; i < 13; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 8, 180, 180, 180);

        for (int i = 3; i < 11; i++)
            canvas -> SetPixel (x_origin + i, y_origin + 10, 180, 180, 180);
    }
    return;
}
