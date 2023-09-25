# ENEE408I - Tutorial 2 - Part C
#
# Author: Daniel McLawhorn, c. 09-23-23
# Adapted: Kent Chesley, 09-24-23

import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
import scipy.signal as sps
from scipy.fft import fft, fftfreq


sample_rate, data = wavfile.read('Cafe_with_noise.wav')
print(sample_rate)
# Perform the Fourier transformation of the cafe signal
duration = len(data)/sample_rate
x = np.linspace(0,duration,len(data))
plt.plot(x,data)
plt.xlim(0,12)
plt.show()

N = len(data)
yf = fft(data)
xf = fftfreq(N,1/sample_rate)

# Plot the frequency response
plt.plot(xf, np.abs(yf))
plt.xlim(0,2000)
plt.ylim(0,30000000)
plt.xlabel("Frequency (Hz)")
plt.ylabel("Amplitude")
plt.title("FFT of cafe sounds")
plt.show()

# From playing the audio file, we can easily identify that there is high-frequency noise. From the FFT, we see that
# this noise has a lower bound of around 1400 Hz. 

ny = sample_rate/2
cutoff = 1190
norm_cutoff = cutoff/ny

b, a = sps.butter(15, norm_cutoff, 'low', False)
data_filtered = sps.filtfilt(b,a,data.astype(float))

# Desierable values for the cutoff and the filter order (the first argument to sps.butter) were determined by trial
# and error with the goal of totally eliminating the audible presence of the high-frequency tone in the output file
# while trying to maintaining audible quality of the voices.

yf_filtered = fft(data_filtered)

# Plot the filtered frequency response
plt.plot(xf, np.abs(yf_filtered))
plt.xlim(0,2000)
plt.xlabel("Frequency (Hz)")
plt.ylabel("Amplitude")
plt.title("FFT of filtered cafe sounds")
plt.show()

wavfile.write("filtered_cafe.wav",sample_rate, data_filtered.astype(np.int16))