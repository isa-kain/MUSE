try:
 import esorexplugin
 from esorexplugin import *
 from astropy.io import fits
except ImportError:
    import_ok = 0

import logging    
import sys   
import numpy as np 

try:
  from astropy.io import fits as pyfits
  from scipy.ndimage.filters import minimum_filter
except ImportError:
    import_ok = 0

 
logging.basicConfig(format='[%(levelname)s] %(message)s', level=logging.INFO,
                    stream=sys.stdout)
logger = logging.getLogger(__name__)
 
# Must define a new class deriving from esorexplugin.RecipePlugin to write a
# new Python based recipe plugin for EsoRex.

def crea_mask(image,min,max):

        try:
            where_are_NaNs = np.isnan(image)
            
            vector = image[~where_are_NaNs]
            num = len(vector)
            vector.sort()
            sel = vector[int(num*min) : int(num*max)]
            min_sel = sel.min()
            max_sel = sel.max()
            mask =  np.where((image >=min_sel) & (image<= max_sel) ,0,1)
            
        except:
            logger.warning('could not apply limits')
            mask= image * 0 + 1
            
        mask[where_are_NaNs] = 1
        
        return mask


def add_neibors(image,n_grow):
        mask= minimum_filter(image, size=n_grow)
        return mask
    
def add_skyboxes(image,skyboxes):
        ydim= len(image)
        xdim= len(image[0])
        for skybox in skyboxes:
            try:
                junk = skybox.replace('"', ' ')
                xmin, xmax, ymin, ymax = map(int,junk.split())
                if xmin >= 0 and xmax <= xdim and ymin >= 0 and ymax <= ydim:
                    image[ymin:ymax,xmin:xmax]= 0
            except:
                self.raise_error("can't use skybox " + skybox)
  
