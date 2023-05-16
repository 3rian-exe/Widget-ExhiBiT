import requests
import json
import math


# api key
api_key = ""

# latitude and longitude get weather for 11230
lat = "40.622164"
lon = "-73.965105"
zip_code = ""
mode = "w"
current_waether_file = "current_weather_data.txt"
current_weather_data = open(current_waether_file, mode)

# url for the open weather map api call for the current weather
current_weather_url = f"https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={api_key}"

# api call
response_current = requests.get(current_weather_url)

# checking if the api call was successful 
print("Status: ", response_current.status_code)

# convert the response to json format
current_data = json.loads(response_current.text)

# Convert the temperature from kelvin to fahrenheit.
temperature = str(math.floor((current_data["main"]["temp"] - 273.15) * (9/5) + 32)) + '\n'
conditions = current_data["weather"][0]["description"].replace(" ", "") + '\n'
humidity = str(current_data["main"]["humidity"]) + '\n'
wind_speed = str(math.ceil(current_data["wind"]["speed"])) + '\n'

# weather_data.write(f"Current weather in {zip_code}: {weather}\n")
current_weather_data.write(temperature)
current_weather_data.write(conditions)
current_weather_data.write(humidity)
current_weather_data.write(wind_speed)

current_weather_data.close()

# retrieve and store the hourly forecast
hourly_weather_file = "hourly_weather_data.txt"
hourly_weather_data = open(hourly_weather_file, mode)

# url for the open weather map api call for the hourly forecast 
hourly_weather_url = f"https://api.openweathermap.org/data/2.5/forecast?lat={lat}&lon={lon}&appid={api_key}"
response_hourly = requests.get(hourly_weather_url)
hourly_data = json.loads(response_hourly.text)
hourly_forecast = hourly_data["list"]

for i in range(5):
    time = hourly_forecast[i]["dt_txt"]
    if int(time[10:13]) > 12:
        time = str(int(time[10:13]) - 12) + 'PM'
    elif int(time[10:13]) == 12:
        time = str(int(time[10:13])) + 'PM'
    elif int(time[10:13]) == 0:
        time = "12AM"
    else:
        time = str(int(time[10:13])) + 'AM'
    temperature = str(math.floor((hourly_forecast[i]["main"]["temp"] - 273.15) * (9/5) + 32))
    weather = hourly_forecast[i]["weather"][0]["description"]
    weather = weather.replace(" ", "")
    hourly_weather_data.write(f"{time} {temperature} {weather}\n")

hourly_weather_data.close()
    


    