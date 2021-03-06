import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import rcParams
import sys
import glob

def unpack(solver,exceptions = []):
	path = "../build/"+solver + "/positions_"
	list_of_files = glob.glob(path+'*.txt')
	for exept in exceptions:
		for ele in list_of_files:
			if exept in ele:
				list_of_files.remove(ele)
	list_names = [i.strip(path)  for i in list_of_files]
	list_names = [i.strip(".txt")  for i in list_names]
	list_objects_pos = []
	for files in list_of_files:
		print '---Loading file---'
		list_objects_pos.append(np.loadtxt(files,unpack=True))
		print files
	return list_objects_pos,list_names

if __name__ == '__main__':
	try:
		solver = sys.argv[1]
	except IndexError:
		print 'Solver type not given, first cmd argument. Using "Verlet".'
		solver = "Verlet"
	try:
		plot_filename = sys.argv[2]
	except IndexError:
		print 'Filename for saving plots not given, second cmd argument. Defaulting to "plot_default_'+solver+'.pdf'
		plot_filename = 'plot_default_' + solver + '.pdf'
	try:
		title = sys.argv[3]
	except IndexError:
		print 'No title given, third cmd argument. Defaulting to "Positions '+solver+'"'
		title = "Positions "+ solver
	try :
		exceptions = sys.argv[4:]
	except IndexError:
		print 'No exceptions given.'
		exceptions = []
	
	objects,names = unpack(solver,exceptions)
	
	# Plotting
	font = {'size'   : 12}
	plt.matplotlib.rc('font', **font)
	rcParams.update({'figure.autolayout': True})

	plt.figure(0)
	for pos,name in zip(objects,names):
		if name == "arth":	#Fixing error from str.strip
			name = "Earth"
		if name == "Sun":
			plt.plot(pos[0],pos[1],'o',label=name)	
		else:
			plt.plot(pos[0],pos[1],'-o',label=name,markevery=[0,-1])
			
	plt.axis('equal')
	plt.title(title)
	plt.xlabel('x [AU]')
	plt.ylabel('y [AU]')
	plt.legend()
	plt.savefig(plot_filename)

	fig = plt.figure(1)
	ax = fig.gca(projection='3d')
	plt.title(title)
	for pos,name in zip(objects,names):
		if name == "arth":	#Fixing error from str.strip
			name = "Earth"
		if name == "Sun":
			plt.plot(pos[0],pos[1],pos[2],'o',label=name)	
		else:
			plt.plot(pos[0],pos[1],pos[2],label=name)
	
	ax.set_xlabel('x [AU]')
	ax.set_ylabel('y [AU]')
	ax.set_zlabel('z [AU]')
	plt.legend()
	plt.savefig("3D"+plot_filename)
	
	plt.show()