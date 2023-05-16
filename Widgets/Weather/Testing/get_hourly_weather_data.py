import requests
import json

zip_code = ""

# OpenWeatherMap API key
api_key = ""

# Latitude and longitude get weather for
lat = "40.62275"
lon = "-73.96625"

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
wind_speed = data["wind"]["speed"]

temperature_fahrenheit = (temperature - 273.15) * (9/5) + 32

# Print current weather conditions
#print(f"Current weather in {zip_code}: {weather}")
#print(f"Temperature: {round(temperature_fahrenheit,2)} F")
#print(f"Humidity: {humidity}%")
#print(f"Wind speed: {wind_speed} mph")

# URL for hourly forecast API call
url_hourly = f"http://api.openweathermap.org/data/2.5/forecast?zip={zip_code},us&appid={api_key}&units=imperial"

# Make hourly forecast API call and get response
response_hourly = requests.get(url_hourly)

# Convert hourly forecast response to JSON format
data_hourly = json.loads(response_hourly.text)

# Get hourly forecast data
hourly_forecast = data_hourly["list"]



# Get the weather for the next 15 hours in 3 hour increments 
for i in range(7):
    time = hourly_forecast[i]["dt_txt"]
    temperature = hourly_forecast[i]["main"]["temp"]
    weather = hourly_forecast[i]["weather"][0]["description"]
    print(f"{time}: {temperature} F, {weather}")
