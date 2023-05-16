import requests
import json
import math


file_name = "weather_data.txt"
mode = "w+"
weather_data = open(file_name, mode)
zip_code = ""

# OpenWeatherMap API key
api_key = ""

# Latitude and longitude get weather for
lat = "40.622164"
lon = "-73.965105"

# URL for OpenWeatherMap API call
url = f"https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={api_key}"

# Make API call and get response
response = requests.get(url)

print("Status: ", response.status_code)

# Convert response to JSON format
data = json.loads(response.text)

# Get current weather conditions
weather = data["weather"][0]["description"]

temperature = data["main"]["temp"]
humidity = data["main"]["humidity"]
wind_speed = math.ceil(data["wind"]["speed"])

# Convert the temperature from kelvin to fahrenheit.
temperature_fahrenheit = math.floor((temperature - 273.15) * (9/5) + 32)

temperature = str(temperature_fahrenheit) + '\n'
humidity = str(humidity) + '\n'
wind_speed = str(wind_speed) + '\n'

# weather_data.write(f"Current weather in {zip_code}: {weather}\n")
weather_data.write(temperature)
weather_data.write(humidity)
weather_data.write(wind_speed)

weather_data.close()