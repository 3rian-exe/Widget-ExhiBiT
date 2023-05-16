#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    char cmd[] = "python3 store_current_weather_data.py";
    cout << "Running get_data.py...\n\n";

    int checkerror = system (cmd);

    if (checkerror == -1)
        cout << "execv error\n";

    string fileName = "weather_data.txt";
    ifstream file (fileName);

    string temp, humidity, windSpeed;
    file >> temp
        >> humidity
        >> windSpeed;
    
    cout << "\ntemp: " << temp << endl
        << "humidity: " << humidity << endl
        << "wind speed: " << windSpeed << endl;

    file.close();

    return 0;
}
