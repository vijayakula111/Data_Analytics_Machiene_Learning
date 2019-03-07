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
import statsmodels.api as sm
import numpy as np		
import pandas as pd
import statsmodels.formula.api as smf

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

#print(len(d))
open_file_republic = open('republican_speeches.txt','r+').read() 
r = ngrams(open_file_republic,2)
#r = {k:r[k] for k in r.keys()[:1000]}
#print(len(r))

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



speech_tokens=[]
democratic_speaker_frequency=[]
path = 'democratic/'
predictor_list = []
response_list = []

		
'''with open('dbfiles/2000-democratic-tokens.csv','r+') as csvfile:
    csvdemocdata = csv.reader(csvfile)
    c = 0
    for row in csvdemocdata:
	c = c + 1
	print(c)
    	content = list(row[i] for i in [0])
	speech_tokens=speech_tokens+content
	#print(content[0])'''
c = 0
predictor_list = []
from pathlib import Path

def shellquote(s):
    return "'" + s.replace("'", "'\\''") + "'"

try:
			
			#for filename in os.listdir('.'):
		items = os.listdir(path)

		for filename in items:
    			if filename.endswith(".txt"):
			#pathlist = Path('.').glob('**/*.txt')
			#for filename in pathlist:
					c = c + 1
					#filename = str(filename)
					print(c, filename)
					
					response_list = []
					predictor_list = []
					f = filename[-11:-4]
							#print(f)
					with open('dbfiles/ideology.csv','r+') as ideologycsv:
    							ideologylist = csv.reader(ideologycsv)	
							for row in ideologylist:
							   ideologycontent = list(row[i] for i in [3])
							   bioguideid = list(row[i] for i in [5])
							   #print("ideology"+str(ideologycontent[0]))
							   #print("bioguide"+str(bioguideid[0])) 
							   						
							   if f == bioguideid[0]:
							
								#print("printing match", f, bioguideid[0], ideologycontent[0])
								ideologyvalue = ideologycontent[0]
					open_file = open(path+filename,'r+').read()
					#print(open_file)
					democratic_speaker = ngrams(open_file,2)
					#print(democratic_speaker)
					with open('dbfiles/2000-democratic-tokens.csv','r+') as csvfile:
    						csvdemocdata = csv.reader(csvfile)
    						for row in csvdemocdata:
    							content = list(row[i] for i in [0])
							speech_tokens=speech_tokens+content
							#print(content[0])
					print('++++++++++++++++++++++++++++++++++++++++')
					#print(type(democratic_speaker))
					for token in democratic_speaker.keys():
						#print(token,"token")
						#print(speech_tokens,"speech_tokens")
						if token in speech_tokens:
							#print(token,democratic_speaker[token])

							v = democratic_speaker[token]/fnotjd
							#print(v,"v")
							predictor_list.append(v)
							response_list.append(ideologyvalue)
							#print("appending",v,ideologyvalue,len(predictor_list),len(response_list))
							
					'''if content[0] in democratic_speaker:
						
						v = democratic_speaker[content[0]]/fnotjd
						predictor_list.append(v)
						f = filename[-11:-4]
						#print(f)
						with open('dbfiles/ideology.csv','r+') as ideologycsv:
    							ideologylist = csv.reader(ideologycsv)	
							for row in ideologylist:
							   ideologycontent = list(row[i] for i in [3])
							   bioguideid = list(row[i] for i in [5])
							   #print("ideology"+str(ideologycontent[0]))
							   #print("bioguide"+str(bioguideid[0])) 
							   response_list = []
							
							   if f == bioguideid[0]:
							
								print("printing match", f, bioguideid[0], ideologycontent[0])
								response_list.append(ideologycontent[0])
								#numpydata = np.array(ideologycontent[0])
								#response_list = numpydata
								#print("response_list", type(response_list))
								#print("predictor_list", type(predictor_list))
								democratic_speaker_frequency.append([f,content[0],democratic_speaker[content[0]],fnotjd,v])'''
					
								
					#print("response_list",response_list,len(response_list))
					#print("predictor_list",predictor_list,len(predictor_list))
					if len(response_list)>0 and len(predictor_list)> 0:
						dc = dict( Y = np.array(response_list), X = np.array(predictor_list) )

						df = pd.DataFrame(dict([ (k,pd.Series(v)) for k,v in dc.items() ]))
						#df = pd.DataFrame({'Y':response_list, 'X':predictor_list})
						#print (df)
						Y = df['Y']  # response
						X = df['X']  # predictor
						#print(X)
						X = sm.add_constant(X,has_constant='add')  # Adds a constant term to the predictor
						#print(df)

						est = sm.OLS(Y.astype(float), X.astype(float))
						est = est.fit()
						print(est.summary())
						#print("params", est.params['X'],est.params['const'])
						coefX = est.params['X']
						coefC = est.params['const']
						##stderrprint("se", est.bse)
						errX = est.bse['X']
						errC = est.bse['const']
						##nobsprint("nobs",est.nobs)
						nobs = est.nobs
						#print("pvalues",est.pvalues['X'],est.pvalues['const'])
						pvaluesX = est.pvalues['X']
						pvaluesC = est.pvalues['const']
						#print("tvalues",est.tvalues['X'],est.tvalues['const'])
						tvaluesX = est.tvalues['X']
						tvaluesC = est.tvalues['const']
						'''with open('dbfiles/democratic_congressman_individual_metrics_inline.csv', 'a+') as csvfile:

    									writer = csv.writer(csvfile)
    									[writer.writerow([filename[0:-4],nobs,coefX,coefC,errX,errC,tvaluesX,tvaluesC,pvaluesX,pvaluesC])]'''


