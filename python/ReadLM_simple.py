
import sys
import math

import numpy as np
import matplotlib
import matplotlib.pyplot as plt

import scipy
from scipy import stats
# from scipy import norm

numarg = int(len(sys.argv))
arglist = sys.argv
# print("Number of arguments: ", numarg)
# print("Argument List: ", arglist)

if (numarg > 1):
  filename = arglist[1]
  # print("you're the one that I want (uh uh uh): ", filename)
else:
  filename = input("give filename:  ")

print ("filename: ", filename)

ffile = open(filename, "r")
if ffile.closed:
  print("file is closed: ", filename)
  quit()

# 		ffilelines = ffile.readlines()
# 		for line in ffilelines:
#			The above does not work when there are empty lines
#			Python is too stupid to understand that we don't care about empty lines
#
# ================== a function <<<<<<<<<<<<<<
def nonblank_lines(in_file):
    for ll in in_file:
        line = ll.rstrip()
        if line:
            yield line
# end of function >>>>>>>>>>>>>>>>>>>>>>>>>>>>

# begin of the loop <<<<<<<<<<<<
linecount = 0
array_E = []
array_posX = []
array_posY = []
array_posZ = []
array_radius = []
for line in nonblank_lines(ffile):
  # Python is so incredibly stupid that we have to do this insane stuff
  # just to read different variables from a line from a file...
  # 	1. Let's split the line into an array called "fields" using the " " as a separator:
  #     1.1 get rid of multiple spaces...
  singlespace = " "
  cleanline = singlespace.join(line.split()) # this is the most ridiculous thing ever....!!!!
  #     1.2 split line in fields
  fields = cleanline.split(" ")	                        # values are separated by a space (obviously)

  #     2. and let's extract the data:
  evt = fields[0]
  detName = fields[1]
  #	print("detName: ", detName)
  time = fields[2]
  energy = float(fields[3])
  pos_x = float(fields[4])
  pos_y = float(fields[5])
  pos_z = float(fields[6])
  radius = math.sqrt(pos_x*pos_x + pos_y*pos_y);

  array_E.append(energy)
  array_posX.append(pos_x)
  array_posY.append(pos_y)
  array_posZ.append(pos_z)
  array_radius.append(radius)
 
  linecount += 1
# end of the loop >>>>>>>>>>>>

# Start or E fitting <<<<<<<<<
plt.figure(1)
n, bins, patches = plt.hist(array_E, bins=1400, range=(0.0,1.400), label="Energy", color="blue")

# We know that the mean should be 0.511, so we select the region around the peak...
mass_emin = 0.511
delta = 0.008 # 0.010
peak_E = []
for E in array_E:
    if ( (E > mass_emin - delta) and (E < mass_emin + delta) ):
        peak_E.append(E)
nvalues, bins, patches = plt.hist(peak_E, bins=1400, range=(0.0,1.400), label="Energy peak", color="red", alpha=0.4)

# We need to know the total area below the peak, so shift the fit up to the correct height
# (This is insane!!!)
bin_width = bins[1] - bins[0]
area = np.sum(nvalues) * bin_width

mu, sigma = scipy.stats.norm.fit(peak_E)
print("fit results: %.4f, %.4f" % (mu, sigma))
best_fit_line = scipy.stats.norm.pdf(bins, mu, sigma) * area
plt.plot(bins, best_fit_line, color="green")

# calculate Chi2
chi2 = 0
counts_binsnon0 = 0
for index, counts in enumerate(nvalues, start=0):   # default is zero
    # the line has #nbins+1 values (a value for begin and end of each bin)
    line_begin = best_fit_line[index]
    line_end = best_fit_line[index+1]
    expected = (line_end + line_begin)/2
    if (counts > 0 and expected > 0):
        chi2 += (counts - expected) * (counts - expected)/expected
        # print(index, "O: ", counts, "line 0 - n: %.2f %.2f", line_begin, line_end
        #            , "E: %.2f", expected, "chi2: %.2f", chi2)
        print(index, "O: %d, line[start]: %.2f, line[end]: %.2f, E: %.2f, chi2: %.3f"
              % (counts, line_begin, line_end, expected, chi2))
        counts_binsnon0 += 1

# print("chi2: %.2f", chi2, " #bins(non-zero): ", counts_binsnon0, " #d.o.f.: ", counts_binsnon0 - 3)
# Why is everything in python weird and complicated??
dof = counts_binsnon0 - 3
print("chi2: %.3f #bins(non-zero): %d  #d.o.f.: %d" % (chi2, counts_binsnon0, dof))

title = "Fit results: $\mu$ = %.4f, $\sigma$ = %.4f, $\chi^2$/d.o.f.: %.3f/%d" % (mu, sigma, chi2, dof)
plt.title(title)

# Print the thing on screen
plt.show(block=False)

# >>>>>>>>>>>>>>>>>>>>>>>>>>>>

# plt.show(block=False) 	# obviously, we don't want to block, you idiot

#	array_wth = [0.8, 0.2, 0.2, 0.4, 0.9, 0.1, 0.4, 0.5, 1.1, 0.2, 0.2, 0.6]
#	print("test array: ", array_wth, " with size: ", len(array_wth) )
#	plt.figure(2)
#	plt.hist(array_wth, bins=1120, range=(0.0, 1.400))
#	plt.show(block=False) 	# obviously, we don't want to block, you idiot

plt.figure(2)
plt.hist2d(array_posX, array_posY, bins=[601, 601], range=[[-300, 300], [-300, 300]], cmin=1)
	# cmin = 1 sets all values below 1 to white (don't ask, just don't ask...)
plt.show(block=False) 	# obviously, we don't want to block, you idiot

plt.figure(3)
plt.hist(array_posZ, bins=500, range=(-250, 250))
plt.show(block=False)   # obviously, we don't want to block, you idiot

plt.figure(4)
plt.hist(array_radius, bins=300, range=(0, 300))
plt.show(block=False)   # obviously, we don't want to block, you idiot

# plt.ion()

dummy=input("type any stupid character to quit ") 
	# and obviously we don't want to quit either, moron!





