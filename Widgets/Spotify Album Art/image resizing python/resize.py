from PIL import Image

# Open the image
image = Image.open('illusory_tracks.jpeg')

# Resize the image to a specific resolution
resized_image = image.resize((32, 32))

# Save the resized image
resized_image.save('resized_image.jpg')