from __future__ import absolute_import
import nltk
from nltk.collocations import *

import sys
from ..fdsys.cr_parser import ParseCRFile as pr
from builtins import str
from builtins import object
import psycopg2 as pc
from psycopg2.extras import RealDictCursor as rdc
from ..fdsys.downloader import Downloader as dl
from collections import OrderedDict
import logging
import unicodecsv as csv
import os
from builtins import object
from bs4 import BeautifulSoup
from io import StringIO, BytesIO
import os
from datetime import datetime
import re
import xml.etree.cElementTree as ET
#from .subclasses import crItem
import logging
import itertools
import json
#list of common stop words various languages like the
from stop_words import get_stop_words
#from stemming.porter2 import stem
from porter2stemmer import Porter2Stemmer


def if_exists(key,store):
    if key in list(store.keys()):
        return store[key]
    else:
        logging.warning('{0} not in {1}, returning default value'.format(key,store))
        return None

def rd(astring,delimiter='|'):
    outstr = astring.replace(delimiter,'')
    return outstr

class outStack(object):
    def add(self,a_page):
        self.stack.append(a_page)

    def write(self):
        while self.stack:
            row = self.stack.pop(0)
            self.writer.writerow(row)

    def __init__(self,outpath,fieldnames):
        """
        Stack object for managing rows.
        Args:
            outpath : File path string
            fieldnames : list of field names in order
        """
        self.outfile = open(outpath,'ab')
        self.stack = []
	#print(fieldnames)
        self.writer = csv.DictWriter(self.outfile,fieldnames=fieldnames,
                                     delimiter='|',encoding='utf-8')

    
