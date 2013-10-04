import random

file = open('test-matrix12500x12500','w')
for row in xrange(1, 12501):
	for col in xrange(1,12501) :
		file.write(str(random.randint(1, 100)))
		file.write(' ')
	file.write('\n')

