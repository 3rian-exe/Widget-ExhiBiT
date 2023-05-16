#include <iostream>
#include <vector>
// .h file

using namespace std;

struct my_type
{
    char letter;
    int letterSize;
};

vector <my_type>  a;



void drawTextScroll (string text, char textSize, int xOrigin, int yOrigin, 
                                                        int availableCols)
{
    int spacing = 0, textLength = text.length();
    if (textSize == 'S' || textSize == 's')
    {
        for (int smallStrIndex = 0; smallStrIndex < textLength; smallStrIndex++)
        {
            spacing += drawSmallChar (text[smallStrIndex], xOrigin + spacing, yOrigin);
        }
    }
    else if (textSize == 'M' || textSize == 'm')
    {
        for (int mediumStrIndex = 0; mediumStrIndex < textLength; mediumStrIndex++)
        {
            spacing += drawMedChar (text[mediumStrIndex], xOrigin + spacing, yOrigin);
        }
    }
    else
        return;
}




// Character by character.
// a b c d ....

// Column by column
// Pass the available columns

// a1 a2 a3 ... an b1 b2 b3 ... bn ........ 