except KeyError:
	print ("File not found Error")

print('***************************************************************')				
# load statsmodels as alias ``sm``

'''
X = sm.add_constant(X, prepend=False)

# Fit and summarize OLS model
mod = sm.OLS(Y, X)

res = mod.fit()'''
'''
endog = df['Y']
exog = sm.add_constant(df[['Y','X']])

# Fit and summarize OLS model
mod = sm.OLS(endog, exog)
res = mod.fit()
'''



#print(speech_tokens)	
#democratic_speaker_frequency=[]
'''
try:
		
			path = 'democratic/'
			
			try:
				for filename in os.listdir(path):
					#print ("Demo")
					open_file = open(path+filename,'r+').read() 
					democratic_speaker = ngrams(open_file,2)

					for tokens in democratic_speaker:
						#print(tokens,democratic_speaker[tokens])
						if str(tokens) in speech_tokens:
							print(filename,tokens,democratic_speaker[tokens])
							v = democratic_speaker[tokens]/fnotjd

							democratic_speaker_frequency.append([filename,tokens,democratic_speaker[tokens],fnotjd,v])
						with open('dbfiles/democratic_congressman_individual_metrics_inline.csv', 'a+') as csvfile:

    							writer = csv.writer(csvfile)
    							[writer.writerow([filename,tokens,democratic_speaker[tokens],fnotjd,democratic_speaker[tokens]/fnotjd])]
						
			except KeyError:
				print ("File not found Error")
				
		

except KeyError:
	print ("Error")
with open('dbfiles/democratic_congressman_individual_metrics.csv', 'w') as csvfile:

    			writer = csv.writer(csvfile)
    			[writer.writerow(r) for r in democratic_speaker_frequency]
'''
'''
#try:
path = 'democratic/'
for i in range(len(speech_tokens)):

			try:

					
					for filename in os.listdir(path):
						r = filename.replace(" ","")
						print(r)
    						if( r != filename):
        						os.rename(filename,r)
						print(path+filename)
						open_file = open(str(path+filename),'r').read() 
						democratic_speaker = ngrams(open_file,2)
						if str(speech_tokens[i]) in democratic_speaker:
							print(filename,speech_tokens[i],democratic_speaker[speech_tokens[i]])
							v = democratic_speaker[speech_tokens[i]]/fnotjd

							##democratic_speaker_frequency.append([filename,tokens,democratic_speaker[tokens],fnotjd,v])
						with open('dbfiles/democratic_congressman_individual_metrics_inline.csv', 'a+') as csvfile:

    							writer = csv.writer(csvfile)
    							[writer.writerow([filename,speech_tokens[i],democratic_speaker[speech_tokens[i]],fnotjd,democratic_speaker[speech_tokens[i]]/fnotjd])]
						
			except KeyError:
				print ("File not found Error")
				
		

#except KeyError:
	#print ("Error")



'''


