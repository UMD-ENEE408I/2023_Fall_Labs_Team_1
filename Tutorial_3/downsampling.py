# ENEE408I - Tutorial 3 - Part A
#
# Author: Kent Chesley
# Date: 09-21-23


# (2)

# Reference: https://stackoverflow.com/questions/2060628/reading-wav-files-in-python (see accepted answer).

from scipy.io  import wavfile

hv_samplerate, hv_data = wavfile.read("human_voice.wav")

print(hv_samplerate) # In Hz.


# (3)

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

t = np.linspace(0, len(hv_data)/hv_samplerate, len(hv_data))    # Reference: 
# https://www.w3schools.com/python/ref_func_len.asp

fig, ax = plt.subplots()
ax.plot(t, hv_data, label="Original Human Voice")
ax.set_xlabel("Time (s)")
ax.set_ylabel("Pressure")
ax.set_title("Original Human Voice Signal")
fig.set_figwidth(8) # Reference: https://www.geeksforgeeks.org/change-plot-size-in-matplotlib-python/.
plt.show()


# (4)

# To downsample to 8 kHz, take every (48 kHz/(8 kHz) = 6) samples.

# Initialization
hv_data_down = np.zeros((int(len(hv_data)/6)))  # Downasampled signal. Reference:
# https://www.geeksforgeeks.org/how-to-convert-float-to-int-in-python/
t_down = np.zeros((int(len(t)/6)))  # Downsampled time axis.

i = 0
while i < len(hv_data):
    hv_data_down[int(i/6)] = hv_data[i]  
    t_down [int(i/6)] = t[i]
    i = i + 6


# (5)

print(len(hv_data_down))


# (6)

fig, ax = plt.subplots()
ax.plot(t_down, hv_data_down, label="Downsampled Human Voice")
ax.set_xlabel("Time (s)")
ax.set_ylabel("Pressure")
ax.set_title("Downsampled Human Voice Signal")
fig.set_figwidth(8)
plt.show()


# (7)

hv_data_down_window = hv_data_down[7000:7020]
t_down_window = t_down[7000:7020]
hv_data_window = hv_data[6*7000:6*7020]
t_window = t[6*7000:6*7020]

f, (ax1, ax2) = plt.subplots(1,2)   # Reference: https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.subplots.html
ax1.plot(t_window, hv_data_window, label="Original Human Voice")
ax1.set_xlabel("Time (s)")
ax1.set_ylabel("Pressure")
ax1.set_title("Original Human Voice Signal")
ax2.plot(t_down_window, hv_data_down_window, label="Original Human Voice")
ax2.set_xlabel("Time (s)")
ax2.set_ylabel("Pressure")
ax2.set_title("Downsampled Human Voice Signal")
f.set_figwidth(12)
plt.show()

