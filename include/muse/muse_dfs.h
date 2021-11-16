/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2005-2014 European Southern Observatory
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef MUSE_DFS_H
#define MUSE_DFS_H

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
/* data dictionary version to add into the product header */
#define MUSE_PRO_DID "PRO-1.16"

/* regular expression of all FITS header keywords that *
 * we want to keep from being overwritten by cpl_dfs   */
#define MUSE_HEADERS_KEEP_REGEXP "^OBJECT$|ESO DRS MUSE"

/* Define here the DO.CATG keywords */
#define MUSE_TAG_EMPTY ""
#define MUSE_TAG_BIAS  "BIAS"
#define MUSE_TAG_DARK  "DARK"
#define MUSE_TAG_FLAT  "FLAT"
#define MUSE_TAG_ARC   "ARC"
#define MUSE_TAG_WAVE  "WAVE"
#define MUSE_TAG_MASK  "MASK"
#define MUSE_TAG_ILLUM "ILLUM"
#define MUSE_TAG_NONLINGAIN  "NONLINEARITY_GAIN"
#define MUSE_TAG_MASTER_BIAS "MASTER_BIAS"
#define MUSE_TAG_MASTER_DARK "MASTER_DARK"
#define MUSE_TAG_DARK_MODEL  "MODEL_DARK"
#define MUSE_TAG_MASTER_FLAT "MASTER_FLAT"
#define MUSE_TAG_TRACE_TABLE   "TRACE_TABLE"
#define MUSE_TAG_TRACE_SAMPLES "TRACE_SAMPLES"
#define MUSE_TAG_WAVECAL_TABLE "WAVECAL_TABLE"
#define MUSE_TAG_WAVECAL_DEBUG "WAVECAL_RESIDUALS"
#define MUSE_TAG_LINE_CATALOG  "LINE_CATALOG"
#define MUSE_TAG_BADPIX_TABLE  "BADPIX_TABLE"
#define MUSE_TAG_ARC_RED_LAMP  "ARC_RED_LAMP"
#define MUSE_TAG_ARC_RED       "ARC_RED"
#define MUSE_TAG_ARC_RESAMP    "ARC_RESAMPLED"
#define MUSE_TAG_WAVE_MAP      "WAVE_MAP"
#define MUSE_TAG_LSF_PROFILE   "LSF_PROFILE"
#define MUSE_TAG_PT_SUB        "PIXTABLE_SUBTRACTED"
#define MUSE_TAG_GEOMETRY_OLD   "GEOMETRY_UNSMOOTHED"
#define MUSE_TAG_GEOMETRY_TABLE "GEOMETRY_TABLE"
#define MUSE_TAG_MASK_REDUCED   "MASK_REDUCED"
#define MUSE_TAG_MASK_COMBINED  "MASK_COMBINED"
#define MUSE_TAG_SPOTS_TABLE    "SPOTS_TABLE"
#define MUSE_TAG_VIGN_MASK      "VIGNETTING_MASK"
#define MUSE_TAG_CUBE_SKYFLAT   "DATACUBE_SKYFLAT"
#define MUSE_TAG_TWILIGHT_CUBE  "TWILIGHT_CUBE"
#define MUSE_TAG_EXTINCT_TABLE  "EXTINCT_TABLE"
#define MUSE_TAG_STD_FLUX_TABLE "STD_FLUX_TABLE"
#define MUSE_TAG_TELLREG        "TELLURIC_REGIONS"
#define MUSE_TAG_STD_RESPONSE   "STD_RESPONSE"
#define MUSE_TAG_STD_TELLURIC   "STD_TELLURIC"
#define MUSE_TAG_CUBE_STD       "DATACUBE_STD"
#define MUSE_TAG_STD_INTIMAGE   "STD_FLUXES"
#define MUSE_TAG_ASTROMETRY_REFERENCE "ASTROMETRY_REFERENCE"
#define MUSE_TAG_ASTROMETRY_WCS       "ASTROMETRY_WCS"
#define MUSE_TAG_CUBE_ASTROMETRY      "DATACUBE_ASTROMETRY"
#define MUSE_TAG_ACAL_MASK      "AUTOCAL_MASK"
#define MUSE_TAG_ACAL_FACTORS   "AUTOCAL_FACTORS"
#define MUSE_TAG_RAMAN_IMAGES   "RAMAN_IMAGES"
#define MUSE_TAG_SKY_SPECTRUM   "SKY_SPECTRUM"
#define MUSE_TAG_SKY_IMAGE      "SKY_IMAGE"
#define MUSE_TAG_SKY_MASK       "SKY_MASK"
#define MUSE_TAG_SKY_CONT       "SKY_CONTINUUM"
#define MUSE_TAG_SKY_LINES      "SKY_LINES"
#define MUSE_TAG_RAMAN_LINES    "RAMAN_LINES"
#define MUSE_TAG_FILTER_LIST    "FILTER_LIST"
#define MUSE_TAG_OUTPUT_WCS     "OUTPUT_WCS"
#define MUSE_TAG_IMAGE_FOV        "IMAGE_FOV"
#define MUSE_TAG_CUBE_FINAL       "DATACUBE_FINAL"
#define MUSE_TAG_PIXTABLE_COMBINED "PIXTABLE_COMBINED"
#define MUSE_TAG_PIXTABLE_POSITIONED "PIXTABLE_POSITIONED"
#define MUSE_TAG_PIXTABLE_REDUCED "PIXTABLE_REDUCED"
#define MUSE_TAG_OBJECT_RESAMPLED "OBJECT_RESAMPLED"
#define MUSE_TAG_OFFSET_LIST      "OFFSET_LIST"
#define MUSE_TAG_SOURCE_LIST      "SOURCE_LIST"
/* tags used by the muse_qi_mask recipe */
#define MUSE_TAG_MASK_IMAGE  "MASK_IMAGE"
#define MUSE_TAG_LINEARITY_BIAS "LINGAIN_LAMP_OFF"
#define MUSE_TAG_LINEARITY_FLAT "LINGAIN_LAMP_ON"

