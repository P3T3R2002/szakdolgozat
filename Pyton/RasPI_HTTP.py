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
    file_path = "/mnt/d/Desktop/Peti/egyetem/Szakdolgozat/"

    while True:

        date = time.ctime().split(" ")
        name_speed = f"WINDSPEED_{date[5]}_{date[1]}_{date[3]}.csv"
        name_angle = f"WINDANGLE_{date[5]}_{date[1]}_{date[3]}.csv"
        day = date[3]
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
                current_time = int(time.time()*1000)
                if current_time % 1000 == 0:
                    angle = get_data("/windangle")
                    angle_writer.writerow([angle, time.ctime()])
                    print(f"Time: --> {time.ctime()}")
                    print(f"Windangle: {angle}")
                    print("------------------------------------------")
                        
                if current_time/10 % 1000 == 0:
                    speed = get_data("/windspeed")
                    speed_writer = csv.writer(speed_csv)
                    speed_writer.writerow([speed, time.ctime()])
                    print(f"Time: --> {time.ctime()}")
                    print(f"Windspeed: {speed}")
                    print("------------------------------------------")

                current_day = time.ctime().split(" ")[3]

        except Exception as e:
            print(f"Error encountered: {e}")

        speed_csv.close()
        angle_csv.close()
        
if __name__ == "__main__":
    main()

