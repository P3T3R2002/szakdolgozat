import urllib.request
import datetime
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
    file_path = "/mnt/d/Desktop/Peti/egyetem/Szakdolgozat/"

    while True:

        date = datetime.datetime.now()

        name_speed = f"WINDSPEED_{date.year}_{date.month}_{date.day}.csv"
        name_angle = f"WINDANGLE_{date.year}_{date.month}_{date.day}.csv"
        day = date.day
        current_day = day

        ROOT_DIR = os.path.abspath(os.curdir)
        SPEED_PATH = os.path.join(ROOT_DIR, file_path+name_speed)
        ANGLE_PATH = os.path.join(ROOT_DIR, file_path+name_angle) 

        speed_csv = open(SPEED_PATH, "w", newline="")
        speed_writer = csv.writer(speed_csv)
        speed_writer.writerow(["SPEED", "TIME"])

        angle_csv = open(ANGLE_PATH, "w", newline="")
        angle_writer = csv.writer(angle_csv)
        angle_writer.writerow(["ANGLE", "TIME"])


        try:
            while day == current_day:
                current_time = datetime.datetime.now()
                if int(current_time.microsecond/1000) % 1000 == 0:
                    angle = get_data("/windangle")
                    angle_writer.writerow([angle, int(time.time()*10)/10])

                if int(current_time.microsecond/1000) % 500 == 0:
                    speed = get_data("/windspeed")
                    speed_writer.writerow([speed, int(time.time()*10)/10])

                current_day = current_time.day

        except Exception as e:
            print(f"Error encountered: {e}")

        speed_csv.close()
        angle_csv.close()

if __name__ == "__main__":
    main()

