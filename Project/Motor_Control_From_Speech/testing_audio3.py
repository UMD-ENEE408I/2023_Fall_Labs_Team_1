import volume
import microphone
import bandpass
microphone.record()
bandpass.filter('left.wav', 'right.wav')
print(volume.sound_dir())