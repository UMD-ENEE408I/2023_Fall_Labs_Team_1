# for data transformation
import numpy as np
# for visualizing the data
import matplotlib.pyplot as plt
# for opening the media file
import scipy.io.wavfile as wavfile

fs, aud = wavfile.read('output.wav')
# select left channel only
aud = aud[:,0]
# trim the first 5 seconds
first = aud[:int(fs*5)]
powerSpectrum, frequenciesFound, time, imageAxis = plt.specgram(first, Fs=fs)
plt.xlabel("Time (sec)")
plt.ylabel("Frequency (Hz)")
plt.show()