import subprocess
from subprocess import call
import time
import sys

starttime = time.time()
call('R < ' + sys.argv[1] + ' --no-save', shell=True)
elapsed = time.time() -starttime

print "Total Time = ",  elapsed
