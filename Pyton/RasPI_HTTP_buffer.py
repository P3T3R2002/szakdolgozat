import urllib.request
import datetime
import time
import json
import csv
import os


staticIP = "192.168.0.200"
port = "8080"

url = 'http://' + staticIP + ":" + port

def index():
    return "Hello there!"

def get_data():
    contents = urllib.request.urlopen(url).read()
    return contents

def main():
    file_path = "/mnt/d/Desktop/Peti/egyetem/Szakdolgozat/"

    while True:

        date = datetime.datetime.now()

        name_speed = f"WINDSDATA_{date.year}_{date.month}_{date.day}.csv"
        day = date.day
        current_day = day

        ROOT_DIR = os.path.abspath(os.curdir)
        SPEED_PATH = os.path.join(ROOT_DIR, file_path+name_speed)

        DATA_csv = open(SPEED_PATH, "w", newline="")
        DATA_writer = csv.writer(DATA_csv)
        DATA_writer.writerow(["SPEED", "ANGLE", "DATE"])


        while day == current_day:
            try:
                current_time = datetime.datetime.now()
                data = json.loads(get_data())
                DATA_writer.writerow(data.values())
                current_day = current_time.day

            except Exception as e:
                print(f"Error encountered: {e}")
        print("close")
        DATA_csv.close()

if __name__ == "__main__":
    main()

