import os
import shutil as sh

def get_src(filepath):
    _src = "{}".format(filepath)
    return os.path.realpath(_src)

# WIP
def get_dst(filepath):
    assert False # code just sanitized, not planning to fix.
    _dst = "".format(filepath)
    return os.path.realpath(_dst)

def upload(filepath):
# Uploads file to same directory structure (with hard-coded remote parent)
    src = get_src(filepath)
    dst = get_dst("test\\{}".format(filepath))

    try:
        sh.copy(src, dst)
    except:
        print("[{}] could not be uploaded to [{}]".format(src,dst))