from rgbmatrix import RGBMatrix, RGBMatrixOptions
import time
import math

def song_progress_bar(song_len, current_progress, x_origin, y_origin):
    progress_bar_pixel_len = 60

    progress_bar_step = song_len / progress_bar_pixel_len
    progress_bar_status = math.ceil(current_progress / progress_bar_step)

    # what out of 56 are we up to ^



    #spotify_green = 30, 215, 96
    # grey = 180, 180, 180


    for grey_border in range(progress_bar_pixel_len):  
        matrix.SetPixel(x_origin + grey_border, y_origin + 1, 180, 180, 180)
        matrix.SetPixel(x_origin + grey_border, y_origin - 1, 180, 180, 180)

    for i in range(progress_bar_status):
        matrix.SetPixel(x_origin + i, y_origin, 30, 215, 96)





# Configuration for the matrix
options = RGBMatrixOptions()
options.rows = 64
options.cols = 64
options.chain_length = 2
options.parallel = 1
options.brightness = 25
options.hardware_mapping = 'adafruit-hat-pwm'  # If you have an Adafruit HAT: 'adafruit-hat'

matrix = RGBMatrix(options = options)


current_progress = 0
length = 10
increment = 1

while current_progress <= length:
    song_progress_bar(length, current_progress, 66, 46)
    time.sleep(increment)
    current_progress += increment

matrix.Clear()

