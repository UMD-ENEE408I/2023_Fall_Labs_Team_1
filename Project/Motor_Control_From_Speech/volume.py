import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
import scipy.signal as sps

import math

def sound_dir():
    samplerate1, data1 = wavfile.read('filteredl.wav')
    data1 = np.int64(data1)
    sq1 = np.square(data1)
    sum1 = np.sum(sq1)
    rms1 = np.sqrt(np.divide(sum1,len(data1)))
    rms1 = np.divide(rms1,112.10185)
    rms1 = np.sum(rms1)
    print("Left: ", rms1)

    samplerate2, data2 = wavfile.read('filteredr.wav')
    data2 = np.int64(data2)
    sq2 = np.square(data2)
    sum2 = np.sum(sq2)
    rms2 = np.sqrt(np.divide(sum2,len(data2)))
    rms2 = np.divide(rms2,113.58207)
    rms2 = np.sum(rms2)
    print("Right: ", rms2)
    if(rms1>1.1*rms2):
        return -1
    elif(rms2>1.1*rms1):
        return 1
    else:
        return 0