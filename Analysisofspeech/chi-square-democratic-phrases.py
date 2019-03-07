from __future__ import division
from decimal import * 
from beautifultable import BeautifulTable
import operator
from tabulate import tabulate
import glob
import os
import csv
import statsmodels.api as sm
import pandas as pd
import collections



def ngrams(inputtext, n):
  		inputtext = inputtext.split()
  		output = {}
  		for i in range(len(inputtext)-n+1):
			#print(inputtext[i:i+n])
    			g = '  '.join(inputtext[i:i+n]).lower()
			#print(g)
    			output.setdefault(g, 0)
    			output[g] += 1
			#print(output)
  		return output
open_file = open('democratic_speeches.txt','r+').read() 
d = ngrams(open_file,2)
#print("before",d)
#d = sorted(d.items(), key=operator.itemgetter(1))
#d = collections.OrderedDict(sorted(d.items()))
#print(type(d))
#print("after",d)
#print("before"+str(len(d)))
#d = dict(d.items()[1000:])
#d=d[0:1000]
#d = {k:d[k] for k in d.keys()[:1000]}

#print("before"+str(len(d)))
open_file_republic = open('republican_speeches.txt','r+').read() 
r = ngrams(open_file_republic,2)
r = collections.OrderedDict(sorted(r.items()))
print(type(r))
print("before"+str(len(r)))
r = {k:r[k] for k in r.keys()[:1000]}
print(len(r))

fnotjd = 0
fnotjr = 0
fjd=0
fjr=0
chisquare = 0
	
for w in sorted(d, key=d.get, reverse=True):
		    fnotjd = fnotjd + d[w]
#print("fnotjd:"+str(fnotjd))


for z in sorted(r, key=r.get, reverse=True):
		    fnotjr = fnotjr + r[z]
#print("fnotjr:"+str(fnotjr))


try:
	dObj=[]
	democratic_speaker_frequency=[]		
	dObj.append(['Tokens', 'fjd','fjr','f~jr','f~jd','Chi Square'])
	democratic_speaker_frequency.append(['BioGuideID', 'Token','f pc','Summation p f pc','~f pc = f pc / Summation p f pc'])
	#republican_speaker_frequency.append(['BioGuideID', 'Token','f pc','Summation p f pc','~f pc = f pc / Summation p f pc'])
	for w in sorted(d, key=d.get, reverse=True):
			if w in r:
		    		#print("Democratic:"+str(w)+":"+str(d[w]))
				#print("Republican:"+str(w)+":"+str(r[w]))
				fjr = r[w]
				fjd = d[w]
				f_jr = fnotjr-r[w]
				f_jd = fnotjd-d[w]
				#getcontext().prec = 28
				#float("{0:.5f}".format(chi_square))
				chi_square = float((fjr*f_jd - fjd*f_jr)/((fjr+fjd)*(fjr+f_jd)*(f_jr+fjd)*(f_jr+f_jd)))
				with open('dbfiles/democratic_speeches_chi_square_metrics_inline.csv', 'a+') as csvfile:
    							writer = csv.writer(csvfile)
			
    							[writer.writerow([str(w),str(fjd),str(fjr),str(f_jr),str(f_jd),str(chi_square)])]

				##dObj.append([str(w),str(fjd),str(fjr),str(f_jr),str(f_jd),str(chi_square)])
		
			path = 'democratic/'
			
			try:
				for filename in os.listdir(path):
					#print ("Demo")
					open_file = open(path+filename,'r+').read() 
					democratic_speaker = ngrams(open_file,2)

					for tokens in democratic_speaker:
						#print(tokens,democratic_speaker[tokens])
						if str(tokens) == str(w):
							print(filename,tokens,str(w),democratic_speaker[tokens])
							v = democratic_speaker[tokens]/fnotjd

							##democratic_speaker_frequency.append([filename,tokens,democratic_speaker[tokens],fnotjd,v])
						with open('dbfiles/democratic_congressman_individual_metrics_inline.csv', 'a+') as csvfile:

    							writer = csv.writer(csvfile)
    							[writer.writerow([filename,tokens,democratic_speaker[tokens],fnotjd,democratic_speaker[tokens]/fnotjd])]
						
			except KeyError:
				print ("File not found Error")
				
		

except KeyError:
	print ("Error")







