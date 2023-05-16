from rgbmatrix import graphics, RGBMatrix, RGBMatrixOptions
import time

# Set up the RGBMatrixOptions object with your desired matrix configuration
options = RGBMatrixOptions()
options.rows = 64
options.cols = 64
options.chain_length = 2
options.parallel = 1
options.hardware_mapping = 'adafruit-hat-pwm'
options.brightness = 25

# Create a RGBMatrix object with the given options
matrix = RGBMatrix(options = options)

# Create a font to use for drawing text
font = graphics.Font()
font.LoadFont("/home/pi/Desktop/rpi-rgb-led-matrix/fonts/5x8.bdf")
char_width = 5
# Define some colors
white = graphics.Color(255, 255, 255)

# Define the text to scroll
text = "A very loooooooooooooooooooooooooong song title :)"


#def scroll_text(matrix, text, color, font_path, x_origin, y_origin)


# Scroll the text horizontally across the matrix display
start_scroll = 127
pos = start_scroll
y_border = 2
while True:
    # Clear the matrix display
    matrix.Clear()

    # Draw the scrolling text
    graphics.DrawText(matrix, font, pos, 9, white, text)
    matrix.SetPixel(64, y_border + 0, 0, 0, 0)
    matrix.SetPixel(65, y_border + 0, 0, 0, 0)
    matrix.SetPixel(64, y_border + 1, 0, 0, 0)
    matrix.SetPixel(65, y_border + 1, 0, 0, 0)
    matrix.SetPixel(64, y_border + 2, 0, 0, 0)
    matrix.SetPixel(65, y_border + 2, 0, 0, 0)
    matrix.SetPixel(64, y_border + 3, 0, 0, 0)
    matrix.SetPixel(65, y_border + 3, 0, 0, 0)
    matrix.SetPixel(64, y_border + 4, 0, 0, 0)
    matrix.SetPixel(65, y_border + 4, 0, 0, 0)
    matrix.SetPixel(64, y_border + 5, 0, 0, 0)
    matrix.SetPixel(65, y_border + 5, 0, 0, 0)
    matrix.SetPixel(64, y_border + 6, 0, 0, 0)
    matrix.SetPixel(65, y_border + 6, 0, 0, 0)
    matrix.SetPixel(64, y_border + 7, 0, 0, 0)
    matrix.SetPixel(65, y_border + 7, 0, 0, 0)
    matrix.SetPixel(64, y_border + 8, 0, 0, 0)
    matrix.SetPixel(65, y_border + 8, 0, 0, 0)
    matrix.SetPixel(64, y_border + 9, 0, 0, 0)
    matrix.SetPixel(65, y_border + 9, 0, 0, 0)

    matrix.SetPixel(126, y_border + 0, 0, 0, 0)
    matrix.SetPixel(127, y_border + 0, 0, 0, 0)
    matrix.SetPixel(126, y_border + 1, 0, 0, 0)
    matrix.SetPixel(127, y_border + 1, 0, 0, 0)
    matrix.SetPixel(126, y_border + 2, 0, 0, 0)
    matrix.SetPixel(127, y_border + 2, 0, 0, 0)
    matrix.SetPixel(126, y_border + 3, 0, 0, 0)
    matrix.SetPixel(127, y_border + 3, 0, 0, 0)
    matrix.SetPixel(126, y_border + 4, 0, 0, 0)
    matrix.SetPixel(127, y_border + 4, 0, 0, 0)
    matrix.SetPixel(126, y_border + 5, 0, 0, 0)
    matrix.SetPixel(127, y_border + 5, 0, 0, 0)
    matrix.SetPixel(126, y_border + 6, 0, 0, 0)
    matrix.SetPixel(127, y_border + 6, 0, 0, 0)
    matrix.SetPixel(126, y_border + 7, 0, 0, 0)
    matrix.SetPixel(127, y_border + 7, 0, 0, 0)
    matrix.SetPixel(126, y_border + 8, 0, 0, 0)
    matrix.SetPixel(127, y_border + 8, 0, 0, 0)
    matrix.SetPixel(126, y_border + 9, 0, 0, 0)
    matrix.SetPixel(127, y_border + 9, 0, 0, 0)

    # Decrement the horizontal position
    pos -= 1
    
    # If the entire text has scrolled off the left edge of the matrix
    # Once the last part of the text is visible pause for 0.5 seconds
    # and then restart the scroll.
    if pos + len(text) * char_width < 126:
        time.sleep(0.5)
        # Reset the horizontal position to the right edge of the matrix
        pos = start_scroll
    
    # Wait for a short period of time to slow down the scrolling speed
    time.sleep(0.05)
