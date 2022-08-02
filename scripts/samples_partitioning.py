import os
import sys
import math
from decimal import *
import random

samplesFile_name = sys.argv[1]
samplesFile = open(samplesFile_name, 'r')

counter = 0
samples = list()
label = 0
counters = ""
for line in samplesFile:
	segs = line.split()
	if len(segs) == 3 and segs[2] == "1":
		print("Skipping line: %s" % line)
		continue
	if counter % 2 == 1 and counter > 0:
		label = line
		samples.append([counters, label])
	else:
		counters = line

	counter += 1

total = int(counter/2)

train_samples = list()
test_samples = list()

random.shuffle(samples)

num_train = int(0.1 * total)
print("Total number of samples: %d, number of train samples: %d, number of test samples: %d" % (total, num_train, total - num_train))

counter = 0
for S in samples:
	if counter < num_train:
		train_samples.append(S)
	else:
		test_samples.append(S)

	counter += 1

train_file_name = samplesFile_name + ".train"
print("Writing the train data to %s" % train_file_name)
train_file = open(train_file_name, 'w')
header = "{} 98 1\n".format(num_train)
train_file.write(header)
for T in train_samples:
	counters = T[0]
	label = T[1]
	train_file.write(counters)
	train_file.write(label)

test_file_name = samplesFile_name + ".test"
print("Writing the test data to %s" % test_file_name)
test_file = open(test_file_name, 'w')
header = "{} 98 1\n".format(total - num_train)
test_file.write(header)
for T in test_samples:
	counters = T[0]
	label = T[1]
	test_file.write(counters)
	test_file.write(label)

