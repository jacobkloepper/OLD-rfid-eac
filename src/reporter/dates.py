# Class for managing date objects of the form tuple(dd, mm, yyyy)
# dd   : [1, last]
# mm   : [1, 12]
# yyyy : [epoch year, maxyear]

from calendar import monthrange # monthrange(year, month) returns (d0, dl): d0 is the number of first weekday of month, dl is the number of days in month


def increment(date):
    next_date = ()
    month_info = monthrange(date[2], date[1])

    # If not last day of month
    if (date[0] < month_info[1]):
        next_date = (date[0]+1, date[1], date[2])
    # If last day of not last month
    elif (date[0] == month_info[1] and date[1] < 12):
        next_date = (1, date[1]+1, date[2])
    # If last day of last month
    elif (date[0] == month_info[1] and date[1] == 12):
        next_date = (1, 1, date[2]+1)

    return next_date

def decrement(date):
    # Get prev_date's year
    # If it's new years, get result for free
    if (date[0] == 1 and date[1] == 1):
        return (31,12,date[2]-1)
    else:
        prev_year = date[2]

    # Get prev_date's month
    if (date[0] == 1 and date[1] != 1):
        return (monthrange(prev_year, date[1]-1)[1], date[1] - 1, prev_year)
    else:
        prev_month = date[1]

    # Get prev_date's day
    prev_day = date[0]-1

    return (prev_day, prev_month, prev_year)

def is_valid(date):
    if ((date[0] < 0 or date[0] > monthrange(date[2], date[1])[1]) or (date[1] < 0 or date[1] > 12)):
        return False
    return True

def dates_between(start, end):
# Given two date tuples return a list of all date tuples between start date and end date (inclusive)
    datelist = []

    new_date = start
    while (new_date != end):
        datelist.append(new_date)
        new_date = increment(new_date)
    # Catch end date
    datelist.append(new_date)

    return datelist