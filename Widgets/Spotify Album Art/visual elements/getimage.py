import requests
from PIL import Image

url = "https://i.scdn.co/image/ab67616d00001e02ff9ca10b55ce82ae553c8228"


data = requests.get(url).content

f = open('img.jpg','wb')

# Store the image data 
f.write(data)
f.close()

# Opening the image and displaying it
img = Image.open('img.jpg')
img.show()
