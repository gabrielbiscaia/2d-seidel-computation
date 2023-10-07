import matplotlib.pyplot as plt

time_small_mpi = [465.252, 280.950, 246.992]
time_small_pthreads = [422.825, 286.282, 248.736]

time_small_sequential = 595.434

time_standard_mpi = [572.389,  392.555, 372.640]
time_standard_pthreads = [580.231, 368.326, 357.986]

time_standard_sequential = 645.608

time_large_mpi = [732.775,518.994,469.826]
time_large_pthreads = [785.297, 496.970, 461.914]

time_large_sequential = 1238.800



# x axis values 
x = [2,4,8]
# corresponding y axis values 
y_mpi_small = []
y_mpi_standard = []
y_mpi_large = []

y_pthreads_small = []
y_pthreads_standard = []
y_pthreads_large = []


x_ideal=[1,1,1]
y_ideal=[1,1,1]


for i in range(3):
    y_mpi_small.append(time_small_sequential/time_small_mpi[i])
    y_mpi_standard.append(time_standard_sequential/time_standard_mpi[i])
    y_mpi_large.append(time_large_sequential/time_large_mpi[i])

for i in range(3):
    y_pthreads_small.append(time_large_sequential/time_small_pthreads[i])
    y_pthreads_standard.append(time_large_sequential/time_standard_pthreads[i])
    y_pthreads_large.append(time_large_sequential/time_large_pthreads[i])



#um grafico 

plt.plot(x, x , '#000000', label='ideal') 

plt.plot(x, y_mpi_small, 'C0', label='mpi_small')
plt.plot(x, y_mpi_standard, 'C1', label='mpi_standard')
plt.plot(x, y_mpi_large, 'C2', label='mpi_large')
plt.plot(x, y_pthreads_small, 'C3', label='pthreads_small')
plt.plot(x, y_pthreads_standard, 'C4', label='pthreads_standard')
plt.plot(x, y_pthreads_large, 'C5', label='pthreads_large')
plt.legend()

  
# naming the x axis 
plt.xlabel('x - Núcleos') 
# naming the y axis 
plt.ylabel('y - Speedup') 
  
# giving a title to my graph 
plt.title('Gráfico de Speedup')
  
# function to show the plot 
plt.show()
