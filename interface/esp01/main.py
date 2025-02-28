import requests
import time
import json
import pandas as pd

url = "http://192.168.4.1/" 
# url = "http://192.168.10.161/" 
# url = "http://lidar.local/"

def rq_get(path=None, output=False):
    print("Get request for: ", path)
    full_url = url + "/" + path if path else url
    r = requests.get(full_url)
    print(r.text)
    print(r.status_code)
    time.sleep(0.5)
    return r.json() if output else None

def rq_post(path=None, json=None):
    print("Post request for: ", path)
    full_url = url + "/" + path if path else url
    r = requests.post(full_url, json=json, timeout=2)
    print(r.text)
    print(r.status_code)
    time.sleep(0.5)



try:
    # print("Longitud de data: ", len(rq_get("sensor", output=True).get("distance")))
    rq_get()
    rq_post("idle")
    rq_get("buffer")
    rq_post("active")
    rq_get("buffer")
    # for _ in range(10):
    #     time.sleep(2)
    #     print("Longitud de data: ", len(rq_get("sensor", output=True).get("distance")))
    out = input("Press enter to stop")
    rq_post("idle") 
except KeyboardInterrupt:
    print("Exiting...")
    rq_post("idle")
    rq_get("buffer")


# rq_post("msg", {"message": "M5.\n"})
# rq_get("buffer")
# rq_post("msg", {"message": "STATUS\n"})
# rq_post("idle")
# rq_get("buffer")


df = pd.DataFrame(rq_get("sensor", output=True))
df.to_csv("output.csv", index=False)
# with open("output.json", "w") as fp:
#     data = rq_get("sensor", output=True)
#     json.dump(data, fp, indent=4)