/* Reasons to exclude frames from a framelist */
#define MUSE_EXCLUDE_OK           0 /* frame not excluded */
#define MUSE_EXCLUDE_WRONG_IFU    1 /* frame is from different IFU */
#define MUSE_EXCLUDE_BINNING      2 /* frame has different binning */
#define MUSE_EXCLUDE_NOT_RAW      3 /* frame is not a raw input frame */
#define MUSE_EXCLUDE_NO_HEADER    4 /* frame header cannot be loaded */
#define MUSE_EXCLUDE_WARN_NO_TAG  5 /* frame does not have a tag at all, outputting a warning recommended */
#define MUSE_EXCLUDE_WRONG_TAG    6 /* frame does not have the tag that the recipe needs */

/* Table column names of tracing table                 *
 * the single digit number gives the polynomial number *
 * (see muse_tracing.h)                                */
#define MUSE_TRACE_TABLE_COL_SLICE_NO "SliceNo" /* number of slice */
#define MUSE_TRACE_TABLE_COL_WIDTH    "Width"   /* mean width of slice in pix */
#define MUSE_TRACE_TABLE_COL_COEFF    "tc%1d_%02d" /* polynomial coefficient */
#define MUSE_TRACE_TABLE_COL_MSE      "MSE%1d"  /* mean squared error of fit */

/* FITS headers of a LINE_CATALOG */
#define MUSE_HDR_LINE_CATALOG_VERSION "VERSION"
/* Table column names of wavelength calibration table (WAVECAL_TABLE) */
#define MUSE_WAVECAL_TABLE_COL_SLICE_NO   "SliceNo"     /* number of slice */
#define MUSE_WAVECAL_TABLE_COL_COEFF      "wlc%1hu%1hu" /* polynomial coefficient */
#define MUSE_WAVECAL_TABLE_COL_MSE        "MSE"         /* mean squared error of fit */
/* Table column names of a GEOMETRY_TABLE */
#define MUSE_GEOTABLE_FIELD  "SubField"
#define MUSE_GEOTABLE_CCD    "SliceCCD"
#define MUSE_GEOTABLE_SKY    "SliceSky"
#define MUSE_GEOTABLE_X      "x"
#define MUSE_GEOTABLE_Y      "y"
#define MUSE_GEOTABLE_ANGLE  "angle"
#define MUSE_GEOTABLE_WIDTH  "width"

