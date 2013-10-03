import random

file = open('test-matrix','w')
for row in xrange(1, 101):
	for col in xrange(1,101) :
		file.write(str(random.randint(1, 10)))
		file.write(' ')
	file.write('\n')

