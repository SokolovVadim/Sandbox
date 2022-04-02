import argparse
import json
import os
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from collections import defaultdict

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
    for date, wellbeing in data.items():
        print(date, wellbeing)
    max_month_len = 31
    days = []
    months = set()
    heat_dict = defaultdict(list)
    for key, value in data.items():
        date = key.split('.')
        day = date[0]
        month = date[1]
        year = date[2]
        days.append(int(day))
        months.add(int(month))

        cumulative_date = int(day) + int(month) * 30 + int(year) * 365
        print('month:', int(month), 'cumulative_date:', cumulative_date)
        if value == 'low':
            if month not in heat_dict:
                heat_dict[month] = [-1] * max_month_len
            heat_dict[month][int(day) - 1] = -0.5#append(-0.5)
        if value == 'normal':
            if month not in heat_dict:
                heat_dict[month] = [-1] * max_month_len 
            heat_dict[month][int(day) - 1] = 0
        if value == 'well':
            if month not in heat_dict:
                heat_dict[month] = [-1] * max_month_len
            heat_dict[month][int(day) - 1] = 0.5 

    
    heat = list(heat_dict.values())
    plt.title('2022')
    days = list(range(1, 32))
    x_axis_labels = days # labels for x-axis
    y_axis_labels = heat_dict.keys() # labels for y-axis

    # create seabvorn heatmap with required labels
    sns.heatmap(heat, xticklabels=x_axis_labels, yticklabels=y_axis_labels, cbar_kws={"orientation": "horizontal"}, cmap="Greens")
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