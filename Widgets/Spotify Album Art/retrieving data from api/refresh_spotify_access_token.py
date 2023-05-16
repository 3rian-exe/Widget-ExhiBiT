import base64
import requests
import json

client_id = ""
client_secret = ""
refresh_token = ""
base64_encoded = base64.b64encode((client_id + ':' + client_secret).encode('ascii')).decode('ascii')

headers = {
    'Authorization': 'Basic ' + base64_encoded
}

payload = {
    'grant_type': 'refresh_token',
    'refresh_token': refresh_token
}

response = requests.post('https://accounts.spotify.com/api/token', headers=headers, data=payload)

json_response = json.loads(response.text)
access_token = json_response['access_token']

print(access_token)
