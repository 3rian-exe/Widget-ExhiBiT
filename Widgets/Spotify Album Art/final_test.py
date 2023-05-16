import spotipy 
import spotipy.util as util
from spotipy.oauth2 import SpotifyOAuth
import requests
from io import BytesIO
from PIL import Image
import sys
import time
import math

from rgbmatrix import graphics, RGBMatrix, RGBMatrixOptions

username = ""
# token_path = access_token for now
token_path = ""

scope = 'user-read-currently-playing'

#***************************************************************

# Draw the Pause icon when playing (True) and the play icon when paused (False)
def pause_play_icon(matrix, x_origin, y_origin, status):
    if status == True:
        for i in range(9):
            matrix.SetPixel(x_origin, y_origin + i, 255, 255, 255)
            matrix.SetPixel(x_origin + 1, y_origin + i, 255, 255, 255)

            matrix.SetPixel(x_origin + 5, y_origin + i, 255, 255, 255)
            matrix.SetPixel(x_origin + 6, y_origin + i, 255, 255, 255)
    else:
        for i in range(5):
            for j in range(i + 1):
                matrix.SetPixel(x_origin + j, y_origin + i, 255, 255, 255)
        for i in range(4):
            for j in range(4 - i, 0, -1):
                matrix.SetPixel(x_origin + j - 1, y_origin + 5 + i, 255, 255, 255) 


def song_progress_bar(matrix, song_len, current_progress, x_origin, y_origin):
    progress_bar_pixel_len = 60

    #spotify_green = 30, 215, 96
    # grey = 180, 180, 180
    # what out of 60 are we up to
    progress_bar_step = song_len / progress_bar_pixel_len
    progress_bar_status = math.ceil(current_progress / progress_bar_step)

    for grey_border in range(progress_bar_pixel_len):  
        matrix.SetPixel(x_origin + grey_border, y_origin + 1, 180, 180, 180)
        matrix.SetPixel(x_origin + grey_border, y_origin - 1, 180, 180, 180)

    for i in range(progress_bar_status):
        matrix.SetPixel(x_origin + i, y_origin, 30, 215, 96)

# Retrieve the song, the artist/s, album image, paused/playing status, 
# current track progress, and track length
def get_song_info(username, token_path):
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
            is_playing = result["is_playing"]
            song_duration = result["item"]["duration_ms"]
            song_progress = result["progress_ms"]            
            return [song, artists, imageURL, is_playing, song_duration, song_progress]
    else:
        print("Can't get token for", username)
        return None
    
#***************************************************************




# Configuration for the matrix
options = RGBMatrixOptions()
options.rows = 64
options.cols = 64
options.chain_length = 2
options.parallel = 1
options.hardware_mapping = 'adafruit-hat-pwm'
options.brightness = 25

song_font = graphics.Font()
song_font.LoadFont("/home/pi/Desktop/rpi-rgb-led-matrix/fonts/7x13B.bdf")

artists_font = graphics.Font()
artists_font.LoadFont("/home/pi/Desktop/rpi-rgb-led-matrix/fonts/6x10.bdf")

# Define some colors
red = graphics.Color(255, 0, 0)
green = graphics.Color(0, 255, 0)
blue = graphics.Color(0, 0, 255)
white = graphics.Color(255, 255, 255)

matrix = RGBMatrix(options = options)

prev_song = ""
current_song = ""

song_char_width = 7
artists_char_width = 6

start_scroll = 127
pos = start_scroll
pos2 = start_scroll 

y_border = 0

try:
    while True:
        try:
                        
            song, artists, image_url, play_pause_stat, song_duration, song_progress = get_song_info(username, token_path)
            current_song = image_url

            # Only process image to be displayed if the song changes. 
            if (prev_song != current_song):    
                responce = requests.get(image_url)
                image = Image.open(BytesIO(responce.content))
                image.thumbnail((64, 64), Image.ANTIALIAS)
                img = image.convert("RGB")
                prev_song = current_song

            # Display album art, song name, artists, paused status, and progress bar.

            matrix.Clear()

            if (len(song) > 8):
                if (pos + (len(song) * song_char_width) < 126):
                    pos = start_scroll
                graphics.DrawText(matrix, song_font, pos, 12, white, song)
            else:
                graphics.DrawText(matrix, song_font, 66, 12, white, song)
            
            if (len(artists) > 10):
                if (pos2 + (len(artists) * artists_char_width) < 126):
                    pos2 = start_scroll
                graphics.DrawText(matrix, artists_font, pos2, 24, white, artists)
            else:
                graphics.DrawText(matrix, artists_font, 66, 24, white, artists)

            # Padding between the scrolling text and the album art.
            for col in range(64):
                matrix.SetPixel(64, y_border + col, 0, 0, 0)
                matrix.SetPixel(65, y_border + col, 0, 0, 0)

            pos -= song_char_width
            pos2 -= artists_char_width


            pause_play_icon(matrix, 92, 49, play_pause_stat)
            matrix.SetImage(img, 0, 0)

            song_progress_bar(matrix, song_duration, song_progress, 66, 44)

            time.sleep(0.5)

        except Exception as e:
            graphics.DrawText(matrix, artists_font, 66, 40, white, "Error")
            image = Image.open("whichway.png")
            print(e)
            time.sleep(5)
except KeyboardInterrupt:
    sys.exit(0)

matrix.Clear()

    