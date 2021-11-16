from __future__ import with_statement
from __future__ import print_function

import sys
import os
import types

import_success = True
try:
    import numpy as np
    import reflex
    import pipeline_display
    import pipeline_product
    import reflex_plot_widgets as reflex_widgets
except ImportError:
    import_success = False
    print("Error importing reflex modules, numpy")


def get_inherited_key(fits_datacontainer, fits_dataset, key):
    value = None
    idx = None
    try:
        idx = fits_dataset
        value = fits_datacontainer.all_hdu[idx].header[key]
    except KeyError:
        # ESO DICB standard: Keyword inheritance is applied unless it is
        # explicitly disabled.
        inherit = True
        try:
            inherit = fits_datacontainer.all_hdu[idx].header["INHERIT"]
            if not isinstance(inherit, bool):
                inherit = True
        except KeyError:
            pass
        if inherit == True:
            try:
                idx = 0
                value = fits_datacontainer.all_hdu[idx].header[key]
            except KeyError:
                raise

    return (value, idx)


def format_coordinates(data, xpos, ypos):
    formatted_string = ""
    (img_data, wcs) = data

    xpix = int(xpos)
    ypix = int(ypos)
    x_inside = xpos > 0 and xpos <= img_data.image.shape[1]
    y_inside = ypos > 0 and ypos <= img_data.image.shape[0]
    if x_inside and y_inside:
        alpha, delta = wcs.image_to_celestial(xpos, ypos)
        formatted_string = ("%s alpha=%.4f delta=%.4f" +
                            "     Image X=%.3f  Y=%.3f" +
                            "     Value=%.3f") % \
            (wcs.radecsys, alpha, delta, xpos + 1., ypos + 1.,
             img_data.image[ypix, xpix])

    return formatted_string


def read_table(frame, columns, dataset=1):
    table = dict()
    for name in columns:
        try:
            table[name] = frame.all_hdu[dataset].data.field(name)
        except KeyError:
            table = None
            print("Warning: column '" + name + "' not found in " +
                  frame.fits_file.name)
        if not table:
            break
    return table


