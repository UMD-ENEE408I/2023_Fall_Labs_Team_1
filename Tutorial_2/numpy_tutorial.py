import numpy as np
#Create a numpy array with the elements [1,2,3,4]
a = np.array([1,2,3,4])
print(a)
#Use np.ones to create an array of 1's with dimension 3x4
b = np.ones((3,4))
print(b)
#Use np.zeros to create an array of 0's with dimension 4x3
c = np.ones((4,3))
print(c)
#Create a 2x3 matrix A and a 3x4 matrix B and perform a matrix multiplication
A = np.array([[1,2,5],[7,1,1]])
print(A)
B = np.array([[1,1,1,1],[1,4,1,1],[1,1,1,1]])
print(B)
A_times_B = np.matmul(A,B)
print(A_times_B)
#Find the eigenvalues and eigenvectors of the matrix [[3,1],[1,2]]
m = np.array([[3,1],[1,2]])
print(m)
eigenvalues = np.linalg.eig(m)
print(eigenvalues)