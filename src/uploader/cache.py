import csv

def add_cfg(filename):
    # Write to cache.cfg
    f = open('src/uploader/cache.cfg', 'a', newline='')
    writer = csv.writer(f)
    writer.writerow([filename])
    f.close()
    # Write to .history
    # Write to cache.cfg
    f = open('src/uploader/.history', 'a', newline='')
    writer = csv.writer(f)
    writer.writerow([filename])
    f.close()

def rm_cfg(filename):
    # Get cfg contents except the rm file
    f = open('src/uploader/cache.cfg', newline='')
    reader = csv.reader(f)
    new_cfg = []
    for row in reader:
        if (row != [filename]):
            new_cfg.append(row)
    f.close()
    # Write new cfg
    f = open('src/uploader/cache.cfg', 'w', newline='')
    writer = csv.writer(f)
    for row in new_cfg:
        writer.writerow(row)
    f.close()

def clear_cfg():
    # Just open and close in write mode
    open('src/uploader/cache.cfg', 'w').close()

def print_cfg():
    f = open('src/uploader/cache.cfg', newline='')
    reader = csv.reader(f)

    print("Files added:")
    for row in reader:
        print("\t{}".format(row[0]))
    f.close()

def get_rows():
    f = open('src/uploader/cache.cfg', newline='')
    reader = csv.reader(f)
    rows = []
    for row in reader:
        rows.append(row)
    f.close()
    return rows

def get_hist():
    f = open('src/uploader/.history', newline='')
    reader = csv.reader(f)
    rows = []
    for row in reader:
        rows.append(row[0])
    f.close()
    return rows