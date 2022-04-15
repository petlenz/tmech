import os
import glob

directions = ['../include/tmech', '../include/tmech/tensor', '../include/tmech/symdiff']

for direction in directions:
	list_of_files = glob.glob(direction + '/*.h')
	for name in list_of_files:
		print(name)
		data = []
		with open(name, 'r') as file:
			data = file.readlines()
		data[0] = '/***************************************************************************\n'
		data[1] = '* Copyright (c) Peter Lenz                                                 *\n'
		data[2] = '*                                                                          *\n'
		data[3] = '* Distributed under the terms of the BSD 3-Clause License.                 *\n'
		data[4] = '*                                                                          *\n'
		data[5] = '* The full license is in the file LICENSE, distributed with this software. *\n'
		data[6] = '****************************************************************************/\n'
		
		with open(name, 'w') as file:
			file.writelines( data )
