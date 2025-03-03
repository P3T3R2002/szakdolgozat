import urllib.request
import time
import csv
import os


staticIP = "192.168.0.200"
port = "8080"

url = 'http://' + staticIP + ":" + port

def index():
    return "Hello there!"

def get_data(data):
    contents = urllib.request.urlopen(url + data).read()
    return str(contents).split("'")[1]

def main():

    file_path = "/mnt/d/Desktop/Peti/egyetem/Szakdolgozat/wind.csv"

    ROOT_DIR = os.path.abspath(os.curdir)
    CONFIG_PATH = os.path.join(ROOT_DIR, file_path) 

    csvfile = open(CONFIG_PATH, "w", newline="")

    try:
        while True:
            current_time = int(time.time()*1000)
            if current_time % 1000 == 0:
                angle = get_data("/windangle")
                writer = csv.writer(csvfile)
                writer.writerow(['Windangle', angle, time.ctime()])
                print(f"Time: --> {time.ctime()}")
                print(f"Windangle: {angle}")
                print("------------------------------------------")
                    
            if current_time/10 % 1000 == 0:
                speed = get_data("/windspeed")
                writer = csv.writer(csvfile)
                writer.writerow(['Windspeed', speed, time.ctime()])
                print(f"Time: --> {time.ctime()}")
                print(f"Windspeed: {speed}")
                print("------------------------------------------")
    except Exception as e:
        print(f"Error encountered: {e}")

    csvfile.close()
if __name__ == "__main__":
    main()

