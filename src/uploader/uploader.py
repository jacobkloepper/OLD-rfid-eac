import argparse
import remote_interface as ri
import cache
import os

# Get all files on system under directory parent
def get_all_files(parent):
    all_files = []

    parent_path = os.listdir(parent)
    for child in parent_path:
        child_path = os.path.join(parent, child)

        # If file is directory, recursively get its children
        if (os.path.isdir(child_path)):
            all_files = all_files + get_all_files(child_path)
        else:
            all_files.append(child_path)

    return all_files


# Parser stuff
p = argparse.ArgumentParser(description='Upload logs and reports.')
p.add_argument('--add', metavar=('<file/path.ext | all>'), nargs=1, type=str, help='adds report or log at file/path.ext, or all unpushed files, to the list of reports/logs to pushed.')
p.add_argument('--rm', metavar=( '<file/path.ext | all>'), nargs=1, type=str, help='removes report or log at file/path.ext, or all added files, from the list of reports/logs to be pushed.')
p.add_argument('--push', action='store_true', help='push all added uploads')
p.add_argument('--list', action='store_true', help='list added files')
args = p.parse_args()

# ADD A FILE
if (args.add):
    filepath = args.add[0]

    # 1. File exists
    if (os.path.isfile(filepath)):

        if (filepath in cache.get_rows()):
            print("[{}] already added.".format(filepath))
        else:
            cache.add_cfg(filepath)
            print("[{}] added".format(filepath))

    # 2. Add all files that haven't been pushed before (WIP: before today)
    elif (args.add[0] == 'all'):

        hist = cache.get_hist()
        files = get_all_files('reports') + get_all_files('src\data\logs')

        print("Files added:")
        for file in files:
            if (file not in hist):
                cache.add_cfg(file)
                print("\t[{}]".format(file))

    # 3. File does not exist
    else:
        print("[{}] cannot be found.".format(filepath))

# REMOVE A FILE
elif (args.rm):
    filepath = args.rm[0]

    # 1. File exists
    if (os.path.isfile(filepath)):

        if ([filepath] in cache.get_rows()):
            cache.rm_cfg(filepath)
            print("[{}] removed.".format(filepath))
        else:
            print("[{}] not added.".format(filepath))
    
    # 2. Remove all files
    elif (args.rm[0] == 'all'):
        if (len(cache.get_rows()) > 0):
            cache.clear_cfg()
            print("Removed all files")
        else:
            print("Config already empty.")

    # 3. File does not exist
    else:
        print("[{}] cannot be found.".format(filepath))

# PUSH FILES
elif (args.push):
    if (len(cache.get_rows()) > 0):
        files = cache.get_rows()
        for file in files:
            ri.upload(file[0])
        print("Files uploaded.")

        cache.clear_cfg()
        print("Config file reset.")
    else:
        print("No files added to list.")

# LIST ADDED FILES
elif (args.list):
    if (len(cache.get_rows()) > 0):
        cache.print_cfg()
    else:
        print("No files added to list.")

# NO ARGS GIVEN
else:
    #
    print("For help, input upload -h")