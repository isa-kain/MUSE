#!/usr/bin/env python

__author__ = "Isabel Kain"
__email__ = "ijkain@ucsc.edu"

'''
Generate one .sof ("set of frames") as input for recipe muse_scipost
Read in and parse XML file trees; use this to assemble space-delimited 
.sof file containing paths to each file needed in reduction recipe 
(first column) and the category of the corresponding file (second column)
'''

import numpy as np
import pandas as pd
from astropy.io import fits
import glob
import os, re
from bs4 import BeautifulSoup 


## Define paths
basepath = '/Volumes/Seagate/MUSE'
datapath = f'{basepath}/raw_data'
sofpath = f'{basepath}/sof'
prepath = f'{basepath}/scibasic_out'
postpath = f'{basepath}/scipost_out'
calpath = '/Users/isabelkain/Desktop/MUSE/share/esopipes/datastatic/muse-2.8.5'



if __name__ == "__main__":
    
    ## Grab folders [...]/sofN/[scibasic output for scibasic_N.sof]
    folders = glob.glob(f'{prepath}/*/', recursive=False)

    if len(folders)==0:
        raise ValueError('Preprocessing output not found/in unexpected file structure. Organize files or run muse_scibasic.')

        
    ## For each observation, create scipost_N.sof
    for fld in folders:

        ## Determine file number

        fstr = fld.split('/')[-2]
        num = int(re.findall(r'\d+', fstr)[0])


        ## Copy contents of scibasic_N.sof

        df = pd.read_csv(f'{sofpath}/scibasic_{num}.sof', delimiter=' ', names=['Filename', 'Type'])


        ## Add PIXELTABLES (output from muse_scibasic)

        pxtables = glob.glob(f'{folders[0]}*PIXTABLE*.fits')

        if len(pxtables)==0:
            raise ValueError('Preprocessing output not found. Please run muse_scibasic.')

        newrows = pd.DataFrame({'Filename':pxtables, 'Type':['PIXTABLE_OBJECT']*len(pxtables)})
        df = df.append(newrows, ignore_index=True)


        ## Add LSF_PROFILE (need to dynamically create, or ok to use static calib?)
        if np.sum(df['Type']=='LSF_PROFILE')==0:
            df = df.append({'Path':f'{calpath}/lsf_profile_slow_nfm-ao-n.fits', 'Type':'LSF_PROFILE'}, ignore_index=True)

        ## Add ASTROMETRY_WCS
        if np.sum(df['Type']=='ASTROMETRY_WCS')==0:
            df = df.append({'Path':f'{calpath}/astrometry_wcs_nfm.fits', 'Type':'ASTROMETRY_WCS'}, ignore_index=True)

        ## Add FILTER_LIST
        if np.sum(df['Type']=='FILTER_LIST')==0:
            df = df.append({'Path':f'{calpath}/filter_list.fits', 'Type':'FILTER_LIST'}, ignore_index=True)

        ## Add SKY_LINES
        if np.sum(df['Type']=='SKY_LINES')==0:
            df = df.append({'Path':f'{calpath}/sky_lines.fits', 'Type':'SKY_LINES'}, ignore_index=True)

        ## Add EXTINCT_TABLE
        if np.sum(df['Type']=='EXTINCT_TABLE')==0:
            df = df.append({'Path':f'{calpath}/extinct_table.fits', 'Type':'EXTINCT_TABLE'}, ignore_index=True)

        ## Add STD_RESPONSE
        if np.sum(df['Type']=='STD_RESPONSE')==0:
            df = df.append({'Path':f'{calpath}/std_response_nfm-ao-n.fits', 'Type':'STD_RESPONSE'}, ignore_index=True)

        ## Save as scipost_N.sof
        df.to_csv(f'{sofpath}/scipost_{num}.sof', header=None, index=None, sep=' ')
        
    print('Done.')