/* temporary properties to be propagated within the MUSE pipeline */
/* a regular expression for easy erasing */
#define MUSE_HDR_TMP_REGEXP "MUSE TMP( |[0-9]+ )(INTAG$|FILE$|NSATURATED$|QUAD)"
/* input filename to be used for diagnostic messages; *
 * should be temporary and never saved to disk!       */
#define MUSE_HDR_TMP_INTAG "MUSE TMP INTAG"
#define MUSE_HDR_TMP_FN "MUSE TMP FILE"
#define MUSE_HDR_TMP_NSAT "MUSE TMP NSATURATED" /* number of saturated pixels */
#define MUSE_HDR_TMPi_NSAT "MUSE TMP%d NSATURATED"
#define MUSE_HDR_TMP_QUADnMED "MUSE TMP QUAD%hhu MEDIAN" /* per-quadrant median */

/* FITS keywords to store overscan statistics */
#define MUSE_HDR_OVSC_REGEXP   "^ESO DRS MUSE OVSC"
#define MUSE_HDR_OVSC_MEAN     "ESO DRS MUSE OVSC%1hhu MEAN"
#define MUSE_HDR_OVSC_STDEV    "ESO DRS MUSE OVSC%1hhu STDEV"
#define MUSE_HDR_OVSC_PNC      "ESO DRS MUSE OVSC%1hhu POLY NCOEFF"
#define MUSE_HDR_OVSC_PY       "ESO DRS MUSE OVSC%1hhu POLY Y%hhu"

/* FITS keywords to exchange the nonlinearity polynomials */
#define MUSE_HDR_NONLINn_ORDER  "ESO DRS MUSE NONLIN%1hhu POLY ORDER"
#define MUSE_HDR_NONLINn_COEFFo "ESO DRS MUSE NONLIN%1hhu POLY COEFF%hhu"
#define MUSE_HDR_NONLINn_LLO    "ESO DRS MUSE NONLIN%1hhu POLY LOLIMIT"
#define MUSE_HDR_NONLINn_LHI    "ESO DRS MUSE NONLIN%1hhu POLY HILIMIT"

/* FITS header keywords for the flat-field levels */
#define MUSE_HDR_FLAT_FLUX_LAMP "ESO DRS MUSE FLAT FLUX LAMP"
#define MUSE_HDR_FLAT_FLUX_SKY  "ESO DRS MUSE FLAT FLUX SKY"

/* FITS header keywords to store the applied RA/DEC offsets */
#define MUSE_HDR_OFFSETi_DATEOBS   "ESO DRS MUSE OFFSET%u DATE-OBS"
#define MUSE_HDR_OFFSETi_DATEOBS_C "offset %u applied to exposure with this DATE-OBS"
#define MUSE_HDR_OFFSETi_DRA       "ESO DRS MUSE OFFSET%u DRA"
#define MUSE_HDR_OFFSETi_DRA_C     "[deg] (= %f arcsec) RA offset applied"
#define MUSE_HDR_OFFSETi_DDEC      "ESO DRS MUSE OFFSET%u DDEC"
#define MUSE_HDR_OFFSETi_DDEC_C    "[deg] (= %f arcsec) DEC offset applied"
#define MUSE_HDR_FLUX_SCALEi       "ESO DRS MUSE FLUX SCALE%u"
#define MUSE_HDR_FLUX_SCALEi_C     "flux scale factor applied"

/* FITS header keyword to list the filter name and *
 * properties in the collapsed images (IMAGE_FOV)  */
