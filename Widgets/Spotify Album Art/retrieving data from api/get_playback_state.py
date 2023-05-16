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
            song_duration = result["item"]["duration_ms"] / 1000 
            is_playing = result["is_playing"]
            song_progress = result["progress_ms"] / 1000
            song = result["item"]["name"]
            imageURL = result["item"]["album"]["images"][0]["url"]
            return [imageURL, song, song_duration, song_progress, is_playing]
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
            imageURL = getSongInfo(username, token_path)[0]
            song = getSongInfo(username, token_path)[1]
            songduration = getSongInfo(username, token_path)[2]
            songprogress = getSongInfo(username, token_path)[3]
            isplaying = getSongInfo(username, token_path)[4]
            currentSong = imageURL

            if ( prevSong != currentSong ):            
                
                print(song, songduration, "in seconds")                
                prevSong = currentSong
            
            print(isplaying, songprogress)

            time.sleep(0.1)
        except Exception as e:
            #image = Image.open("whichway.png")
            #image.thumbnail((64, 64), Image.ANTIALIAS)
            #data = image.thumbnail((64, 64))
            #image.show()
            #matrix.SetImage(image.convert('RGB'))
            print(e)
            time.sleep(2)
except KeyboardInterrupt:
    sys.exit(0)
    

