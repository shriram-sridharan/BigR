import sys

file = open("testR.R", "w")
file.write('A <- matrix(scan("'+sys.argv[3]+'", n = ' + sys.argv[1] + '*' + sys.argv[2] + '),' + sys.argv[1] + ',' + sys.argv[2] + ')\n')
file.write('B <- matrix(scan("'+sys.argv[3]+'", n = ' + sys.argv[1] + '*' + sys.argv[2] + '),' + sys.argv[1] + ',' + sys.argv[2] + ')')
file.write('\nC <- A %*% B')
file.close()


file = open("testBigR.R", "w")
file.write('library(BIGR)\n')
file.write('A <- matrix.bigr(' + sys.argv[1] + ',' + sys.argv[2] + ',"' + sys.argv[3] +'")\n')
file.write('B <- matrix.bigr(' + sys.argv[1] + ',' + sys.argv[2] + ',"' + sys.argv[3] +'")')
file.write('\nC <- A %*% B')
file.close()

