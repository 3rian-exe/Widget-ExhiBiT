import spotipy 
import spotipy.util as util
from spotipy.oauth2 import SpotifyOAuth

from rgbmatrix import graphics, RGBMatrix, RGBMatrixOptions

username = ""
# token_path = access_token for now
token_path = ""
scope = 'user-read-currently-playing'

#***************************************************************

import requests
from io import BytesIO
from PIL import Image

def getSongInfo(username, token_path):
    scope = 'user-read-currently-playing'
    #token = util.prompt_for_user_token(username, scope, cache_path=token_path)
    token = token_path
    #print("hello")
    print('\n')
    if token:
        sp = spotipy.Spotify(auth=token)
        result = sp.current_user_playing_track()
    
        if result is None:
            print("No song playing")
        else:  
            song = result["item"]["name"]
            artists = ""
            if len(result["item"]["artists"]) > 1:
                size = len(result["item"]["artists"])
                for artist in range(size):
                    if artist < size - 1:
                        artists += result["item"]["artists"][artist]["name"] + ", "
                    else:
                        artists += result["item"]["artists"][artist]["name"]
                        
                    #artists.append(result["item"]["artists"][artist]["name"])
            else:
                artists = result["item"]["artists"][0]["name"]
            imageURL = result["item"]["album"]["images"][0]["url"]
            return [song, artists, imageURL]
    else:
        print("Can't get token for", username)
        return None
    
#***************************************************************

import sys
import time


# Configuration for the matrix
options = RGBMatrixOptions()
options.rows = 64
options.cols = 64
options.chain_length = 2
options.parallel = 1
options.hardware_mapping = 'adafruit-hat-pwm'

font = graphics.Font()
font.LoadFont("/home/pi/Desktop/rpi-rgb-led-matrix/fonts/5x8.bdf")

# Define some colors
red = graphics.Color(255, 0, 0)
green = graphics.Color(0, 255, 0)
blue = graphics.Color(0, 0, 255)
white = graphics.Color(255, 255, 255)

matrix = RGBMatrix(options = options)

prevSong = ""
currentSong = ""
try:
    while True:
        try:
            imageURL = getSongInfo(username, token_path)[2]
            currentSong = imageURL
            artist = getSongInfo(username, token_path)[1]
            song = getSongInfo(username, token_path)[0]

            if ( prevSong != currentSong ):                
                data = requests.get(imageURL).content      
                f = open('img.jpg','wb')

                # Storing the image data inside the data variable to the file
                f.write(data)
                f.close()

                # Opening the saved image and displaying it
                img = Image.open(r'img.jpg')
                img = img.resize((64, 64))
                img = img.convert('RGB')
                #img.show()
                
                matrix.Clear()
                matrix.SetImage(img, 0, 0)
                graphics.DrawText(matrix, font, 66, 40, white, song)
                graphics.DrawText(matrix, font, 66, 50, white, artist)

                #matrix.SetImage(image.convert('RGB'))
                prevSong = currentSong



            time.sleep(1)
        except Exception as e:
            graphics.DrawText(matrix, font, 66, 40, white, "Error")
            image = Image.open("whichway.png")
            #image.thumbnail((64, 64), Image.ANTIALIAS)
            #data = image.thumbnail((64, 64))
            #image.show()
            #matrix.SetImage(image.convert('RGB'))
            print(e)
            time.sleep(5)
except KeyboardInterrupt:
    sys.exit(0)

matrix.Clear()

    