#define MUSE_HDR_FILTER          "ESO DRS MUSE FILTER NAME"
#define MUSE_HDR_FILTER_C        "filter name used"
#define MUSE_HDR_FILTER_ZPVEGA   "ESO DRS MUSE FILTER ZPVEGA"
#define MUSE_HDR_FILTER_ZPVEGA_C "[mag] Vega-mag zeropoint"
#define MUSE_HDR_FILTER_ZPAB     "ESO DRS MUSE FILTER ZPAB"
#define MUSE_HDR_FILTER_ZPAB_C   "[mag] AB-mag zeropoint"
#define MUSE_HDR_FILTER_FFRAC    "ESO DRS MUSE FILTER FRACTION"
#define MUSE_HDR_FILTER_FFRAC_C  "[%] fraction of filter covered by data"
#define MUSE_HDR_FILTER_REGEXP   "^ESO DRS MUSE FILTER "

/* informational keywords used when computing the WCS solution */
#define MUSE_HDR_WCS_DETSIGMA       "ESO DRS MUSE WCS DETSIGMA"
#define MUSE_HDR_WCS_DETSIGMA_C     "Final detsigma level used"
#define MUSE_HDR_WCS_DETSIGMA_C_ONE "detsigma used"
#define MUSE_HDR_WCS_RADIUS         "ESO DRS MUSE WCS RADIUS"
#define MUSE_HDR_WCS_RADIUS_C       "[pix] final ppm radius used"
#define MUSE_HDR_WCS_ACCURACY       "ESO DRS MUSE WCS ACCURACY"
#define MUSE_HDR_WCS_ACCURACY_C     "[pix] final ppm accuracy used"
#define MUSE_HDR_WCS_FACCURACY      "ESO DRS MUSE WCS FACCURACY"
#define MUSE_HDR_WCS_FACCURACY_C    "final fractional accuracy used"

/* Bias QC headers */
#define QC_BIAS_PREFIXi         "ESO QC BIAS INPUT%d"
#define QC_BIAS_MASTER_PREFIX   "ESO QC BIAS MASTER"
#define QC_BIAS_MASTERn_PREFIX  QC_BIAS_MASTER_PREFIX"%1hhu"
#define QC_BIAS_MASTER_RON      QC_BIAS_MASTERn_PREFIX" RON"
#define QC_BIAS_MASTER_RONERR   QC_BIAS_MASTERn_PREFIX" RONERR"
#define QC_BIAS_MASTER_SLOPEX   QC_BIAS_MASTERn_PREFIX" SLOPE X"
#define QC_BIAS_MASTER_SLOPEY   QC_BIAS_MASTERn_PREFIX" SLOPE Y"
#define QC_BIAS_MASTER_NBADPIX  QC_BIAS_MASTER_PREFIX" NBADPIX"
#define QC_BIAS_LEVELi          "ESO QC BIAS LEVEL%hhu"
#define QC_BIAS_LEVELi_MEAN      QC_BIAS_LEVELi" MEAN"
#define QC_BIAS_LEVELi_STDEV     QC_BIAS_LEVELi" STDEV"
#define QC_BIAS_LEVELi_MEDIAN    QC_BIAS_LEVELi" MEDIAN"

/* Dark QC headers */
#define QC_DARK_PREFIXi          "ESO QC DARK INPUT%d"
#define QC_DARK_MASTER_PREFIX    "ESO QC DARK MASTER"
#define QC_DARK_MASTER_NBADPIX   QC_DARK_MASTER_PREFIX" NBADPIX"
#define QC_DARK_MASTER_DARKVALUE QC_DARK_MASTER_PREFIX" DC"
#define QC_DARK_MASTER_DARKERROR QC_DARK_MASTER_PREFIX" DCERR"

