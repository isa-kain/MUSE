import reflex 
import sys 
import os
import json
from optparse import OptionParser
message=''
import_ok =1
try: 
    import astropy
    from astropy.io import fits as pyfits
    from astropy import wcs as pywcs
    from astropy.io import fits
except ImportError:
    import_ok = 0
    message=message+'  astropy is missing\n'
try: 
    import numpy as np
    from numpy import array 
except ImportError:
    import_ok = 0
    message=message+'  numpy is missing\n'
#try:
#    import Topolino
#except ImportError:
#    import_ok = 0
#    message=message+'  Topolino is missing\n'
#try:
#    import Pippo
#except ImportError:
#    import_ok = 0
#    message=message+'  Pippo is missing\n'
try:
    import scipy
except ImportError:
    import_ok = 0
    message=message+'  scipy is missing\n'
    
try:
    from sklearn.decomposition import PCA
except ImportError:
    import_ok = 0
    message=message+'  scikit learn missing\n'
    

if __name__ == '__main__':

#   MAIN MODULE ---

    
  parser = reflex.ReflexIOParser()
  parser.add_option("-i", "--trigger", dest="trigger")
  parser.add_output("-p", "--messages", dest="messages")
  #this line is needed but never used
  #parser.add_input("-v", "--fits_viewer", dest="fits_viewer", type='string', default="fv")
 # print 'this is it',sys.argv
  inputs  = parser.get_inputs()
  outputs = parser.get_outputs()
 
  
  if import_ok == 0: # in the case import fails
      outputs.messages = ('Warning: Failing in importing some required Python modules:\n')+message+('\nCheck software requirements '
            ' https://www.eso.org/sci/software/pipelines/reflex_workflows/#software_prerequisites\n'
            ' in the MUSE Reflex tutorial.\n'
            ' Terminating workflow execution.\n ')
      
  if import_ok == 1: # in the case import is fine
      outputs.messages = ('ok')
 
  parser.write_outputs()
  sys.exit()
 