import re
import string
class crToPG(object):
	

    stemmer = Porter2Stemmer()

    #remove stop words
    def remove_stop_words(self,frequency_list):

	stop_words = []
    	#stop_words = get_stop_words('en')
	with open("stop_words_list.txt") as stop_words_list:
    		#stop_words = [line.split(',') for line in stop_words_list if line.strip()]
		stop_words_data = stop_words_list.read()
		stop_words = stop_words_data.split(',')
	#frequency_list=frequency_list.split(" ")

    	temp_list = []
	stemmer = Porter2Stemmer()
	#print(frequency_list)
	#stop_words.append(stop_words_data)
	#print(stop_words)
	try:
		'''for key in frequency_list:
				#print(key)
        			if key.lower() not in stop_words:
					
    							#print (key)
							
							#key = self.remove_custom_stop_words(key)
							temp_list.append([key])
							
				else:
					print("Removing:"+str(key))'''
		#frequency_list = ' '.join(e for e in frequency_list if e.isalnum())
		for c in string.punctuation:
		   frequency_list= frequency_list.replace(c,"")
		#print(frequency_list)
		querywords = frequency_list.split()
		#querywords = ' '.join(e for e in frequency_list if e.isalnum())
		#print(querywords)

		#for stemwords in querywords:
			#print(stemwords,stemmer.stem(stemwords))

		

		resultwords  = [stemmer.stem(word) for word in querywords if word.lower() not in stop_words]

		result = ' '.join(resultwords)

		#print result

	except ValueError:
	     print "no such value"

	#print(result)    	
	return result
    
    def ngrams(self,inputtext, n):
  		inputtext = inputtext.split()
  		output = {}
  		for i in range(len(inputtext)-n+1):
			#print(inputtext[i:i+n])
    			g = ' \n'.join(inputtext[i:i+n])
    			output.setdefault(g, 0)
    			output[g] += 1
  		return output




    
    def remove_custom_stop_words(self, word_list):
	
	
	#stop_words_lst = ['yo', 'so', 'well', 'um', 'a', 'the', 'you know', 'i mean']
	with open("stop_words_list.txt") as stop_words_list:
    		stop_words_list = [line.split('\n') for line in stop_words_list if line.strip()]
		
		'''for word in stop_words_list:

    			pattern = r'\b'+word[0]+r'\b'

    			word_list = re.sub(pattern, '', word_list)'''

	
	try:
        	if word_list not in stop_words_list:
			return word_list
		else:
			print("Match found:"+str(word_list))
	except ValueError:
	     print "no such value"



    def ingest(self,crfile,pagestack,billstack,speechstack,speechstack1):
        """
        Break a crdoc into three parts
        Pass the appropriate rows for each part
        to the right stack for a bulk insert.
        """
	#print(crfile)
        page_row =  OrderedDict([('pageid',crfile['id']),
                     ('title',rd(crfile['doc_title'])),
                     ('chamber',crfile['header']['chamber']),
                     ('extension',crfile['header']['extension']),
                     ('cr_day',crfile['header']['day']),
                     ('cr_month',crfile['header']['month']),
                     ('cr_year',crfile['header']['year']),
                     ('num',crfile['header']['num']),
                     ('vol',crfile['header']['vol']),
                     ('wkday',crfile['header']['wkday'])
                     

                   ])
        # Add the "page" level to the page stack first
        pagestack.add(page_row)

        bills = []
        if 'related_bills' in list(crfile.keys()):
            for bill in crfile['related_bills']:
                bill_row = OrderedDict([('congress',bill['congress']),
                            ('context',bill['context']),
                            ('bill_type',bill['type']),
                            ('bill_no',bill['number']),
                            ('pageid',crfile['id'])
                            ])
                bills.append(bill_row)

        # Bills for the bill god!
        billstack.add(bills)

        #speeches = []
        ''' for speech in crfile['content']:
            if speech['kind'] == 'speech':
                speechid = crfile['id'] + '-' + str(speech['turn'])
		test = 'test string'
                speech_row = OrderedDict([('speechid',speechid),
                              ('speaker',speech['speaker']),
                              ('speaker_bioguide',speech['speaker_bioguide']),
                              ('pageid',crfile['id']),
                              ('text',rd(speech['text'])),
                              ('turn',speech['turn']),
			      ('party',test)
                             ]) # Gotta get rid of delimiter char
                speeches.append(speech_row)'''
	speeches_republican = []
	speeches_democratic = []
	#speech_row_D =[]
	#speech_row_R =[]
	democratic_data_output=''
	republican_data_output=''

	for speech in crfile['content']:

            if speech['kind'] == 'speech':
                #speechid = crfile['id'] + '-' + str(speech['turn'])
		#test = 'anannya'
		#print(speech)
		
		#print(speech['speaker_bioguide'])
		#print(rd(speech['text']))

		v = str(speech['speaker_bioguide']) + "||" + str(rd(speech['text']))
		'''with open('speeches_test','a+') as out_json:
                    json.dump(v,out_json)'''
		if speech['speaker_bioguide']:
			keybioguideid = speech['speaker_bioguide']
			outpath = os.path.join('','json',keybioguideid+'.json')
			#print(outpath)
			#outpath = 'json\\'+keybioguideid+'.json'
			with open(outpath) as json_data:
    					d = json.load(json_data)
			#print(rd(speech['text']))
			#print('*****************************************************************************************************************************************')
			
			#print(speech_remove_sort_words)
			if d['party']=='D':
				speech_row_D =[]
				
				speech_remove_stop_words =[]
				speech_remove_stop_words = self.remove_stop_words(rd(speech['text']))
    				#print(d['party'])
				#print(speech_remove_stop_words)

				current_speaker_data = '\n' + speech_remove_stop_words
				democratic_data_output =democratic_data_output + current_speaker_data + '\n'
				#print(democratic_data_output)
				'''with open('democratic_speeches.txt','a+') as out_json:
                    			out_json.write(democratic_data_output)'''
				
               			speech_row_D = OrderedDict([
				#('speechid',speechid),
 			        ('affiliation','Affiliation:'+d['party']),
                                ('speaker',speech['speaker']),
                                #('speaker_bioguide',speech['speaker_bioguide']),
			        #('pageid',crfile['id']),
                                ('text',speech_remove_stop_words),
                                #('turn',speech['turn'])
			      
                                ]) 
				speeches_democratic.append(speech_row_D)
				'''if len(speech_remove_stop_words):
					#print(speech_remove_stop_words)
					speeches_democratic.append(speech_row_D)
				else:
					pass
				#print(str(keybioguideid) + "D")'''
				
			     
			elif d['party'] =='R':
			      speech_row_D =[]
			      speech_remove_stop_words =[]
			      speech_remove_stop_words = self.remove_stop_words(rd(speech['text']))

			      #print(speech_remove_stop_words,stemmer.stem(speech_remove_stop_words))
			     
			      current_speaker_data = '\n' + speech_remove_stop_words
			      republican_data_output =republican_data_output + current_speaker_data + '\n'
	
			      '''with open('republican_speeches.txt','a+') as out_json:
                    			out_json.write(republican_data_output)'''
		 	      speech_row_R = OrderedDict([
			      #('speechid',speechid),
 			      ('affiliation','Affiliation:'+d['party']),
                              ('speaker',speech['speaker']),
                              #('speaker_bioguide',speech['speaker_bioguide']),
			      #('pageid',crfile['id']),
			      #('text',''),
                              ('text',speech_remove_stop_words),
                              #('turn',speech['turn'])
			      
                              ])
			      speeches_republican.append(speech_row_R)
			      #print(str(keybioguideid) + "R")
			      '''if len(speech_remove_stop_words):
					#print(speech_remove_stop_words)
			      		speeches_republican.append(speech_row_R)
			      else:
					pass'''
			      
			      
		else:
			keybioguideid = 'dummy'
			#print(str(keybioguideid))
		
		
		
		#pr.find_people(pr(),'','')
		

		
		# SPEECHES FOR THE SPEECH THRONE
		#print(speeches_republican)
		#print(speeches_democratic)
		#print(democratic_data_output)
		
        	speechstack.add(speeches_republican)
		speechstack1.add(speeches_democratic)
	


	#print(democratic_data_output)
	
	
	'''
	line = ""
	open_file = democratic_data_output
	for val in open_file:
    		line += val.lower()
	tokens = line.split()

	bigram_measures = nltk.collocations.BigramAssocMeasures()
	finder = BigramCollocationFinder.from_words(tokens)
	finder.apply_freq_filter(1)
	a = finder.ngram_fd.viewitems()
	democratic_speeches_2_grams=''
	for i, j in a:
  		#print("{0} {1} {2}".format(i[0], i[1], j))
		democratic_speeches_2_grams = str("{0} {1} {2}".format(i[0], i[1],': '+ str(j))) + '\n                                     '
		
		with open('democratic_speeches_2_grams.txt','a+') as out_json:
		    #print(democratic_speeches_2_grams)
                    out_json.write(str(democratic_speeches_2_grams) + '\n')'''
	
	'''democratic_speeches_2_grams = str(self.ngrams(democratic_data_output,2)) + "\n"
	#print(self.ngrams(democratic_data_output,2))
	with open('democratic_speeches_2_grams.txt','a+') as out_json:
                    out_json.write(democratic_speeches_2_grams)'''
	
	
	#republican_speeches_2_grams = str(self.ngrams(republican_data_output,2)) + "\n"
	'''
	repline = ""
	open_file_rep = republican_data_output
	for val in open_file_rep:
    		repline += val.lower()
	tokens = repline.split()

	bigram_measures = nltk.collocations.BigramAssocMeasures()
	finder = BigramCollocationFinder.from_words(tokens)
	finder.apply_freq_filter(1)
	a = finder.ngram_fd.viewitems()
	republican_speeches_2_grams = ''
	for i, j in a:
  		#print("{0} {1} {2}".format(i[0], i[1], j))
		republican_speeches_2_grams = str("{0} {1} {2}".format(i[0], i[1], ': '+ str(j))) + '\n                                     '

	with open('republican_speeches_2_grams.txt','a+') as out_json:
		    #print(republican_speeches_2_grams)
                    out_json.write(str(republican_speeches_2_grams) + '\n')'''
    
    def find_people(self):
        	mbrs = self.doc_ref.find_all('congmember')
        	if mbrs:
            		for mbr in mbrs:
                		self.speakers[mbr.find('name',
                                       {'type':'parsed'}).string] = \
                                       self.people_helper(mbr)  

        
    '''def people_helper(self,tagobject):
        output_dict = {}
        if 'bioguideid' in tagobject.attrs:
            output_dict['bioguideid'] = tagobject['bioguideid']
        elif 'bioGuideId' in tagobject.attrs:
            output_dict['bioguideid'] = tagobject['bioGuideId']
        else:
            output_dict['bioguideid'] = 'None'
        for key in ['chamber','congress','party','state','role']:
            if key in tagobject.attrs:
                output_dict[key] = tagobject[key]
            else:
                output_dict[key] = 'None'
        try:
            output_dict['name_full'] = tagobject.find('name',{'type':'authority-fnf'}).string
        except:
            output_dict['name_full'] = 'None'
	#print(output_dict)
        return output_dict

    # Flow control for metadata generation
    def gen_file_metadata(self):
        # Sometimes the searchtitle has semicolons in it so .split(';') is a nogo
        temp_ref = self.cr_dir.mods.find('accessid', text=self.access_path)
        if temp_ref is None:
            raise RuntimeError("{} doesn't have accessid tag".format(self.access_path))
        self.doc_ref = temp_ref.parent
        matchobj = re.match(self.re_vol, self.doc_ref.searchtitle.string)
        if matchobj:
            self.doc_title, self.cr_vol, self.cr_num = matchobj.group('title','vol','num')
        else:
            logging.warn('{0} yields no title, vol, num'.format(
                self.access_path))
            self.doc_title, self.cr_vol, self.cr_num = \
              'None','Unknown','Unknown'
        self.find_people()
        self.find_related_bills()
        self.find_related_laws()
        self.find_related_usc()
        self.find_related_statute()
        self.date_from_entry()
        self.chamber = self.doc_ref.granuleclass.string
        self.re_newspeaker = self.make_re_newspeaker()
        self.item_types['speech']['patterns'] = [self.re_newspeaker]'''

    def __init__(self,start,**kwargs):
        """
        BE SURE TO INCLUDE do_mode='yield' in kwargs!
        This object handles flow control for new data
        entering a Postgres database using congressionalrecord2s
        data model.

        It breaks the incoming Python dictionaries into three stacks
        of rows, one for each table in this data model.

        It writes the results to each of three flatfiles suitable for
        a bulk update through COPY.

        This is the way to minimize the number
        of transactions to the database, which we want.
        """
        kwargs['do_mode'] = 'yield'
        if 'csvpath' in kwargs:
            pass
        else:
            kwargs['csvpath'] = 'dbfiles'
        pagepath, billpath, speechpath,speechpath1 = [
            os.path.join(kwargs['csvpath'], filename)
            for filename in ['pages.csv','bills.csv','speeches_R.csv','speeches_D.csv']]
        self.downloader = dl(start,**kwargs)
	self.doc_ref = ''
	memberlistfinal = []
	#object1 = congressionalrecord.fdsys.cr_parser.ParseCRDir()
	#print(object1)
        #self.cr_dir = '<congressionalrecord.fdsys.cr_parser.ParseCRDir object at 0x7f0c7c88cb90>'
	#self.cr_dir=cr_dir
	#self.gen_file_metadata()
	#print(pr.find_people(pr(self,'')))
	#self.find_people()
	#print('anannya'+str(pr.memberlist))
	#print(pr('/home/anannyadas/Desktop/congress/congressional-record-master/congressionalrecord/pg_run/fdsys'))
        self.page_fields = ['pageid','title','chamber','extension',
                           'cr_day','cr_month','cr_year','num','vol',
                           'pages','wkday']
        self.bill_fields = ['congress','context',
                            'bill_type','bill_no','pageid']
        #self.speech_fields = ['speechid','affiliation','speaker','speaker_bioguide','pageid','text','turn']
	self.speech_fields = ['affiliation','speaker',
                              'text']
        pagestack = crPages(pagepath,self.page_fields)
        billstack = crBills(billpath,self.bill_fields)
        speechstack = crSpeeches(speechpath,self.speech_fields)
	speechstack1 = crSpeeches(speechpath1,self.speech_fields)
        for crfile in self.downloader.yielded:
	    #print(crfile)
            doc = crfile.crdoc
            self.ingest(doc,pagestack,billstack,speechstack,speechstack1)
           # pagestack.write()
           # billstack.write()
            speechstack.write()
 	    speechstack1.write()
	
	'''with open('democratic_speeches.txt','r+') as json_data:
    		open_file = json.load(json_data)
		print(open_file)'''
	'''open_file = open('democratic_speeches.txt','r+').read() 
	#print(open_file)
	line = ""

	for val in open_file:
    		line += val.lower()
	tokens = line.split()

	bigram_measures = nltk.collocations.BigramAssocMeasures()
	finder = BigramCollocationFinder.from_words(tokens)
	finder.apply_freq_filter(1)
	a = finder.ngram_fd.viewitems()
	democratic_speeches_2_grams=''
	print(sorted(a, key=lambda x: x[1]))
	#print(a)
	for i, j in a:
  		#print("{0} {1} {2}".format(i[0], i[1], j))
		democratic_speeches_2_grams = str("{0} {1} {2}".format(i[0], i[1],': '+ str(j))) + '\n                                     '
		#democratic_speeches_2_grams = "{0} {1} {2}".format(i[0], i[1],j)
		#democratic_speeches_2_grams = democratic_speeches_2_grams.update({str(i[0])+" "+str(i[1]),j}) 
		
		#sorted(democratic_speeches_2_grams, key=lambda x: x[1])
		#print(democratic_speeches_2_grams)
		#democratic_speeches_2_grams_set = set()

		#for details in democratic_speeches_2_grams:
     			#democratic_speeches_2_grams_set.add(details)
		with open('democratic_speeches_2_grams.txt','a+') as out_json:
		    #print(democratic_speeches_2_grams_set)
                    out_json.write(str(democratic_speeches_2_grams))'''
	'''
	repline = ""
	open_file_rep = open('republican_speeches.txt','r+').read() 
	for val in open_file_rep:
    		repline += val.lower()
	tokens_r = repline.split()
	#print(tokens)

	bigram_measures = nltk.collocations.BigramAssocMeasures()
	finder = BigramCollocationFinder.from_words(tokens_r)
	finder.apply_freq_filter(1)
	tokens_r_a = finder.ngram_fd.viewitems()
	#print(type(a))
	
	sorted(tokens_r_a, key=lambda x: x[1], reverse=True)
	#print(tokens_r_a)
	republican_speeches_2_grams = ''
	for i, j in tokens_r_a:
  		#print("{0} {1} {2}".format(i[0], i[1], j))
		republican_speeches_2_grams = str("{0} {1} {2}".format(i[0], i[1], ': '+ str(j))) + '\n                                     '

	with open('republican_speeches_2_grams.txt','a+') as out_json:
		    #print(republican_speeches_2_grams)
                    out_json.write(str(republican_speeches_2_grams))'''
        

class crPages(outStack):
    pass

class crBills(outStack):
    
    def add(self,some_bills):
        self.stack.extend(some_bills)

class crSpeeches(crBills):
    pass