try:
 class CreaMaskRecipe(esorexplugin.RecipePlugin):

    # The recipe must be given a name by setting the 'name' class attribute.
    name = "muse_zap_crea_mask"     

    # A version needs to be assigned. For this it is best to use the
    # VersionNumber method that will correctly encode a major, minor and micro
    # version sub-numbers. NOTE: minor and micro must be in the range [0..99].
    version = VersionNumber(2, 0, 0)

    # A synopsis string should be given for the recipe for the man page created
    # by EsoRex.
    # NOTE: If the following class attribute is not provided, the first line of
    # this class's doc string will be used for the synopsis instead.
    synopsis = "Create or modify sky mask; idendify the sky regions to compute principal components"

    # A detailed description string should be given for the recipe for the man
    # page created by EsoRex.
    # NOTE: If the following class attribute is not provided, the whole text in
    # the class's doc string will be used for the description instead.
    description = ("Create or modify sky mask; idendify the sky regions to compute principal components"
                   "This recipe It is part of a series of contributed recipes and it is meant to be used within the muse_zap \n"
                   "ESOreflex workflow. It is not part of the official MUSE pipeline.\n"
                   "For any question related to this recipe, please contact sdp_muse@eso.org.\n")
    # The following additional fields should be provided for the man page
    # generated by EsoRex.
    author = "Lodovico Coccato"
    email = "sdp_muse@eso.org"
    
    copyright = EsoCopyright("MUSE_ZAP", [2010, 2013, 2014, 2015, 2017])

 
    names=["min_frac",
           "max_frac",
           "n_grow",
           "use_input_mask",
           "sky_region_1",
           "sky_region_2",
           "sky_region_3",
           "sky_region_4",
           "sky_region_5",
           "sky_region_6",
           "sky_region_7",
           "sky_region_8",
           "sky_region_9",
           "sky_region_10"]

    alias=["min_frac",
           "max_frac",
           "n_grow",
           "use_input_mask",
           "sky_region_1",
           "sky_region_2",
           "sky_region_3",
           "sky_region_4",
           "sky_region_5",
           "sky_region_6",
           "sky_region_7",
           "sky_region_8",
           "sky_region_9",
           "sky_region_10"]
    fullnames = list(map(lambda x: "muse_zap.muse_zap_crea_mask."+x, names))
     
    desc1= "Fraction of the image to be ignored"
    desc2= "Fraction of the image (without the ignored part) to be considered as sky"
    desc3= "number of adjacent pixels about each sky pixel to add to sky region"
    desc4= "Specification how to use the input mask if provided: 'use'= use as is; 'add'= use it and add to it according to parameters n_grow and skyregions to mask; 'discard'= use parameters n_grow and skyboxes to create completely new mask"
    desc5= "sky region 1 specified as xmin xmax ymin ymax"
    desc6= "sky region 2 specified as xmin xmax ymin ymax"
    desc7= "sky region 3 specified as xmin xmax ymin ymax"
    desc8= "sky region 4 specified as xmin xmax ymin ymax"
    desc9= "sky region 5 specified as xmin xmax ymin ymax"
    desc10= "sky region 6 specified as xmin xmax ymin ymax"
    desc11= "sky region 7 specified as xmin xmax ymin ymax"
    desc12= "sky region 8 specified as xmin xmax ymin ymax"
    desc13= "sky region 9 specified as xmin xmax ymin ymax"
    desc14= "sky region 10 specified as xmin xmax ymin ymax"

   
    #names=map(lambda x: 'muse_zap.muse_zap_crea_mask.'+x,alias)
    parameters = [
        RangeParameter(fullnames[0],0.1,0.0,1.0,cli_alias=alias[0],cli_enabled=True, description=desc1),
        RangeParameter(fullnames[1],0.7,0.0,1.0,cli_alias=alias[1],cli_enabled=True, description=desc2),
        ValueParameter(fullnames[2],3,cli_alias=alias[2],cli_enabled=True, description=desc3),
        EnumParameter(fullnames[3], "use", ["use", "add", "discard"],cli_alias=alias[3],cli_enabled=True, description=desc4),
        ValueParameter(fullnames[4],'-1 -1 -1 -1',cli_alias=alias[4],cli_enabled=True, description=desc5),
        ValueParameter(fullnames[5],'-1 -1 -1 -1',cli_alias=alias[5],cli_enabled=True, description=desc6),
        ValueParameter(fullnames[6],'-1 -1 -1 -1',cli_alias=alias[6],cli_enabled=True, description=desc7),
        ValueParameter(fullnames[7],'-1 -1 -1 -1',cli_alias=alias[7],cli_enabled=True, description=desc8),
        ValueParameter(fullnames[8],'-1 -1 -1 -1',cli_alias=alias[8],cli_enabled=True, description=desc9),
        ValueParameter(fullnames[9],'-1 -1 -1 -1',cli_alias=alias[9],cli_enabled=True, description=desc10),
        ValueParameter(fullnames[10],'-1 -1 -1 -1',cli_alias=alias[10],cli_enabled=True, description=desc11),
        ValueParameter(fullnames[11],'-1 -1 -1 -1',cli_alias=alias[11],cli_enabled=True, description=desc12),
        ValueParameter(fullnames[12],'-1 -1 -1 -1',cli_alias=alias[12],cli_enabled=True, description=desc13),
        ValueParameter(fullnames[13],'-1 -1 -1 -1',cli_alias=alias[13],cli_enabled=True, description=desc14)
        ]
 
    # The set_frame_group method must be overriden by a new recipe plugin. The
    # purpose of this method is to catalogue the input frames to the recipe.
    # Primarily to indicate to EsoRex if an input frame is a raw frame or
    # calibration frame.



    def set_frame_group(self, frame):
        if frame.tag == 'RAW':
            frame.group = Frame.GROUP_RAW
        else:
            frame.group = Frame.GROUP_CALIB
        frame.level = Frame.LEVEL_FINAL



    # A method called process must be defined that will take as first argument
    # 'frames', which is a list of Frame objects. Initially 'frames' contains
    # the input Frames from the SOF. As the process method executes it should
    # append any new output Frame objects to the 'frames' list.
    # The additional attributes should be the parameters as declared in the
    # 'parameters' class attribute above. There should be as many method
    # arguments as there were parameters declared and their names must
    # correspond.
