import csv
import __future__
import util

class log_interface:

    def __init__(self):
        self.me = "me"
    # Logs are in files by date
    # Alg:
    #   0. We know the date now and the next day to report.
    #   1. We need to update every day from next to now.
    #   2. Starting at next, read the log.csv for that day, write to report
    #   3. Continue loop until now.

    def get_day_log(self, date):
    # Given a date (tuple,: (d,m,y) return a list of tuples, [(first, last, timein, timeout)] where timein and timeout are the initial and final enters/exits for that user
        logname = "{}-{}-{}".format(date[0],date[1],date[2])
        logfile = "src/data/logs/attendance/{}/{}/{}.csv".format(date[2],util.mons.get(date[1]), logname)

        log_info = []

        with open(logfile, newline="") as f:
            reader = csv.reader(f)
            next(reader)
            for row in reader:
                log_info.append((row[1], row[2], row[3], row[4]))

        return log_info

    def get_pop_log(self, date):
        logname="{}" .format(util.mons.get(date[1]))
        logfile = "src/data/logs/pop/{}/{}.csv".format(date[2], logname)
        stale_entries = 0
        log_info = [] 
        csv_lines = [] # Get all rows in order to rebuild file later

        with open(logfile, newline="") as f:
            reader = csv.reader(f)
            csv_lines.append(next(reader))

            for row in reader:
                staled_row = []
                staled_row.extend(row)
                staled_row[5] = "TRUE"
                csv_lines.append(staled_row)

                if (row[5] == "TRUE"):
                    stale_entries = stale_entries + 1
                else:
                    stale_entries = stale_entries # do nothing
                    

        with open(logfile, newline="") as f:
            reader = csv.reader(f)
            next(reader)

            for i in range(stale_entries):
                next(reader) 
            for row in reader:
                log_info.append((row[1], row[2]))
        
        try:
            with open(logfile, "w", newline="") as f:
                writer = csv.writer(f)
                for row in csv_lines:
                    writer.writerow(row)
            #print("Log [{}] updated.".format(logfile))
        except:
            print("Could not save report [{}]. File is likely open, close it and try again.".format(logfile))

        return log_info