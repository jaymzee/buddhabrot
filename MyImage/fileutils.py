import os

def change_extension(in_filename, ext):
    """change file extension"""
    out_basename, _ = os.path.splitext(in_filename)
    return out_basename + "." + ext