/* Flat QC headers */
#define QC_FLAT_PREFIXi             "ESO QC FLAT INPUT%d"
#define QC_FLAT_MASTER_PREFIX       "ESO QC FLAT MASTER"
#define QC_FLAT_MASTER_INTFLUX      QC_FLAT_MASTER_PREFIX" INTFLUX"
#define QC_FLAT_MASTER_THRUPUT      QC_FLAT_MASTER_PREFIX" THRUPUT"
#define QC_FLAT_MASTER_SLICEj_MEAN  QC_FLAT_MASTER_PREFIX" SLICE%d MEAN"
#define QC_FLAT_MASTER_SLICEj_STDEV QC_FLAT_MASTER_PREFIX" SLICE%d STDEV"
/* Tracing QC headers (also produced by flat recipe) */
#define QC_TRACE_PREFIX        "ESO QC TRACE"
#define QC_TRACE_L_XPOS        QC_TRACE_PREFIX" SLICE_L XPOS"
#define QC_TRACE_L_TILT        QC_TRACE_PREFIX" SLICE_L TILT"
#define QC_TRACE_R_XPOS        QC_TRACE_PREFIX" SLICE_R XPOS"
#define QC_TRACE_R_TILT        QC_TRACE_PREFIX" SLICE_R TILT"
#define QC_TRACE_SLICEj_MXSLOP QC_TRACE_PREFIX" SLICE%d MAXSLOPE"
#define QC_TRACE_SLICEj_WIDTH  QC_TRACE_PREFIX" SLICE%d WIDTH"
#define QC_TRACE_WIDTHS_MEDIAN QC_TRACE_PREFIX" WIDTHS MEDIAN"
#define QC_TRACE_WIDTHS_MEAN   QC_TRACE_PREFIX" WIDTHS MEAN"
#define QC_TRACE_WIDTHS_STDEV  QC_TRACE_PREFIX" WIDTHS STDEV"
#define QC_TRACE_WIDTHS_MIN    QC_TRACE_PREFIX" WIDTHS MIN"
#define QC_TRACE_WIDTHS_MAX    QC_TRACE_PREFIX" WIDTHS MAX"
#define QC_TRACE_WIDTHS_MAX    QC_TRACE_PREFIX" WIDTHS MAX"
#define QC_TRACE_GAPS_MEDIAN   QC_TRACE_PREFIX" GAPS MEDIAN"
#define QC_TRACE_GAPS_MEAN     QC_TRACE_PREFIX" GAPS MEAN"
#define QC_TRACE_GAPS_STDEV    QC_TRACE_PREFIX" GAPS STDEV"
#define QC_TRACE_GAPS_MIN      QC_TRACE_PREFIX" GAPS MIN"
#define QC_TRACE_GAPS_MAX      QC_TRACE_PREFIX" GAPS MAX"

/* Arc QC headers */
#define QC_WAVECAL_PREFIX                  "ESO QC WAVECAL"
#define QC_WAVECAL_PREFIXi                 QC_WAVECAL_PREFIX" INPUT%u"
#define QC_WAVECAL_PREFIXli                QC_WAVECAL_PREFIX" LAMP%u INPUT%u"
#define QC_WAVECAL_SLICE_PREFIX            QC_WAVECAL_PREFIX" SLICE"
#define QC_WAVECAL_SLICEj_PREFIX           QC_WAVECAL_SLICE_PREFIX"%hu"
#define QC_WAVECAL_SLICEj_LINES_NDET       QC_WAVECAL_SLICEj_PREFIX" LINES NDET"
#define QC_WAVECAL_SLICEj_LINES_NID        QC_WAVECAL_SLICEj_PREFIX" LINES NID"
#define QC_WAVECAL_SLICEj_LINES_PEAK_MEAN  QC_WAVECAL_SLICEj_PREFIX" LINES PEAK MEAN"
#define QC_WAVECAL_SLICEj_LINES_PEAK_STDEV QC_WAVECAL_SLICEj_PREFIX" LINES PEAK STDEV"
#define QC_WAVECAL_SLICEj_LINES_PEAK_MIN   QC_WAVECAL_SLICEj_PREFIX" LINES PEAK MIN"
#define QC_WAVECAL_SLICEj_LINES_PEAK_MAX   QC_WAVECAL_SLICEj_PREFIX" LINES PEAK MAX"
#define QC_WAVECAL_SLICEj_LAMPl_LINES_PEAK_MEAN  \
        QC_WAVECAL_SLICEj_PREFIX" LAMP%d LINES PEAK MEAN"
