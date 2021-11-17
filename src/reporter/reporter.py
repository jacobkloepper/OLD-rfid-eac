# Call to generate the report.
# Will update the report file YTD from the log files.

import argparse
import subprocess
import time

import report_writer as rw

# Pre
date = time.localtime(time.time())
default_year = str(date.tm_year)


# Parser stuff:
p = argparse.ArgumentParser(description='Get formatted reports from log files.')
p.add_argument('--write', action='store_true', help='update report to today')
p.add_argument('--upload', action='store_true', help='upload report to server')
p.add_argument('--pop', action='store_true', help='write a pop log')
p.add_argument('--checkoff', metavar=('<firstname>', '<lastname>', '<quantity>'), nargs=3, type=str, help="check off a user's pop IOU ticks (quantity: n=number of quarters, all=all ticks)")
args = p.parse_args()



# UPDATE REPORT
if (args.write):
    filename = "reports/attendance/{}.xlsx".format(date.tm_year)
    r = rw.ReportWriter(path=filename)
    r.update_ytd()

    try:
        r.wb.save(r.wb_file)
        print("Report [{}] updated.".format(filename))
    except:
        print("Could not save report [{}]. File is likely open, close it and try again.".format(filename))

elif (args.upload):
    print("WIP")

elif (args.pop):
    filename = "reports/pop/{}.xlsx".format(date.tm_year)
    r = rw.ReportWriter(path=filename, pop=True)
    r.update_pop()
    try:
        r.wb.save(r.wb_file)
        print("Report [{}] updated.".format(filename))
    except:
        print("Could not save report [{}]. File is likely open, close it and try again.".format(filename))

elif (args.checkoff):
    filename = "reports/pop/{}.xlsx".format(date.tm_year)
    r = rw.ReportWriter(path=filename, pop=True)
    r.checkoff(args.checkoff[0], args.checkoff[1], args.checkoff[2])
    try:
        r.wb.save(r.wb_file)
        print("Report [{}] updated.".format(filename))
    except:
        print("Could not save report [{}]. File is likely open, close it and try again.".format(filename))

# NO ARGS GIVEN  
else:
    print("For help, input report -h")
