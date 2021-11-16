# ========================================================================================================
#
# Lodovico Coccato & John Pritchard
#
# A python script to check that the specified python modules are available
# Two types of modules can be specified:
#
#   processing_modules: requried for correct execution of the workflow
#                       if any are missing the workflow is stopped.
#
#   interactive_modules: required for certain interactive features, but
#                        the workflow can run correctly without them
#
# ========================================================================================================


import reflex 
import sys
import os
import json
from optparse import OptionParser
import time
from importlib import import_module
message_r='The following python packages needed for reduction are missing:\n'
message_i='The following python packages needed for interactivity are missing:\n'
import_r=False
import_i=False

def findOneoOf(module_list) :
    foundOne=False
    for om in m.split("|") :
        try :
            cm=om.split(":")
            print(("importing %s" %(cm[0])))
            mod=import_module(cm[0])
            if len(cm) > 1  : print(("checking for klass %s" %(cm[1])))
            klass=getattr(mod, cm[1]) if len(cm) > 1 else None
            foundOne=True
            print(('found %s' %(om)))
        except :
            print(('FAILED to find %s' %(om)))
            pass
    return foundOne

if __name__ == '__main__':

#   MAIN MODULE ---

    parser = reflex.ReflexIOParser()
    parser.add_option("-i", "--interactive_mode", dest="interactive_mode", type='string')
    parser.add_option("-R", "--processing_modules", dest="processing_modules", type='string')
    parser.add_option("-I", "--interactive_modules", dest="interactive_modules", type='string')
    parser.add_output("-m", "--message", dest="message")
    parser.add_output("-t", "--trigger", dest="trigger")
    inputs  = parser.get_inputs()
    outputs = parser.get_outputs()

    ## Python Modules required for processing...
    if not inputs.processing_modules == '' :
        for m in inputs.processing_modules.split(',') :
            try:
                if not findOneoOf(m) : raise ImportError
            except ImportError :
                import_r=True
                message_r=message_r+' %s\n' %(m)
            except Exception as e:
                raise e

    ## Python Modules required for interactivity but not for processing...
    if not inputs.interactive_modules == '' :
        for m in inputs.interactive_modules.split(',') :
            try:
                if not findOneoOf(m) : raise ImportError
            except ImportError :
                import_i=True
                message_i=message_i+' %s\n' %(m)
            except Exception as e:
                raise e

    message='' 
    if import_i :
        message = message + message_i+(' \n')

    if import_r :
        message = message + message_r+(' \n')
        message = message+('The workflow will stop')
    
    if not import_r and import_i :
        message = message+('The workflow will continue but interactivity features will not work')

    if not import_r :
        outputs.trigger = True

    #broadcast the output message only if a module is missing.
    if import_i or import_r : 
        outputs.message=message

    if inputs.interactive_mode.lower() == 'false' and not message == '' :
        print(message)


    parser.write_outputs()
    sys.exit()
 


      
