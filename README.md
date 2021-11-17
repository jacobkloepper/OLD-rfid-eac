# old-rfid-eac
NB: this is an old repo that evolved into my `linux-rfid-eac`. I am keeping this up for reference. There were a few good ideas done here that were abandoned later on.
I had to sanitize some of the old code so this should be broken now. 

This project covers the logistic aspects of an RFID electronic access control system. This is intended to be added on top of an existing EAC. The project assumes a 13.56 MHz reader is used for logistics. 

## data_processing
The data processing functionality is written in C++. 
1. It accepts RFID tag ID's and reader flags from a reader via serial bus. Currently uses Windows api. Implementation is planned to be on Raspberry Pi.
2. The ID's and flags are parsed and mapped to the corresponding user, action, and location. The program interfaces with a database.
3. The user, action, and location information is written to a log file along with the time.

## user_mod
User modification entails control over the user database. This is achieved via Python scripts.
1. A user mod python script is run via a CLI. The user table can be created, added to, removed from, edited, or cleared.
2. The project interfaces with the database via SQLite3. SQLite3 is included with Python, but external `sqlite3.c` and `sqlite3.h` files must be added to the C++ source folder.

## reporter
Reporting is done by reading the log files produced by data_processing and parsing them into a prettier format.
1. A reporter python script is run via a CLI. Reports can be viewed, updated, or copied. The only useful command is updating, as the other commands could simply be done in a file explorer.
2. Reports are yearly and in an `.xlsx` format.

# scripts (from project root)

## usermod
Running the command `usermod` (with the `.bashrc` included) in the project's root directory will run one of two scripts:
1. `usermod_ux.py`, which asks for a user name, then for an RFID tag to be read. Depending on the state of the user database, the script will complete the most logical task (adding new user, adding new key to user, or removing old key from user)
2. If run with command line arguments, `user_mod.py` will run. Input `usermod -h` for info on use.

## report
Running this command in the project's root directory can do a couple of things:
1. With the argument `report --write`, the script will update this year's report form the last unwritten day to yesterday.
2. With the argument `report --pop`, the script will update this year's pop IOU report from the logs, and update the logs to reflect that they have been tracked.
3. With the argument `report --checkoff <firstname> <lastname> <quantity>`, the script will "check off" a user's IOU ticks from the pop log.

## upload
This command uploads files to a remote location (test loc right now)
1. The argument `upload --add path/from/wd.extension' will add the file at `path/from/wd.extension' to a remote location with the same file structure.
2. The argument `upload --add all` will upload all add and logs that have not previously been added.
3. The argument `upload --rm <filename.ext | all>` works similarly to `--add`.
4. `upload --push` uploads all added files.
5. `upload --list` lists all added files.