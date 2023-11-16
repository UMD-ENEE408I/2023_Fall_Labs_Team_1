import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
import scipy.signal as sps

import math

samplerate1, data1 = wavfile.read('output.wav')
data1 = np.int64(data1)
sq1 = np.square(data1)
sum1 = np.sum(sq1)
rms1 = np.sqrt(np.divide(sum1,len(data1)))
rms1 = np.divide(rms1,57.5068)
rms1 = np.sum(rms1)
print(rms1)

samplerate2, data2 = wavfile.read('test.wav')
data2 = np.int64(data2)
sq2 = np.square(data2)
sum2 = np.sum(sq2)
rms2 = np.sqrt(np.divide(sum2,len(data2)))
rms2 = np.divide(rms2,92.3593)
rms2 = np.sum(rms2)
print(rms2)

r = sps.correlate(data1,data2)
print(max(r))
max_index = np.array(r).argmax()
print(max_index)
print(r[max_index])
print(data1[np.sum(round(np.divide(max_index,2)))])
print(data2[np.sum(round(np.divide(max_index,2)))])

print(len(data1))
print(len(data2))
print(len(r))

sample_delay = len(data1)-(max_index+1)
print(sample_delay)
time_delay = sample_delay/8000
print("Time delay: " + str(time_delay) + "seconds")