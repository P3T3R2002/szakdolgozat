import urllib.request
import datetime
import time
import json
import csv
import os


STATIC_IP = "192.168.0.200"
PORT = "8080"
FILE_PATH = "/mnt/d/Desktop/Peti/egyetem/Szakdolgozat/"

URL = 'http://' + STATIC_IP + ":" + PORT

def index():
    return "Hello there!"

def get_data():
    contents = urllib.request.urlopen(URL).read()
    return contents

def get_date():
    contents = urllib.request.urlopen(URL + "/date").read()
    return contents

def log_error(LOG_txt, e):
    LOG_txt.write(f"{datetime.datetime.now()}: {e}\n")


def main():

    ROOT_DIR = os.path.abspath(os.curdir)
    LOG_PATH = os.path.join(ROOT_DIR, FILE_PATH + 'wind_data_error_log.txt')

    LOG_txt = open(LOG_PATH, "w", newline="")
    log_error(LOG_txt, "Logging start...")


    try:
        reference_date = time.time()-json.loads(get_date())["date"]/1000
    except Exception as e:
        log_error(LOG_txt, e)

    while True:
        try:
            date = datetime.datetime.now()

            name_speed = f"WINDSDATA_{date.year}_{date.month}_{date.day}.csv"
            day = date.day
            current_day = day

            SPEED_PATH = os.path.join(ROOT_DIR, FILE_PATH+name_speed)

            DATA_csv = open(SPEED_PATH, "w", newline="")
            DATA_writer = csv.writer(DATA_csv)
            DATA_writer.writerow(["SPEED", "ANGLE", "DATE"])
        except Exception as e:
            log_error(LOG_txt, e)

        while day == current_day:
            try:
                current_time = datetime.datetime.now()
                data = json.loads(get_data())
                data["date"] = reference_date + data["date"]/1000
                DATA_writer.writerow(data.values())

            except Exception as e:
                log_error(LOG_txt, e)

            current_day = current_time.day

        print("close")
        DATA_csv.close()

if __name__ == "__main__":
    main()

