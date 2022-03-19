import argparse
import json
import os
import matplotlib.pyplot as plt
import numpy as np

def plot_calendar(calendar):
    print('plotting calendar ...')
    data = {}
    with open(calendar) as json_file:
        try:
            data = json.load(json_file)
        except:
            print('reading err')
            return
    data = dict(sorted(data.items()))
    print(data)
    heat = []
    for key, value in data.items():
        date = key.split('.')
        day = date[0]
        month = date[1]
        year = date[2]

        cumulative_date = int(day) + int(month) * 30 + int(year) * 365
        print('month:', int(month), 'cumulative_date:', cumulative_date)
        if value == 'low':
            heat.append(1)
        if value == 'normal':
            heat.append(0.5)
        if value == 'well':
            heat.append(0)
    heat = [heat]
    print(heat)
    plt.imshow(heat, cmap='summer', interpolation='nearest')
    plt.show()

# if date is not specified, add today's date

def add_day(day, marker):
    print('adding day ...')
    data = {}
    file_path = 'health.json'
    # check if size of file is 0
    if os.stat(file_path).st_size == 0:
        print('File is empty')
        data[day] = marker
        with open(file_path, 'w') as  outfile:
            json.dump(data, outfile)
        return
    
    with open(file_path) as json_file:
        try:
            data = json.load(json_file)
            print(data)
            data[day] = marker
        except:
            print('reading err')
            return
    with open(file_path, 'w') as  outfile:
        json.dump(data, outfile)

if __name__ == "__main__":
    # Instantiate the parser
    parser = argparse.ArgumentParser(description='Optional app description')

    # Optional arguments
    parser.add_argument('--plot', type=str,
                    help='Plot the calendar')
    parser.add_argument('--add_day', type=str,
                    help='Add a day to the calendar')
    parser.add_argument('--marker', type=str,
                    help='Add day marker to the calendar')
    args = parser.parse_args()
    # print("Argument values:")
    print('file_name:', args.plot)
    print('date:', args.add_day)
    print('marker:', args.marker)
    if args.plot:
        plot_calendar(args.plot)
    if args.add_day:
        add_day(args.add_day, args.marker)