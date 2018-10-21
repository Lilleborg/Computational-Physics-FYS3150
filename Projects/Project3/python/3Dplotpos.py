import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import sys
import glob

def unpack(solver,exeptions = []):
	path = "../build/"+solver + "/positions_"
	list_of_files = glob.glob(path+'*.txt')
	for exept in exeptions:
		for ele in list_of_files:
			if exept in ele:
				list_of_files.remove(ele)
	list_names = [i.strip(path)  for i in list_of_files]
	list_names = [i.strip(".txt")  for i in list_names]
	list_objects_pos = []
	for files in list_of_files:
		list_objects_pos.append(np.loadtxt(files,unpack=True))
	return list_objects_pos,list_names

if __name__ == '__main__':
	solver = sys.argv[1]
	objects,names = unpack(solver,["Uranus"])
	skip = int(len(objects[0][0])/1000.)
	
	plt.figure(0)
	for pos,name in zip(objects,names):
		plt.plot(pos[0,::skip],pos[1,::skip],label=name)
	
	plt.xlabel('x [AU]')
	plt.ylabel('y [AU]')
	plt.legend()

	fig = plt.figure(1)
	ax = fig.gca(projection='3d')
	for pos,name in zip(objects,names):
		ax.plot(pos[0,::skip],pos[1,::skip],pos[2,::skip],label=name)
	
	ax.set_xlabel('x [AU]')
	ax.set_ylabel('y [AU]')
	ax.set_zlabel('z [AU]')
	plt.legend()
	ax.set_zlim(-1,1)
	
	plt.show()