#define QC_WAVECAL_SLICEj_LAMPl_LINES_PEAK_STDEV \
        QC_WAVECAL_SLICEj_PREFIX" LAMP%d LINES PEAK STDEV"
#define QC_WAVECAL_SLICEj_LAMPl_LINES_PEAK_MAX   \
        QC_WAVECAL_SLICEj_PREFIX" LAMP%d LINES PEAK MAX"
#define QC_WAVECAL_SLICEj_LINES_FWHM_MEAN  QC_WAVECAL_SLICEj_PREFIX" LINES FWHM MEAN"
#define QC_WAVECAL_SLICEj_LINES_FWHM_STDEV QC_WAVECAL_SLICEj_PREFIX" LINES FWHM STDEV"
#define QC_WAVECAL_SLICEj_LINES_FWHM_MIN   QC_WAVECAL_SLICEj_PREFIX" LINES FWHM MIN"
#define QC_WAVECAL_SLICEj_LINES_FWHM_MAX   QC_WAVECAL_SLICEj_PREFIX" LINES FWHM MAX"
#define QC_WAVECAL_SLICEj_RESOL            QC_WAVECAL_SLICEj_PREFIX" RESOL"
#define QC_WAVECAL_SLICEj_FIT_NLINES       QC_WAVECAL_SLICEj_PREFIX" FIT NLINES"
#define QC_WAVECAL_SLICEj_FIT_RMS          QC_WAVECAL_SLICEj_PREFIX" FIT RMS"
#define QC_WAVECAL_SLICEj_DWLEN_BOT        QC_WAVECAL_SLICEj_PREFIX" DWLEN BOTTOM"
#define QC_WAVECAL_SLICEj_DWLEN_TOP        QC_WAVECAL_SLICEj_PREFIX" DWLEN TOP"
#define QC_WAVECAL_SLICEj_WLPOS            QC_WAVECAL_SLICEj_PREFIX" WLPOS"
#define QC_WAVECAL_SLICEj_WLEN             QC_WAVECAL_SLICEj_PREFIX" WLEN"

/* Geometry QC headers */
#define QC_GEO_PREFIX       "ESO QC GEO "
#define QC_GEO_EXPk_MEAN    QC_GEO_PREFIX"EXP%u FWHM MEAN"
#define QC_GEO_EXPk_MEDIAN  QC_GEO_PREFIX"EXP%u FWHM MEDIAN"
#define QC_GEO_EXPk_STDEV   QC_GEO_PREFIX"EXP%u FWHM STDEV"
#define QC_GEO_FWHM_MEAN    QC_GEO_PREFIX"FWHM MEAN"
#define QC_GEO_FWHM_STDEV   QC_GEO_PREFIX"FWHM STDEV"
#define QC_GEO_IFUi_ANGLE   QC_GEO_PREFIX"IFU%hhu ANGLE"
#define QC_GEO_IFUi_WLENj   QC_GEO_PREFIX"IFU%hhu WLEN%d"
#define QC_GEO_IFUi_MEANj   QC_GEO_PREFIX"IFU%hhu WLEN%d FLUX MEAN"
#define QC_GEO_IFUi_MEDIANj QC_GEO_PREFIX"IFU%hhu WLEN%d FLUX MEDIAN"
#define QC_GEO_IFUi_STDEVj  QC_GEO_PREFIX"IFU%hhu WLEN%d FLUX STDEV"
#define QC_GEO_IFUi_GAP     QC_GEO_PREFIX"IFU%hhu GAPPOS MEAN"
#define QC_GEO_GAPS_MEAN    QC_GEO_PREFIX"GAPPOS MEAN"
#define QC_GEO_GAPS_STDEV   QC_GEO_PREFIX"GAPPOS STDEV"
#define QC_GEO_MASK_ANGLE   QC_GEO_PREFIX"MASK ANGLE"
#define QC_GEO_SMOOTH_NX    QC_GEO_PREFIX"SMOOTH NX"
#define QC_GEO_SMOOTH_NY    QC_GEO_PREFIX"SMOOTH NY"
#define QC_GEO_SMOOTH_NA    QC_GEO_PREFIX"SMOOTH NANGLE"
#define QC_GEO_SMOOTH_NW    QC_GEO_PREFIX"SMOOTH NWIDTH"
#define QC_GEO_TABLE_NBAD   QC_GEO_PREFIX"TABLE NBAD"

