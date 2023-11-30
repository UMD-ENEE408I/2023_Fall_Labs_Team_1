import numpy as np
import scipy.signal
import scipy.io.wavfile
import matplotlib.pyplot as plt

def bandpass(data: np.ndarray, edges: list[float], sample_rate: float, poles: int = 5):
    sos = scipy.signal.butter(poles, edges, 'bandpass', fs=sample_rate, output='sos')
    filtered_data = scipy.signal.sosfiltfilt(sos, data)
    return filtered_data


# Load sample data from a WAV file
sample_rate, data = scipy.io.wavfile.read('left.wav')
times = np.arange(len(data))/sample_rate

# Apply a 10-50 Hz bandpass filter to the original data
filtered = bandpass(data, [6500, 8500], sample_rate)
filtered = np.asarray(filtered, dtype=np.int16)
filtered_wav = scipy.io.wavfile.write('filteredl.wav', sample_rate, filtered)

# Load sample data from a WAV file
sample_rate, data = scipy.io.wavfile.read('right.wav')
times = np.arange(len(data))/sample_rate

# Apply a 10-50 Hz bandpass filter to the original data
filtered = bandpass(data, [6500, 8500], sample_rate)
filtered = np.asarray(filtered, dtype=np.int16)
filtered_wav = scipy.io.wavfile.write('filteredr.wav', sample_rate, filtered)