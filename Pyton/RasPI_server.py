import os
import csv
import json
from datetime import datetime
from flask import Flask, jsonify, request

app = Flask(__name__)

FILE_PATH = "/mnt/d/Desktop/Peti/egyetem/Szakdolgozat/"

@app.route('/')
def hello_world():
    
    # Get the url queries parameters
    from_date_str = request.args.get("from_date")
    to_date_str = request.args.get("to_date")
    avg = int(request.args.get("avg"))
    
    from_date = datetime.strptime(from_date_str, "%Y-%m-%d")
    to_date = datetime.strptime(to_date_str, "%Y-%m-%d")

    # List all files in the directory
    files = os.listdir(FILE_PATH)

    # Filter the files to only those that match the pattern
    matching_files = []
    
    for file in files:
        # Check if the file name matches the pattern
        if file.startswith('WINDSDATA_') and file.endswith('.csv'):
            # Extract the year, month, and day from the filename
            try:
                file_date = get_file_date(file)
                
                # Check if the file's date is within the start and end date range
                if from_date <= file_date <= to_date:
                    matching_files.append(file)

            except ValueError:
                # If there's a problem with the filename format, skip the file
                continue

    # Read and process the matching files
    json_list = []
    for file in matching_files:
        json_list.append(csv_to_json(file, avg))

    response = {
        "from_date": from_date.strftime("%Y-%m-%d"),
        "to_date": to_date.strftime("%Y-%m-%d"),
        "data": json_list,
        "message": "Hello, Raspberry Pi 4 with a static IP!",
        "status": "success"
    }
    return jsonify(response)

#-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-#

def csv_to_json(csv_file_path, avg):
    data_list = []
    date = get_file_date(csv_file_path)
    
    with open(os.path.join(FILE_PATH, csv_file_path), mode='r', newline='', encoding='utf-8') as csvfile:
        csvreader = csv.DictReader(csvfile)  # Automatically converts rows to dictionaries

        data_list =  action_average(csvreader, avg)

    response = {
        "date": date,
        "data": data_list
    }
    return response

#-------------------------------------------------------------#

def action_average(csv, avg):
    speed_list = []
    angle_list = []
    date_list = []

    row_list = []
    i = 1
    for row in csv:
        if i == avg:
            if i == 1:
                speed_list.append(row['SPEED'])
                angle_list.append(row['ANGLE'])
                date_list.append(row['DATE'])
            else:
                average = take_average(row_list)
                speed_list.append(average['SPEED'])
                angle_list.append(average['ANGLE'])
                date_list.append(average['DATE'])

            row_list = []
            i = 1
        else:
            row_list.append(row)
            i += 1

    return {
                "SPEED": speed_list,
                "DATE": date_list,
                "ANGLE": angle_list
            }

#----------------------#

def take_average(row_list):
    speed = 0
    angle = 0
    date = 0
    i = 0
    for row in row_list:
        speed += int(row['SPEED'])
        if row['ANGLE'] != None:
            angle += int(row['ANGLE'])
        else:
            i += 1
        date += float(row['DATE'])

    if i != len(row_list):
        angle /= (len(row_list)-i)
    else:
        angle = None

    speed /= len(row_list)
    date /= len(row_list)

    return {
        'SPEED': speed,
        'ANGLE': angle,
        'DATE': date
    }


#-------------------------------------------------------------#



#-------------------------------------------------------------#

def get_file_date(path):
    # Extract year, month, day from the filename format
    filename_parts = path.replace('WINDSDATA_', '').replace('.csv', '').split('_')

    return datetime(int(filename_parts[0]), int(filename_parts[1]), int(filename_parts[2]))

#-------------------------------------------------------------#

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)  # Accessible from any device on your local network
