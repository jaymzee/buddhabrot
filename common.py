import numpy as np


def array_stats(arr, header):
    """print statistics of the numpy array"""
    if header:
        print(header)
        indent = "    "
    else:
        indent = ""
    print("%smin: %.3f, max: %.3f" % (indent, np.min(arr), np.max(arr)))
    print("%smedian: %.3f, mean: %.3f, std: %.3f" %
          (indent, np.median(arr), np.mean(arr), np.std(arr)))


def load_array(filename):
    """read text file into numpy array"""
    arr = np.loadtxt(filename)
    array_stats(arr, "input file statistics:")
    return arr


def clip_array(arr, threshold):
    """clip the image array at threshold"""
    # clip values
    if threshold is None:
        return arr

    print("clipping values abouve %d" % threshold)
    clipped_arr = np.minimum(arr, threshold)
    diff_arr = arr - clipped_arr
    diff_count = np.count_nonzero(diff_arr)
    print("clipped %d pixels (%.3f %%)" %
          (diff_count, 100.0 * diff_count / diff_arr.size))
    array_stats(clipped_arr, "statistics after clipping:")
    return clipped_arr
