import requests
import time

# url = "http://192.168.4.1/" 
url = "http://lidar.local/" 

def rq_get(path=None):
    print("Get request for: ", path)
    full_url = url + "/" + path if path else url
    r = requests.get(full_url)
    print(r.text)
    print(r.status_code)
    time.sleep(0.5)

def rq_post(path=None, json=None):
    print("Post request for: ", path)
    full_url = url + "/" + path if path else url
    r = requests.post(full_url, json=json, timeout=2)
    print(r.text)
    print(r.status_code)
    time.sleep(0.5)

rq_get()
rq_get("buffer")
rq_post("active")
# rq_post("idle")
# rq_post("msg", {"message": "STATUS\n"})
# rq_get("buffer")
rq_get("sensor")
