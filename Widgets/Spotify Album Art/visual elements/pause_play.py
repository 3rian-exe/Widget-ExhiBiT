from rgbmatrix import RGBMatrix, RGBMatrixOptions
import time

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




# Configuration for the matrix
options = RGBMatrixOptions()
options.rows = 64
options.cols = 64
options.chain_length = 2
options.parallel = 1
options.brightness = 25
options.hardware_mapping = 'adafruit-hat-pwm'  # If you have an Adafruit HAT: 'adafruit-hat'

matrix = RGBMatrix(options = options)

matrix.Clear()



i = 15
while i > 0:
    pause_play_icon(matrix, 70, 10, True)
    pause_play_icon(matrix, 90, 10, False)
    time.sleep(2)
    i -= 1
matrix.Clear()