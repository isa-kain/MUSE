from __future__ import with_statement
import sys
import time

try:
    import numpy
    import reflex
    from pipeline_product import PipelineProduct
    from reflex_plot_widgets import *
    import pipeline_display
    import_success = True

except ImportError:
    import_success = False
    print("Error importing modules pyfits, wx, matplotlib, numpy")
    

def paragraph(text, width=None):
    """ wrap text string into paragraph
       text:  text to format, removes leading space and newlines
       width: if not None, wraps text, not recommended for tooltips as
              they are wrapped by wxWidgets by default
    """
    import textwrap
    if width is None:
        return textwrap.dedent(text).replace('\n', ' ').strip()
    else:
        return textwrap.fill(textwrap.dedent(text), width=width)


class DataPlotterManager(object):
    """
    This class must be added to the PipelineInteractiveApp with setPlotManager
    It must have following member functions which will be called by the app:
     - setInteractiveParameters(self)
     - readFitsData(self, fitsFiles):
     - addSubplots(self, figure):
     - plotProductsGraphics(self)
    Following members are optional:
     - setWindowHelp(self)
     - setWindowTitle(self)
     - setCurrentParameterHelper(self, helper)
    """

    # static members
    recipe_name = "muse_zap_crea_mask"
    mask_cat = "MASK_FINAL_CUBE"
    fov_cat_sky = "IMAGE_SKY"
    fov_cat_obj = "IMAGE_OBJECT"

    def setInteractiveParameters(self):
        """
        This function specifies which are the parameters that should be presented
        in the window to be edited.  Note that the parameter has to also be in the
        in_sop port (otherwise it won't appear in the window). The descriptions are
        used to show a tooltip. They should match one to one with the parameter
        list.
        """

        # the parameters we send to the GUI depends on whether the in_sof_rec_orig includes a mask or not:
        mask_was_provided= False
        for file in self.orig_files:
            if file.category[0:4] == "MASK":
                    mask_was_provided= True

        if mask_was_provided:
          parlist= [
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="use_input_mask",
                                   group="Mask", description="How to use the user supplied mask: use= use the mask without modification;  add= add n_grow and skyregions to mask (parameters are specified in the 'Flux Ranges' and 'Sky Regions' tabs); discard=discard mask and use parameters in the 'Flux Ranges' and 'Sky Regions' tabs to create a new mask."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="min_frac",
                                   group="Flux Ranges", description="Fraction of faintest pixel NOT to use as sky. If N is the total number of pixels, then the N*min_frac faintest pixels will NOT be considered as sky."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="max_frac",
                                   group="Flux Ranges", description="Fraction of faint pixels to use as sky. If N is the total number of pixels, then the N*max_frac faintest pixels will be candidates for being a sky pixel. Note the parameter min_frac that can be used to exclude the very faintest pixels. "),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="n_grow",
                                   group="Flux Ranges", description="Add a region of ngrow pixels about any pixel found as sky using the flux range method."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_1",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_2",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_3",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_4",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_5",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_6",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_7",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_8",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_9",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_10",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax.")
            ]
        else:
          parlist= [
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="min_frac",
                                   group="Flux Ranges", description="Fraction of faintest pixel NOT to use as sky"),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="max_frac",
                                   group="Flux Ranges", description="Fraction of faint pixel to use as sky."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="n_grow",
                                   group="Flux Ranges", description="Add a region of ngrow pixels about any pixel found as sky using the flux percentile method."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_1",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_2",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_3",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_4",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_5",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_6",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_7",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_8",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_9",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax."),
            reflex.RecipeParameter(recipe=self.recipe_name, displayName="sky_region_10",
                                   group="Sky Regions", description="Sky box specified as xmin xmax ymin ymax.")
            ]
        return parlist

    def readFitsData(self, fitsFiles):
        """
        This function should be used to read and organize the raw fits files
        produced by the recipes.
        It receives as input a list of reflex.FitsFiles
        """
        # organize the files into a dictionary, here we assume we only have 
        # one file per category if there are more, one must use a
        # dictionary of lists
        self.frames = dict()
        for f in fitsFiles:
            self.frames[f.category] = PipelineProduct(f)

        # we only have two states, we have data or we don't
        # define the plotting functions we want to use for each
        if (self.mask_cat in self.frames and self.fov_cat_sky in self.frames) or (self.mask_cat in self.frames and self.fov_cat_obj in self.frames):
            self._add_subplots = self._add_subplots
            self._plot = self._data_plot
        else:
            self._add_subplots = self._add_nodata_subplots
            self._plot = self._nodata_plot

    def addSubplots(self, figure):
        """
        This function should be used to setup the subplots of the gui.  The the
        matplotlib documentation for a description of subplots.
        """
        self._add_subplots(figure)

    def plotProductsGraphics(self):
        """
        This function should be used to plot the data onto the subplots.
        """
        self._plot()

    def setWindowHelp(self):
        return 'Help for mask interactive window'

    def setWindowTitle(self):
        return 'mask creation interactive window'

    def _add_nodata_subplots(self, figure):
        self.txt_plot = figure.add_subplot(111)

    def _add_subplots(self, figure):
        self.img_plot = figure.add_subplot(211)
        self.skyimg_plot = figure.add_subplot(212)

    def _data_plot(self):
        # get the right category file from our dictionary
        try:
         fov = self.frames[self.fov_cat_obj]
        except:
         fov = self.frames[self.fov_cat_sky]
    #    try: 
    #     fov = self.frames[self.fov_cat_sky]

        fov.readImage(1)
        mask = self.frames[self.mask_cat]
        mask.readImage()

        imgdisp = pipeline_display.ImageDisplay()
        imgdisp.setLabels('X', 'Y')
        imgdisp.setAspect('equal')
        tooltip_sky = paragraph("""\
        White light image of the cube.
        A greyscale is used for pixels 
        that will be used as sky, the fluxes
        of all other pixels are marked in 
        red.
        """)
        tooltip_obj = paragraph("""\
        Different representation of the same image as 
        above. The roles of the grey scale and red
        pixels are reversed:
        Sky pixels are marked in red.  A greyscale is 
        used for all other pixels.
        """)
        imgdisp.display(self.img_plot, "Sky", tooltip_sky, fov.image, bpmimage=mask.image)
        imgdisp.display(self.skyimg_plot, "Object", tooltip_obj, fov.image, bpmimage=(1-mask.image))

    def _nodata_plot(self):
        # could be moved to reflex library?
        self.txt_plot.set_axis_off()
        text_nodata = "Data not found. Input files should contain these" \
                       " types:\n%s" % self.img_cat
        self.txt_plot.text(0.1, 0.6, text_nodata, color='#11557c',
                      fontsize=18, ha='left', va='center', alpha=1.0)
        self.txt_plot.tooltip = 'No data found'


    def increaseFloatNumber(self, point) :
        floatpoint = self.getCurrentParameterHelper('floatopt') + 1
        new_params = list()
        new_params.append(reflex.RecipeParameter(self.recipe_name,'floatopt',
                                                 value=str(floatpoint)))
        return new_params

    def setCurrentParameterHelper(self, helper) :
        self.getCurrentParameterHelper = helper

#This is the 'main' function
if __name__ == '__main__':
    from reflex_interactive_app import PipelineInteractiveApp

    # Create interactive application
    interactive_app = PipelineInteractiveApp(enable_init_sop=True)

    # get inputs from the command line
    interactive_app.parse_args()

    #Check if import failed or not
    if not import_success:
        interactive_app.setEnableGUI(False)

    #Open the interactive window if enabled
    if interactive_app.isGUIEnabled():
        #Get the specific functions for this window
        dataPlotManager = DataPlotterManager()

        #the dataPlotManager uses the in_sof_rec_orig for deciding which
        #parameters to offer for plotting:
        dataPlotManager.orig_files= interactive_app.inputs.in_sof_rec_orig.files

        interactive_app.setPlotManager(dataPlotManager)
        interactive_app.showGUI()
    else:
        interactive_app.set_continue_mode()

    #Print outputs. This is parsed by the Reflex python actor to
    #get the results. Do not remove
    interactive_app.print_outputs()
    
    sys.exit()
 
