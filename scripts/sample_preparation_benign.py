import os
import sys
import math
from decimal import *

file1_name = sys.argv[1]
file1 = open(file1_name, 'r')
top_params = list()

for line in file1:
	name = line.split('\n')
	top_params.append(name[0])

counters = list()
all_counters = list()

statsFile_name = sys.argv[2]
statsFile = open(statsFile_name, 'r')

features = list()

cnt = 0
for line in statsFile:
	if "Begin" in line:
		counters = []
		cnt = 0
	elif "End" in line:
		new_counters = []
		for param in top_params:
			found = False
			for counter_element in counters:
				if param == counter_element[0]:
					found = True
					if math.isnan(counter_element[1]):
						new_counters.append(0)
					else:
						new_counters.append(counter_element[1])
			if found == False:
				new_counters.append(0)
		all_counters.append(new_counters)
	else:
		segments = line.split()
		if len(segments) > 2:
			name = segments[0]
			counter = float(segments[1])
			if name in top_params:
				element = [name, counter]
				counters.append(element)
				cnt = cnt + 1


#max_vector keeps the maximumn value of each counter across all counters
max_vector = []
for i in range(0, len(all_counters[0])):
	max_vector.append(all_counters[0][i])

for vector in all_counters:
	for i in range(0, len(vector)):
		if vector[i] > max_vector[i]:
			max_vector[i] = vector[i]

#in this step we normalize all the counters in a samplle to the maximum counter
all_normalized_samples = []

for sample in all_counters:
	normalized_counters = []
	for i in range(0, len(sample)):
		norm = 0
		if max_vector[i] > 0:
			norm = sample[i] / max_vector[i]
		normalized_counters.append(norm)
	all_normalized_samples.append(normalized_counters)

features_string = ""
#in this step we print the samples in the FANN format (and with a label)
for sample in all_normalized_samples:
	cnt = 0
	for counter in sample:
		cnt = cnt + 1
		rounded_counter = ("%.6f" % counter).rstrip('.')
		if cnt == len(sample):
			features_string = features_string + str(rounded_counter) + "\n"
		else:
			features_string = features_string + str(rounded_counter) + " "
	features_string = features_string + "-1\n"

print("%d %d 1" % (len(all_normalized_samples), len(top_params)))
print(features_string)
