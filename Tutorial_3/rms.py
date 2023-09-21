import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
import scipy.signal as sps


import math

samplerate1, data1 = wavfile.read('M1.wav')
data1 = np.int64(data1)
sq1 = np.square(data1)
sum1 = np.sum(sq1)
rms1 = np.sqrt(np.divide(sum1,len(data1)))
rms1 = np.sum(rms1)
print(rms1)

samplerate2, data2 = wavfile.read('M2.wav')
data2 = np.int64(data2)
sq2 = np.square(data2)
sum2 = np.sum(sq2)
rms2 = np.sqrt(np.divide(sum2,len(data2)))
rms2 = np.sum(rms2)
print(rms2)

samplerate3, data3 = wavfile.read('M3.wav')
data3 = np.int64(data3)
sq3 = np.square(data3)
sum3 = np.sum(sq3)
rms3 = np.sqrt(np.divide(sum3,len(data3)))
rms3 = np.sum(rms3)
print(rms3)



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

fig, ax = plt.subplots(3)
x = np.linspace(0,len(data1),len(data1))
x2 = np.linspace(0,len(r),len(r))
ax[0].plot(x,data1,label="Curve")
ax[1].plot(x,data2,label="Curve")
ax[2].plot(x2,r,label="Curve")

# ax[2].plot(x,r,label="Curve")
plt.show()