#    def process(self, frames, par1, par2, par3):
  #  def process(self, frames,**kwargs):
    def process(self, frames,min_frac,max_frac,n_grow,use_input_mask,sky_region_1,
                sky_region_2,sky_region_3,sky_region_4,sky_region_5,
                sky_region_6,sky_region_7,sky_region_8,sky_region_9,sky_region_10):

        
        values=[min_frac,max_frac,n_grow,use_input_mask,sky_region_1,
                sky_region_2,sky_region_3,sky_region_4,sky_region_5,
                sky_region_6,sky_region_7,sky_region_8,sky_region_9,sky_region_10]
        
       

        # This will rarely be needed, but just in case, have more detailed
        # parameter information in self.input_parameters.
        # This is a list of RecipeParameter objects.
        # print(self.input_parameters)
        # self.input_parameters
        # Errors should be raised with the raise_error method.
        # Its signature is:
        #   def raise_error(self, message, exitcode = 1, print_traceback = False)
        if len(frames) < 1:
            self.raise_error("Need at least one input frame.")


        skyboxes= []

        if min_frac < 0:
            min_frac = 0.0
        if max_frac > 1:
            max_frac = 1.0
                    
        if min_frac < 0 and max_frac > 1:
            self.raise_error('Warning')
                        
        if min_frac < 0 and max_frac < 0:
            self.raise_error('Warning')
                            
        if min_frac > 1 and max_frac >1:
            self.raise_error('Warning')
                                
        if min_frac > max_frac:
            min_frac = 0.0
            max_frac = 0.0
            self.raise_error('Warning')
                                    
            
        skyboxes.append(sky_region_1)
        skyboxes.append(sky_region_2)
        skyboxes.append(sky_region_3)
        skyboxes.append(sky_region_4)
        skyboxes.append(sky_region_5)
        skyboxes.append(sky_region_6)
        skyboxes.append(sky_region_7)
        skyboxes.append(sky_region_8)
        skyboxes.append(sky_region_9)
        skyboxes.append(sky_region_10)

        bad_parameters = 0
        #read input images
        input_mask_provided = False
        for j in range(0,len(frames)):
            if frames[j].tag == 'IMAGE_OBJECT' or frames[j].tag == 'IMAGE_SKY':
                hdu_fov  = fits.open(frames[j].filename,checksum=True)
                image_fov = hdu_fov[1].data 
        #input_mask_provided = False
            if frames[j].tag == 'MASK_OBJECT' or frames[j].tag == 'MASK_SKY':
                input_mask_name = frames[j].filename#file.name
                hdu  = fits.open(input_mask_name,checksum=True)
                input_mask = hdu[0].data
                input_mask_provided = True

        if input_mask_provided and  use_input_mask[0] == 'u':
           input_mask_name = output_name = 'MASK_FINAL_CUBE.fits'
           output_mask = Frame(input_mask_name, "MASK_FINAL_CUBE")
           output_frames = []
           output_frames.append(output_mask)
           output_mask.write(hdu, overwrite = True, output_verify = 'ignore',checksum=True)
           
        else: 
           if input_mask_provided and  use_input_mask[0] == 'a':
                first_mask= input_mask
           else:
                first_mask = crea_mask(image_fov,min_frac,max_frac)
                mask = image_fov
           mask = add_neibors(first_mask, n_grow)
           add_skyboxes(mask, skyboxes)
 

           #check for bad parameters and create a fake mask + error message
           if np.min(mask) != 0 or np.max(mask) !=1:
              mask=mask*0.0+1.
              mask[0]= 0
              self.raise_error('Warning, invalid mask created. Check the recipe parameters')
           # create output mask:
           
           output_name = 'MASK_FINAL_CUBE_'+str(j+1)+'.fits'
           hdu2 = fits.PrimaryHDU(mask)
           hdulist = fits.HDUList([hdu2])
           hdulist[0].header = hdu_fov[0].header
           hdulist[0].header['HIERARCH ESO PRO CATG'] = 'MASK_FINAL_CUBE'
           hdulist[0].data=mask


           
           #Write recipe parameters in the output header
           for k in range(len(self.input_parameters)):
              
               hkey1 = 'HIERARCH ESO MASK REC'+str(k+1)+' NAME'
               hkey2 = 'HIERARCH ESO MASK REC'+str(k+1)+' VALUE'
               hdulist[0].header[hkey1] = self.alias[k]
               hdulist[0].header[hkey2] = values[k]
               
           output_mask = Frame(output_name, "MASK_FINAL_CUBE")

        # New output frames are created with the Frame object's constructor.
        # The signature for the constructor method is:
        #   def __init__(self, filename, tag, type = None, group = None, level = None)
       
        # Make sure to add the new frames to the 'frames' list to register them
        # with EsoRex.
           output_frames = []
           output_frames.append(output_mask)

        # One can write the associated FITS file with the frame's write method.
        # It accepts HDUList objects and additional.
           output_mask.write(hdulist, overwrite = True, output_verify = 'ignore',checksum=True)

        return output_frames
except:
 fail=1