/* Twilight QC headers */
#define QC_TWILIGHTm_PREFIXi       "ESO QC TWILIGHT%hhu INPUT%u"
#define QC_TWILIGHTm_MASTER_PREFIX "ESO QC TWILIGHT%hhu MASTER"
#define QC_TWILIGHTm_INTFLUX       "ESO QC TWILIGHT%hhu INTFLUX"
#define QC_TWILIGHT_REGEXP         "^ESO QC TWILIGHT[0-9]"

/* SciBasic QC headers */
#define QC_SCIBASIC_PREFIX   "ESO QC SCIBASIC "
#define QC_SCIBASIC_SHIFT    QC_SCIBASIC_PREFIX"LAMBDA SHIFT"

/* general basic reduction QC headers */
#define QC_BASIC_NSATURATED  "NSATURATED" /* postfix! */

/* SciPost QC headers */
/* header prefixes for the FWHMi.X and FWHMi.Y keywords, depending on recipe */
#define QC_POST_PREFIX_SCIPOST    "ESO QC SCIPOST"
#define QC_POST_PREFIX_EXPCOMBINE "ESO QC EXPCOMB"
#define QC_POST_PREFIX_STANDARD   "ESO QC STANDARD"
#define QC_POST_PREFIX_ASTROMETRY "ESO QC ASTRO"
/* the Raman removal keywords, only in scipost */
#define QC_POST_RAMAN_SP_XX       QC_POST_PREFIX_SCIPOST" RAMAN SPATIAL XX"
#define QC_POST_RAMAN_SP_XY       QC_POST_PREFIX_SCIPOST" RAMAN SPATIAL XY"
#define QC_POST_RAMAN_SP_YY       QC_POST_PREFIX_SCIPOST" RAMAN SPATIAL YY"
#define QC_POST_RAMAN_SP_X        QC_POST_PREFIX_SCIPOST" RAMAN SPATIAL X"
#define QC_POST_RAMAN_SP_Y        QC_POST_PREFIX_SCIPOST" RAMAN SPATIAL Y"
#define QC_POST_RAMAN_FLUXO2      QC_POST_PREFIX_SCIPOST" RAMAN FLUX O2"
#define QC_POST_RAMAN_FLUXN2      QC_POST_PREFIX_SCIPOST" RAMAN FLUX N2"
/* end of the FWHMi.X and FWHMi.Y keywords, depending on recipe */
#define QC_POST_NDET              "%s NDET"
#define QC_POST_LAMBDA            "%s LAMBDA"
#define QC_POST_POSX              "%s POS%d X"
#define QC_POST_POSY              "%s POS%d Y"
#define QC_POST_FWHM_NVALID       "%s FWHM NVALID"
#define QC_POST_FWHMX             "%s FWHM%d X"
#define QC_POST_FWHMY             "%s FWHM%d Y"
#define QC_POST_FWHM_MEDIAN       "%s FWHM MEDIAN"
#define QC_POST_FWHM_MAD          "%s FWHM MAD"