def cycle_marker_properties(index, colors, symbols):
    ncolors = len(colors)
    nsymbols = len(symbols)

    color = colors[index % ncolors]
    symbol = symbols[(index // ncolors) % nsymbols]
    return (symbol, color)


class WorldCoordinateSystem(object):
    def __init__(self, fits_datacontainer, fits_dataset=0, tiny=1.e-6):
        self.tiny = tiny
        self.ctype = None
        self.crval = None
        self.crpix = None
        self.cdelt = None
        self.pc_matrix = None
        self.cd_matrix = None

        try:
            _equinox = get_inherited_key(
                fits_datacontainer, fits_dataset, "EQUINOX")
        except KeyError:
            _equinox = None

        try:
            _radecsys = get_inherited_key(
                fits_datacontainer, fits_dataset, "RADESYS")
        except KeyError:
            try:
                _radecsys = fits_datacontainer.all_hdu[fits_dataset].header["RADECSYS"]
            except KeyError:
                _radecsys = None

        if _equinox is not None:
            equinox = _equinox[0]
        else:
            equinox = None

        if _radecsys is not None:
            radecsys = _radecsys[0]
        else:
            radecsys = None

        if equinox is not None:
            if radecsys is None:
                if equinox < 1984.0:
                    self.radecsys = "FK4"
                else:
                    self.radecsys = "FK5"
            else:
                self.radecsys = radecsys.strip()
        else:
            if radecsys is not None:
                self.radecsys = radecsys.strip()
                if self.radecsys == "FK4" or self.radecsys == "FK4-NO-E":
                    self.equinox = 1950.
                elif self.radecsys == "FK5":
                    self.equinox = 2000.
            else:
                self.radecsys = "ICRS"

        try:
            ctype1 = fits_datacontainer.all_hdu[fits_dataset].header["CTYPE1"]
            ctype2 = fits_datacontainer.all_hdu[fits_dataset].header["CTYPE2"]
            crval1 = fits_datacontainer.all_hdu[fits_dataset].header["CRVAL1"]
            crval2 = fits_datacontainer.all_hdu[fits_dataset].header["CRVAL2"]
            crpix1 = fits_datacontainer.all_hdu[fits_dataset].header["CRPIX1"]
            crpix2 = fits_datacontainer.all_hdu[fits_dataset].header["CRPIX2"]

            self.ctype = np.array([ctype1, ctype2])
            self.crval = np.array([crval1, crval2])
            self.crpix = np.array([crpix1, crpix2])

            if "CD1_1" in fits_datacontainer.all_hdu[fits_dataset].header:
                cd11 = fits_datacontainer.all_hdu[fits_dataset].header["CD1_1"]
                cd12 = fits_datacontainer.all_hdu[fits_dataset].header["CD1_2"]
                cd21 = fits_datacontainer.all_hdu[fits_dataset].header["CD2_1"]
                cd22 = fits_datacontainer.all_hdu[fits_dataset].header["CD2_2"]

                rho_a = 0.
                if cd21 > 0.:
                    rho_a = np.arctan2(cd21, cd11)
                elif cd21 < 0.:
                    rho_a = np.arctan2(-cd21, -cd11)
                rho_b = 0.
                if cd12 > 0.:
                    rho_b = np.arctan2(cd12, -cd22)
                elif cd12 < 0.:
                    rho_b = np.arctan2(-cd12, cd22)
                if np.fabs(rho_a - rho_b) < self.tiny:
                    rho = 0.5 * (rho_a + rho_b)
                    pc11 = np.cos(rho)
                    pc12 = -np.sin(rho)
                    if cd11 != 0.:
                        cdelt1 = cd11 / pc11
                    else:
                        cdelt1 = -cd21 / pc12
                    if cd22 != 0.:
                        cdelt2 = cd22 / pc11
                    else:
                        cdelt2 = cd12 / pc12
                else:
                    raise ValueError

                self.cdelt = np.array([[cdelt1, 0.], [0., cdelt2]])
                self.pc_matrix = np.array([[pc11, pc12], [-pc12, pc11]])
                self.cd_matrix = np.array([[cd11, cd12], [cd21, cd22]])
            else:
                cdelt1 = fits_datacontainer.all_hdu[fits_dataset].header["CDELT1"]
                cdelt2 = fits_datacontainer.all_hdu[fits_dataset].header["CDELT2"]

                pc11 = fits_datacontainer.all_hdu[fits_dataset].header["PC1_1"]
                pc12 = fits_datacontainer.all_hdu[fits_dataset].header["PC1_2"]
                pc21 = fits_datacontainer.all_hdu[fits_dataset].header["PC2_1"]
                pc22 = fits_datacontainer.all_hdu[fits_dataset].header["PC2_2"]

                self.cdelt = np.array([[cdelt1, 0.], [0., cdelt2]])
                self.pc_matrix = np.array([[pc11, pc12], [pc21, pc22]])
                self.cd_matrix = np.dot(self.cdelt, self.pc_matrix)

        except(KeyError, ValueError):
            self.ctype = None
            self.crval = None
            self.crpix = None
            self.cdelt = None
            self.cd_matrix = None
            self.pc_matrix = None

    def image_to_celestial(self, xpix, ypix):
        # Linear transformation
        pos = np.array([xpix - self.crpix[0], ypix -
                        self.crpix[1]]).transpose()
        xpos, ypos = np.dot(self.cd_matrix, pos)

        # Gnomonic projection
        phi = np.arctan2(xpos, -ypos)
        theta = np.arctan(1. / np.radians(np.sqrt(xpos**2. + ypos**2.)))

        alpha_p = np.radians(self.crval[0])
        delta_p = np.radians(self.crval[1])

        stheta = np.sin(theta)
        ctheta = np.cos(theta)
        sphi = np.sin(phi)
        cphi = np.cos(phi)
        sdelta_p = np.sin(delta_p)
        cdelta_p = np.cos(delta_p)

        ra_rad = alpha_p + np.arctan2(ctheta * sphi,
                                      stheta * cdelta_p +
                                      ctheta * sdelta_p * cphi)
        dec_rad = np.arcsin(stheta * sdelta_p - ctheta * cdelta_p * cphi)

        return (np.degrees(ra_rad), np.degrees(dec_rad))

    def celestial_to_image(self, ra_deg, dec_deg):
        ra_rad = np.radians(ra_deg)
        dec_rad = np.radians(dec_deg)

        alpha_p = np.radians(self.crval[0])
        delta_p = np.radians(self.crval[1])

        sra = np.sin(ra_rad - alpha_p)
        cra = np.cos(ra_rad - alpha_p)
        sdec = np.sin(dec_rad)
        cdec = np.cos(dec_rad)
        sdelta_p = np.sin(delta_p)
        cdelta_p = np.cos(delta_p)

        phi = np.arctan2(-cdec * sra,
                         sdec * cdelta_p - cdec * sdelta_p * cra) + np.pi
        theta = np.arcsin(sdec * sdelta_p + cdec * cdelta_p * cra)
        r_theta = np.rad2deg(1.) / np.tan(theta)

        xpos = r_theta * np.sin(phi)
        ypos = -r_theta * np.cos(phi)

        invdet = 1. / np.linalg.det(self.cd_matrix)

        xpix = (self.cd_matrix[1, 1] * xpos -
                self.cd_matrix[0, 1] * ypos) * invdet + self.crpix[0]
        ypix = (self.cd_matrix[0, 0] * ypos -
                self.cd_matrix[1, 0] * xpos) * invdet + self.crpix[1]

        return (xpix, ypix)


class DataPlotterManager(object):

    recipeName = "muse_exp_align"
    tagInputData = "IMAGE_FOV"
    tagFovPreview = "PREVIEW_FOV"
    tagAuxiliaryData = "SOURCE_LIST"

    _src_list_column_ids = ["Id", "X", "Y", "RA",
                            "DEC", "RA_CORR", "DEC_CORR", "Flux"]

    _btn_label_all = "All detections (uncorrected)"
    _btn_label_all_corr = "All detections (corrected)"

    _obs_id = "DATE-OBS"

    _marker_symbols = ["o", "s", "D", "p", "*", "+", "x"]
    _marker_colors = ["#0000FF", "#00FF00", "#FF0000", "#00FFFF",
                      "#FF00FF", "#FFFF00", "#FFA500", "#800080"]

    def __init__(self):
        self._plt = dict()
        self._widgets = dict()
        self._image_selector_active = 0
        self._get_parameter_value = None
        self._plt_plotter = None
        self._plt_creator = None
        self._figure = None
        self._resize_cid = None

        self.parameters = list()
        self.frameset = dict()

        super(DataPlotterManager, self).__init__()
        return

    def setWindowTitle(self):
        return self.recipeName + " GUI"

    def setWindowHelp(self):
        return "Help for " + self.setWindowTitle()

    def setInteractiveParameters(self):
        self.parameters += [
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="rsearch",
                group="Offset Calculation",
                description="Search radius (in arcsec)"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="nbins",
                group="Offset Calculation",
                description="Number of bins of the 2D histogram"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="weight",
                group="Offset Calculation",
                description="Use weighting"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="fwhm",
                group="Source Detection",
                description="FWHM of the convolution filter"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="bpixdistance", group="Source Detection",
                description="Minimum distance from image border [pixels]"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="threshold",
                group="Source Detection",
                description="Initial threshold for detecting point sources"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="bkgignore",
                group="Source Detection",
                description="Fraction of the image to be ignored"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="bkgfraction",
                group="Source Detection",
                description="Fraction of the image (without the ignored " +
                "part) considered as background"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="step",
                group="Source Detection",
                description="Increment/decrement of the threshold value"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="iterations",
                group="Source Detection",
                description="Maximum number of iterations"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="srcmin",
                group="Source Detection",
                description="Minimum number of sources"),
            reflex.RecipeParameter(
                recipe=self.recipeName,
                displayName="srcmax",
                group="Source Detection",
                description="Maximum number of sources")
        ]
        return self.parameters

    def setCurrentParameterHelper(self, parameter_value_getter):
        self._get_parameter_value = parameter_value_getter
        return

    def readFitsData(self, fitsfiles):
        self.frameset = dict()
        for fitsfile in fitsfiles:
            if fitsfile.category not in self.frameset:
                self.frameset[fitsfile.category] = []

            datasets = pipeline_product.PipelineProduct(fitsfile)

            frame_id = os.path.basename(datasets.fits_file.name)
            if fitsfile.category == self.tagInputData:
                frame_id += "\n(" + DataPlotterManager._obs_id + ": "
                frame_id += datasets.readKeyword(
                    DataPlotterManager._obs_id) + ")"

            frame = dict()
            frame["label"] = frame_id
            frame["datasets"] = datasets
            self.frameset[fitsfile.category].append(frame)

        if self.tagInputData in self.frameset and self.tagAuxiliaryData in self.frameset:
            self._plt_creator = self._data_plot_create
            self._plt_plotter = self._data_plot_draw
        else:
            self._plt_creator = self._dummy_plot_create
            self._plt_plotter = self._dummy_plot_draw
        return

    def addSubplots(self, figure):
        self._figure = figure
        self._figure.clear()
        self._plt_creator()
        return

    def plotWidgets(self):

        if "dummy" not in self._plt:
            if "buttons" not in self._widgets:
                button_labels = []
                for frame in self.frameset[self.tagInputData]:
                    button_labels.append(frame["label"])
                button_labels.append(DataPlotterManager._btn_label_all)
                button_labels.append(DataPlotterManager._btn_label_all_corr)

                self._widgets["buttons"] = \
                    reflex_widgets.InteractiveRadioButtons(
                        self._plt["buttons"],
                        self.on_image_selected,
                        button_labels,
                        self._image_selector_active,
                        title="Image Selector")
                self._resize_cid = self._figure.canvas.mpl_connect("resize_event",
                                                                   self.on_resize)

                for btn_label in self._widgets["buttons"].rbuttons.labels:
                    btn_label.set_clip_on(True)

        widgets = list()
        for key in self._widgets:
            widgets.append(self._widgets[key])

        return widgets

    def plotProductsGraphics(self):
        self._plt_plotter()
        return

    # Event handler

    def on_image_selected(self, button_label):
        selection_changed = False
        if button_label == DataPlotterManager._btn_label_all:
            if self._image_selector_active != -1:
                self._image_selector_active = -1
                selection_changed = True
        elif button_label == DataPlotterManager._btn_label_all_corr:
            if self._image_selector_active != -2:
                self._image_selector_active = -2
                selection_changed = True
        else:
            for idx, frame in enumerate(self.frameset[self.tagInputData]):
                if frame["label"] == button_label and idx != self._image_selector_active:
                    self._image_selector_active = idx
                    selection_changed = True

        # Update image view only if the selection changed
        if selection_changed:
            self._data_plot_draw()

        return

    def on_resize(self, event):
        if hasattr(self, '_widgets') and "buttons" in self._widgets:
            bbox = self._plt["buttons"].get_position().get_points()
            btns = self._widgets["buttons"].rbuttons
            figure = event.canvas.figure

            wbox = bbox[:, 0].ptp()
            hbox = bbox[:, 1].ptp()
            hfig = figure.get_figheight()
            wfig = figure.get_figwidth()
            vscale = (wbox * wfig) / (hbox * hfig)

            width = btns.labels[0].get_fontsize()
            width /= (figure.get_dpi() * wbox * wfig)

            for circle in btns.circles:
                circle.width = width
                circle.height = width * vscale
        return

    # Implementation of the plot creator and plotter delegates to be used
    # if the required data is actually available.

    def _data_plot_create(self):
        self._plt["buttons"] = self._figure.add_axes([0.05, 0.05, 0.35, 0.90])
        self._plt["imageview"] = self._figure.add_axes(
            [0.55, 0.05, 0.40, 0.90])

        # Turn off coordinate display in the status bar for the radio
        # buttons widget
        self._plt["buttons"].format_coord = lambda x, y: ""

        return

    def _data_plot_draw(self):
        if self._image_selector_active >= 0:
            img_frame = self.frameset[self.tagInputData][self._image_selector_active]
            img_data = img_frame["datasets"]
            img_data.readImage(1)

            wcs = WorldCoordinateSystem(img_data, 1)
            if wcs.ctype[0].strip() != "RA---TAN" or wcs.ctype[1].strip() != "DEC--TAN":
                raise ValueError("Unsupported world coordinate system found in '%s'" %
                                 (img_data.fits_file.name,))

            tooltip = img_frame["label"]

            # Find matching source list for the current FOV image img_data
            tbl_data = None
            num_sources = 0
            for frame in self.frameset[self.tagAuxiliaryData]:
                timestamp = frame["datasets"].readKeyword(
                    DataPlotterManager._obs_id)
                if timestamp == img_data.readKeyword(DataPlotterManager._obs_id):
                    print("Reading Source list '" + frame["label"] +
                          "' for FOV image '" + img_data.fits_file.name + "'")
                    tbl_data = read_table(frame["datasets"],
                                          self._src_list_column_ids)
                    if tbl_data is not None:
                        num_sources = len(tbl_data["Id"])

            tooltip += ": " + str(num_sources) + " sources detected"

            xlabel = "X [pixel]"
            ylabel = "Y [pixel]"

            self._plt["imageview"].clear()

            imageview = pipeline_display.ImageDisplay()
            imageview.setAspect("equal")
            imageview.setCmap("gray")
            imageview.setZAutoLimits(img_data.image, None)
            imageview.setLabels(xlabel, ylabel)
            imageview.setXLinearWCSAxis(0., 1., 1.)
            imageview.setYLinearWCSAxis(0., 1., 1.)

            imageview.display(self._plt["imageview"],
                              "Exposure FOV", tooltip, img_data.image)

            # This must be called after imageview display, to override the internal
            # formatting by imageview.display. This is a hack!

            self._plt["imageview"].format_coord = types.MethodType(format_coordinates,
                                                                   (img_data, wcs))

            self._plt["imageview"].autoscale(enable=False, tight=True)

            # If the source table is not empty, plot the detected source
            # positions and their labels on top of the displayed image
            if num_sources > 0:
                xpos = tbl_data["X"]
                ypos = tbl_data["Y"]
                marker_size = np.zeros(num_sources)
                marker_size[:] = 8

                marker, color = cycle_marker_properties(self._image_selector_active,
                                                        DataPlotterManager._marker_colors,
                                                        DataPlotterManager._marker_symbols)

                self._plt["imageview"].scatter(xpos, ypos,
                                               facecolors="none", edgecolors=color,
                                               marker=marker, s=marker_size**2)

                for idx, src_id in enumerate(tbl_data["Id"]):
                    txt_offset = 0.5 * np.sqrt(2.) * marker_size[idx]
                    xydata = (xpos[idx], ypos[idx])
                    xytext = (txt_offset, -txt_offset)
                    self._plt["imageview"].annotate(str(src_id),
                                                    xy=xydata, xycoords="data",
                                                    xytext=xytext, textcoords="offset points",
                                                    ha="left", va="top", fontsize="x-small",
                                                    color=color, clip_on=True)
        else:
            # If it exists, use the FOV preview image to setup the image
            # display and get the WCS. If the preview image is not available
            # use the first input image as fallback.
            have_fov_preview = False
            if self.tagFovPreview in self.frameset:
                img_frame = self.frameset[self.tagFovPreview][0]
                have_fov_preview = True
            else:
                img_frame = self.frameset[self.tagInputData][0]
            img_data = img_frame["datasets"]
            img_data.readImage(1)

            # Show the FOV preview as background image only if the selected
            # overlay are the corrected source positions.
            show_bkg_image = have_fov_preview and \
                self._image_selector_active in [-2]

            wcs = WorldCoordinateSystem(img_data, 1)
            if wcs.ctype[0].strip() != "RA---TAN" or wcs.ctype[1].strip() != "DEC--TAN":
                raise ValueError("Unsupported world coordinate system found in '%s'" %
                                 (img_data.fits_file.name,))

            if show_bkg_image == True:
                bkg_image = img_data.image
            else:
                # Set the pixel data of the image to 0 to clear it so that no
                # background image is displayed.
                bkg_image = np.array(img_data.image, copy=True)
                bkg_image[:] = 0

            src_lists = list()
            for img_frame in self.frameset[self.tagInputData]:
                timestamp = img_frame["datasets"].readKeyword(
                    DataPlotterManager._obs_id)
                img_name = img_frame["datasets"].fits_file.name
                for tbl_frame in self.frameset[self.tagAuxiliaryData]:
                    if timestamp == tbl_frame["datasets"].readKeyword(DataPlotterManager._obs_id):
                        print("Reading Source list '" + tbl_frame["label"] +
                              "' for FOV image '" + img_name + "'")
                        tbl_data = read_table(tbl_frame["datasets"],
                                              self._src_list_column_ids)
                        src_lists.append(tbl_data)

            xlabel = "X [pixel]"
            ylabel = "Y [pixel]"
            col_name_x = None
            col_name_y = None
            if self._image_selector_active == -1:
                col_name_x = "RA"
                col_name_y = "DEC"
                tooltip = "Uncorrected positions"
            else:
                col_name_x = "RA_CORR"
                col_name_y = "DEC_CORR"
                tooltip = "Corrected positions"

            self._plt["imageview"].clear()

            imageview = pipeline_display.ImageDisplay()
            imageview.setAspect("equal")
            imageview.setCmap("gray")
            if show_bkg_image == True:
                imageview.setZAutoLimits(bkg_image, None)
            else:
                imageview.setZLimits([0., 1.])
            imageview.setLabels(xlabel, ylabel)
            imageview.setXLinearWCSAxis(0., 1., 1.)
            imageview.setYLinearWCSAxis(0., 1., 1.)
            imageview.display(self._plt["imageview"],
                              "Source Positions", tooltip, bkg_image)

            # This must be called after imageview display, to override the internal
            # formatting by imageview.display. This is a hack!

            self._plt["imageview"].format_coord = types.MethodType(format_coordinates,
                                                                   (img_data, wcs))

            self._plt["imageview"].autoscale(enable=False)

            for idx, src_list in enumerate(src_lists):
                num_sources = len(src_list[col_name_x])
                if num_sources > 0:
                    img_coordinates = np.empty((num_sources, 2))
                    sky_coordinates = zip(
                        src_list[col_name_x], src_list[col_name_y])
                    for i, (ra_deg, dec_deg) in enumerate(sky_coordinates):
                        img_coordinates[i] = wcs.celestial_to_image(
                            ra_deg, dec_deg)
                    marker_size = np.zeros(num_sources)
                    marker_size[:] = 8
                    marker, color = cycle_marker_properties(idx,
                                                            DataPlotterManager._marker_colors,
                                                            DataPlotterManager._marker_symbols)
                    self._plt["imageview"].scatter(img_coordinates[:, 0], img_coordinates[:, 1],
                                                   facecolors="none", edgecolors=color,
                                                   marker=marker, s=marker_size**2)

        return

    # Implementation of the dummy plot creator and plotter delegates
    # follows here.

    def _dummy_plot_create(self):
        self._plt["dummy"] = self._figure.add_subplot(1, 1, 1)
        return

    def _dummy_plot_draw(self):
        label = "Data not found!\n\nInput files should contain these types:\n%s\n%s" \
            % (self.tagInputData, self.tagAuxiliaryData)

        self._plt["dummy"].set_axis_off()
        self._plt["dummy"].text(0.5, 0.5, label, color="#11557c", fontsize=18,
                                horizontalalignment="center", verticalalignment="center",
                                alpha=0.25)
        self._plt["dummy"].tooltip = "No data found"
        return


if __name__ == "__main__":

    from reflex_interactive_app import PipelineInteractiveApp

    interactive_app = PipelineInteractiveApp(enable_init_sop=True)
    interactive_app.parse_args()

    if not import_success:
        interactive_app.setEnableGUI(False)

    if interactive_app.isGUIEnabled():
        dataPlotManager = DataPlotterManager()

        interactive_app.setPlotManager(dataPlotManager)
        interactive_app.showGUI()
    else:
        interactive_app.set_continue_mode()

    # NOTE: Do not remove this line! This prints the output, which is parsed
    #       by the Reflex PythonActor to get the results!
    interactive_app.print_outputs()

    sys.exit()
