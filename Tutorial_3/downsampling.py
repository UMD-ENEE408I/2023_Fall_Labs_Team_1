import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
import scipy.signal as sps


samplerate, data = wavfile.read('human_voice.wav')

print(samplerate)
print(data.size)

fig, ax = plt.subplots(2)
x = np.linspace(0,85248,85248)

print(x.shape)

print("Downsampling....")
ax[0].plot(x,data,label="Curve")
# Add labels to the axes
ax[0].set_xlabel("x")
ax[0].set_ylabel("y")
# plt.show()

new_rate = 8000
samples = round(len(data)*float(new_rate)/samplerate)
new_data = sps.resample(data,samples)

x = np.linspace(0,new_data.shape[0],new_data.shape[0])

print(x.shape)
print(new_data.shape)
ax[1].plot(x*6,new_data,label="Curve")
# Add labels to the axes
ax[1].set_xlabel("x")
ax[1].set_ylabel("y")
plt.show()

