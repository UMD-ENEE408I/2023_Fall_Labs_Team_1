from scipy import linalg
from scipy.optimize import minimize_scalar
import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq
# Solve the linear system of equations
# 3x+y = 9
# x+2y = 8
A = np.array([[3,1],[1,2]])
b = np.array([[9],[8]])
print(np.linalg.solve(A,b))

# Find the minimum of y = x^2 + 2x
f = lambda x: np.square(x) + 2*x
ans = minimize_scalar(f,method='brent')
print(ans.x)

# Perform the Fourier transformation of f(x)=sin(100*pi*x)+0.5*sin(160*pi*x)
sample_rate = 25000
duration = 5
x = np.linspace(0,duration,sample_rate*duration)
y = np.sin(100*np.pi*x)+0.5*np.sin(160*np.pi*x)
plt.plot(x,y)
plt.xlim(0,0.1)
plt.show()

N = sample_rate*duration
yf = fft(y)
xf = fftfreq(N,1/sample_rate)

# Plot the frequency response
plt.plot(xf, np.abs(yf))
plt.xlim(0,100)
plt.xlabel("Frequency (Hz)")
plt.ylabel("Amplitude")
plt.title("FFT of f(x)=sin(100*pi*x)+0.5*sin(160*pi*x)")
plt.show()