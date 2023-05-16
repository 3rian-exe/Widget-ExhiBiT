import spotipy 
import spotipy.util as util
from spotipy.oauth2 import SpotifyOAuth

username = ""
# token_path = access_token for now
token_path = ""


scope = 'user-read-currently-playing'

#auth = SpotifyOAuth(scope = scope, open_browser = False)
#token = auth.get_access_token()

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
            print(song)
            print(artists)
            return [song, imageURL]
    else:
        print("Can't get token for", username)
        return None
    
#***************************************************************

import sys
import time


prevSong = ""
currentSong = ""
try:
    while True:
        try:
            imageURL = getSongInfo(username, token_path)[1]
            currentSong = imageURL

            if ( prevSong != currentSong ):
                #response = requests.get(imageURL)
                
                data = requests.get(imageURL).content      

                #image = Image.open(BytesIO(response.content))
                #data = image.thumbnail((64, 64), Image.ANTIALIAS)
                #data = image.thumbnail((64, 64))

                # Opening a new file named img with extension .jpg
                # This file would store the data of the image file
                f = open('img.jpg','wb')

                # Storing the image data inside the data variable to the file
                f.write(data)
                f.close()

                # Opening the saved image and displaying it
                img = Image.open('img.jpg')
                img.show()


                #matrix.SetImage(image.convert('RGB'))
                prevSong = currentSong

            time.sleep(1)
        except Exception as e:
            image = Image.open("whichway.png")
            #image.thumbnail((64, 64), Image.ANTIALIAS)
            data = image.thumbnail((64, 64))
            image.show()
            #matrix.SetImage(image.convert('RGB'))
            print(e)
            time.sleep(10)
except KeyboardInterrupt:
    sys.exit(0)
    

