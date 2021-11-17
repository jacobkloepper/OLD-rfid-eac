# Functions

# usermod CLI (from root)
usermod() {
	py src/usermod/usermod.py $@
}

# reporter CLI (from root)
report() {
	py src/reporter/reporter.py $@
}

# uploader CLI (from root)
upload() {
	py src/uploader/uploader.py $@
}