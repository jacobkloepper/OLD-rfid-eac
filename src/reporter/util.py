# Map number of day of week to string
wdays = {
    0: "Monday",
    1: "Tuesday",
    2: "Wednesday",
    3: "Thursday",
    4: "Friday",
    5: "Saturday",
    6: "Sunday"
}
# Map number of month to string
mons = {
    1: "January",
    2: "February",
    3: "March",
    4: "April",
    5: "May",
    6: "June",
    7: "July",
    8: "August",
    9: "September",
    10: "October",
    11: "November",
    12: "December"
}
wdays_abbrev = {
    0: "MON",
    1: "TUE",
    2: "WED",
    3: "THU",
    4: "FRI",
    5: "SAT",
    6: "SUN"
}
mon_num = {
    "January": 1,
    "February": 2,
    "March": 3,
    "April": 4,
    "May": 5,
    "June": 6,
    "July": 7,
    "August": 8,
    "September": 9,
    "October": 10,
    "November": 11,
    "December": 12
}
# Number of people in report
NUM_PERSONS = 24
# check if all members of iterable equal the argument
# NOTE: not general. has to take an iterable which elements containing .value attributes (ie., openpyxl cells)
def all_of(iter, arg):
    for item in iter:
        if (item.value != arg):
            return False
    return True

# given a list of arguments, print a pretty debug message
def db_print(T_list):
    var_list = [str(item) for item in T_list]

    print("\n---------------" + "-"*len((max(var_list,key=len))))
    for i in range(len(var_list)):
        print("var_list[{:>3}]: {}".format(i, var_list[i]))
    print("---------------" + "-"*len((max(var_list,key=len))) + "\n")