#!/usr/bin/env python

__author__ = "Isabel Kain"
__email__ = "ijkain@ucsc.edu"

'''
Generate one .sof ("set of frames") as input for recipe muse_scibasic.
Read in and parse XML file trees; use this to assemble space-delimited 
.sof file containing paths to each file needed in reduction recipe 
(first column) and the category of the corresponding file (second column)
'''

import numpy as np
import pandas as pd
from astropy.io import fits
import glob
import os
from bs4 import BeautifulSoup 


## Define paths
basepath = '/Volumes/Seagate/MUSE'
datapath = f'{basepath}/raw_data'
sofpath = f'{basepath}/sof'
prepath = f'{basepath}/scibasic_out'
postpath = f'{basepath}/scipost_out'
calpath = '/Users/isabelkain/Desktop/MUSE/share/esopipes/datastatic/muse-2.8.5'



if __name__ == "__main__":
    
    ## Grab XML filetrees (point to files required for data processing)
    xml_files = glob.glob(f'{datapath}/*.xml*', recursive=True)

    ## Keep track of original XML for each SOF
    fname_key = pd.DataFrame(columns=['Filename', 'Tag'])
    
    

	for i, xml in enumerate(xml_files):
        
        ## Save name of XML file, number tag
        fname_key = fname_key.append({'Filename':xml.split('/')[-1].split('.xml')[0], 'Tag':i+1}, ignore_index=True)

	    ## Open, parse XML file
	    with open(xml_files[i], 'r') as f:
	        data = f.read()
	        f.close()

	    bs_data = BeautifulSoup(data, 'xml')

	    ## Pull filenames and types from XML tree, save to pandas DF
	    filenames = []
	    filetypes = []

	    filedivs = bs_data.find_all('file')

	    for line in filedivs:

	        ## Pull filename and type from XML tree
	        catg = line['category']
	        name = line['name'].replace(':', '_')

	        ## Use XML filename to search for actual filename in data directory
	        ## (there are some weird formatting differences)
	        find_name = glob.glob(f'{datapath}/*{name}*.fits*')

	        if len(find_name) != 1:
	            print('ERROR: more than 1 filename match.', find_name)
	            continue

	        filenames.append(find_name[0])
	        filetypes.append(catg)

	    sofDF = pd.DataFrame(np.array([filenames, filetypes]).T, columns=('Filenames', 'Filetypes'))
	    
	    
	    ## Drop multiple frames for ILLUM, ___ (max allowed: 1 per image)
	    sofDF.drop_duplicates(subset='Filetypes', inplace=True, ignore_index=True)
	    

	    ## Check if all files the XML tree points to can been found
	    check = []

	    for pth in sofDF['Filenames']:
	        check.append(os.path.exists(pth))

	    print('All pointer files found in', xml_files[i], np.all(check))

	    ## Write .sof file if passes check
	    if check:
	        sofDF.to_csv(f'{sofpath}/scibasic_{i+1}.sof', header=None, index=None, sep=' ')


    ## Write fnames_key
    fname_key.to_csv(f'{basepath}/filenames_key.txt')
    
    
	print('Done.')
	        