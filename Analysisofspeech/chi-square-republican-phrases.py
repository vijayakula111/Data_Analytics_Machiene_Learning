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
#d = dict(d.items()[1000:])
#d = {k:d[k] for k in d.keys()[:1000]}


d = collections.OrderedDict(sorted(d.items()))
print(type(d))
#print("after",d)
print("before"+str(len(d)))
#d = dict(d.items()[1000:])
#d=d[0:1000]
d = {k:d[k] for k in d.keys()[:1000]}

print("before"+str(len(d)))
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
	rObj=[]
	#table_R = BeautifulTable()
	#table_R.column_headers = ['Tokens', 'fjr','fjd','f~jr','f~jd','Chi Square']
	
	#with open('republican_speeches_chi_square_metrics.txt','a+') as out_json:
	
	rObj.append(['Tokens', 'fjr','fjd','f~jr','f~jd','Chi Square'])
	for z in sorted(r, key=r.get, reverse=True):
                    if z in d:

				fjr = r[z]
				fjd = d[z]
				f_jr = fnotjr-r[z]
				f_jd = fnotjd-d[z]
				#getcontext().prec = 28
				chi_square = float((fjr*f_jd - fjd*f_jr)/((fjr+fjd)*(fjr+f_jd)*(f_jr+fjd)*(f_jr+f_jd)))
                    			
				#table_R.append_row([str(z),str(fjr),str(fjd),str(f_jr),str(f_jd),str(chi_square)])
				##rObj.append([str(z),str(fjr),str(fjd),str(f_jr),str(f_jd),str(chi_square)])
				with open('dbfiles/republican_speeches_chi_square_metrics_inline.csv', 'a+') as csvfile:
    							writer = csv.writer(csvfile)
			
    							[writer.writerow([str(z),str(fjr),str(fjd),str(f_jr),str(f_jd),str(chi_square)])]
				

	 path = 'republican/'
			
		    try:
				
				for filename in os.listdir(path):
					#print ("Repu")
					open_file = open(path+filename,'r+').read() 
					republican_speaker = ngrams(open_file,2)

					#for tokens in sorted(republican_speaker, key=republican_speaker.get, reverse=True):
						#print(filename,tokens,w)
						#if str(tokens) == str(w):
					#print(tokens,republican_speaker[tokens])
					for tokens in republican_speaker:
						if str(tokens) == str(z):
							#print(tokens,str(z),republican_speaker[tokens])
							v = republican_speaker[tokens]/fnotjr
							#OLS_Y_R.append(v)							
							republican_speaker_frequency.append([filename,tokens,republican_speaker[tokens],fnotjr,v])
						
		    except KeyError:
				print ("File not found Error")


except KeyError:
		print ("Error")








