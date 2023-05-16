#include <iostream>
#include <fstream>

using namespace std;

int main (int argc, char * argv[])
{
    string fileName = "zip2latlon.txt", a, b, c, d, e, g;
    fstream file (fileName);
    while (file)
    {
        file >> a;
        cout << a << endl;
    }


   /*  if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
            cout << argv[i] << endl;
    }
    else
        cout << "Hello World!" << endl; */
    return 0;
}