/* Exposure alignment QC headers */
#define QC_ALIGN_BKG_MEDIAN       "ESO QC EXPALIGN BKG MEDIAN"
#define QC_ALIGN_BKG_MAD          "ESO QC EXPALIGN BKG MAD"
#define QC_ALIGN_THRESHOLD        "ESO QC EXPALIGN THRESHOLD"
#define QC_ALIGN_SRC_POSITIONS    "ESO QC EXPALIGN SRC POSITIONS"
#define QC_ALIGN_NDET             "ESO QC EXPALIGN NDET"
#define QC_ALIGN_NDETi            "ESO QC EXPALIGN NDET%d"
#define QC_ALIGN_NMATCHi          "ESO QC EXPALIGN NMATCH%d"
#define QC_ALIGN_NMATCH_MIN       "ESO QC EXPALIGN NMATCH MIN"
#define QC_ALIGN_NOMATCH          "ESO QC EXPALIGN NOMATCH"
#define QC_ALIGN_DRA_MIN           "ESO QC EXPALIGN OFFSET RA MIN"
#define QC_ALIGN_DRA_MAX           "ESO QC EXPALIGN OFFSET RA MAX"
#define QC_ALIGN_DRA_MEAN          "ESO QC EXPALIGN OFFSET RA MEAN"
#define QC_ALIGN_DRA_STDEV         "ESO QC EXPALIGN OFFSET RA STDEV"
#define QC_ALIGN_DDEC_MIN          "ESO QC EXPALIGN OFFSET DEC MIN"
#define QC_ALIGN_DDEC_MAX          "ESO QC EXPALIGN OFFSET DEC MAX"
#define QC_ALIGN_DDEC_MEAN         "ESO QC EXPALIGN OFFSET DEC MEAN"
#define QC_ALIGN_DDEC_STDEV        "ESO QC EXPALIGN OFFSET DEC STDEV"

/* Standard star throughput estimates */
#define QC_STD_NAME          "ESO QC STANDARD STARNAME"
#define QC_STD_THRU          "ESO QC STANDARD THRU%04.0f"
#define QC_STD_ZP            "ESO QC STANDARD ZP %s"

/* Astrometric solution QC headers */
#define QC_ASTROMETRY_NSTARS "ESO QC ASTRO NSTARS"
#define QC_ASTROMETRY_SCX    "ESO QC ASTRO SCALE X"
#define QC_ASTROMETRY_SCY    "ESO QC ASTRO SCALE Y"
#define QC_ASTROMETRY_ANGX   "ESO QC ASTRO ANGLE X"
#define QC_ASTROMETRY_ANGY   "ESO QC ASTRO ANGLE Y"
#define QC_ASTROMETRY_RESX   "ESO QC ASTRO MEDRES X"
#define QC_ASTROMETRY_RESY   "ESO QC ASTRO MEDRES Y"

/* Non-linearity gain QC headers */
#define QC_LINGAIN_PREFIX      "ESO QC LINGAIN "
#define QC_LINGAIN_RONi        QC_LINGAIN_PREFIX "OUT%u RON"
#define QC_LINGAIN_RONi_ERR    QC_LINGAIN_PREFIX "OUT%u RONERR"
#define QC_LINGAIN_RONi_MEDIAN QC_LINGAIN_PREFIX "OUT%u RON MEDIAN"
#define QC_LINGAIN_RONi_MAD    QC_LINGAIN_PREFIX "OUT%u RON MAD"
#define QC_LINGAIN_CONADi      QC_LINGAIN_PREFIX "OUT%u CONAD"
#define QC_LINGAIN_GAINi       QC_LINGAIN_PREFIX "OUT%u GAIN"
#define QC_LINGAIN_GAINi_ERR   QC_LINGAIN_PREFIX "OUT%u GAINERR"
#define QC_LINGAIN_COUNTSi_MIN QC_LINGAIN_PREFIX "OUT%u COUNTS MIN"
#define QC_LINGAIN_COUNTSi_MAX QC_LINGAIN_PREFIX "OUT%u COUNTS MAX"
#define QC_LINGAIN_NLFITi_RMS QC_LINGAIN_PREFIX "NLFIT%u RMS"

#endif /* MUSE_DFS_H */
