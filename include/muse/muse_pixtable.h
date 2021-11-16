/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2005-2018 European Southern Observatory
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

#ifndef MUSE_PIXTABLE_H
#define MUSE_PIXTABLE_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>
#include <stdint.h>

#include "muse_dfs.h"
#include "muse_image.h"
#include "muse_imagelist.h"
#include "muse_mask.h"
#include "muse_cplwrappers.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
#if !HAVE_UINT32_T /* is common but C99 only requires uint_least32_t... */
typedef unsigned int uint32_t;
#endif

/** @addtogroup muse_pixtable */
/**@{*/

/* Table columns of a MUSE pixel table. See muse_pixtable_def for comments. */
#define MUSE_PIXTABLE_DATA    "data" /**< data column of a MUSE pixel table */
#define MUSE_PIXTABLE_DQ      "dq" /**< data quality column of a MUSE pixel table */
#define MUSE_PIXTABLE_STAT    "stat" /**< variance column of a MUSE pixel table */
#define MUSE_PIXTABLE_XPOS    "xpos" /**< x-position column of a MUSE pixel table */
#define MUSE_PIXTABLE_YPOS    "ypos" /**< y-position column of a MUSE pixel table */
#define MUSE_PIXTABLE_LAMBDA  "lambda" /**< wavelength column of a MUSE pixel table */
#define MUSE_PIXTABLE_ORIGIN  "origin" /**< origin column of a MUSE pixel table */

/* relative weight of a pixel (used for combining exposures) */
#define MUSE_PIXTABLE_WEIGHT  "weight" /**< weight column of a MUSE pixel table (optional) */

/* Table columns of the flat-field spectrum of a MUSE pixel table. */
#define MUSE_PIXTABLE_FF_EXT   "PIXTABLE_FLAT_FIELD" /**< flat-field extension of a MUSE pixel table (optional) */
#define MUSE_PIXTABLE_FFLAMBDA MUSE_PIXTABLE_LAMBDA /**< wavelength column for the flat-field extension of a MUSE pixel table */
#define MUSE_PIXTABLE_FFDATA   MUSE_PIXTABLE_DATA/**< wavelength column for the flat-field extension of a MUSE pixel table */

extern const muse_cpltable_def muse_pixtable_def[];

/**
  @brief   FITS header keyword for the horizontal slice offset on the CCD.

  This FITS header entry is needed to transform the "origin" column back to CCD
  pixels. The "EXP" part is originally created with EXP0, signifying a single
  exposure. When multiple exposures are merged into a single pixel table, the
  EXP number is updated to reflect the exposure number for which this entry is
  to be used. See @ref MUSE_HDR_PT_EXP_FST, @ref MUSE_HDR_PT_EXP_LST.
 */
#define MUSE_HDR_PT_IFU_SLICE_OFFSET "ESO DRS MUSE PIXTABLE EXP%u IFU%02hu SLICE%02hu XOFFSET"
#define MUSE_HDR_PT_IFU_SLICE_OFFSET_COMMENT "x-offset of given slice in given IFU of given exposure"

/**
  @brief Pixel table "type" stored in the FITS header.

  Can be either "GEOFULL" (for science use and 3D resampling) and "SIMPLE" for
  quick-look, tests, and calibration (2D resampling).
 */
#define MUSE_HDR_PT_TYPE             "ESO DRS MUSE PIXTABLE TYPE"
#define MUSE_PIXTABLE_STRING_SIMPLE  "SIMPLE"
#define MUSE_PIXTABLE_STRING_FULL    "GEOFULL"
#define MUSE_PIXTABLE_COMMENT_SIMPLE "pixel table without y coordinates"
#define MUSE_PIXTABLE_COMMENT_FULL   "pixel table with full geometry information"

/**
  @brief FITS header keyword defining the first row index for a given exposure.
  @note This keyword needs to be updated when rows are removed from the pixel table!
 */
#define MUSE_HDR_PT_EXP_FST "ESO DRS MUSE PIXTABLE EXP%u FIRST"
/**
  @brief FITS header keyword defining the last row index for a given exposure.
  @note This keyword needs to be updated when rows are removed from the pixel table!
 */
#define MUSE_HDR_PT_EXP_LST "ESO DRS MUSE PIXTABLE EXP%u LAST"
#define MUSE_HDR_PT_EXP_FST_COMMENT "Exposure %u first row index"
#define MUSE_HDR_PT_EXP_LST_COMMENT "Exposure %u last row index"
#define MUSE_HDR_PT_EXP_REGEXP "ESO DRS MUSE PIXTABLE EXP[0-9]+ (FIRST|LAST)"

/** illumination-correction (multiplication) factors for each slice */
#define MUSE_HDR_PT_ILLUMi      "ESO DRS MUSE PIXTABLE ILLUM%hu"
/** illumination-correction statistics: mean */
#define MUSE_HDR_PT_ILLUM_MEAN  "ESO DRS MUSE PIXTABLE ILLUM MEAN"
/** illumination-correction statistics: stdev */
#define MUSE_HDR_PT_ILLUM_STDEV "ESO DRS MUSE PIXTABLE ILLUM STDEV"
/** regular expression to remove the illum values and statistics from a header */
#define MUSE_HDR_PT_ILLUM_REGEXP "^ESO DRS MUSE PIXTABLE ILLUM"

/* properties of the pixel table stored in the FITS header */
/**
  @brief FITS header keyword contains the lower limit of the data in x-direction.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_XLO "ESO DRS MUSE PIXTABLE LIMITS X LOW"
/**
  @brief FITS header keyword contains the upper limit of the data in x-ion.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_XHI "ESO DRS MUSE PIXTABLE LIMITS X HIGH"
/**
  @brief FITS header keyword contains the lower limit of the data in y-direction.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_YLO "ESO DRS MUSE PIXTABLE LIMITS Y LOW"
/**
  @brief FITS header keyword contains the upper limit of the data in y-direction.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_YHI "ESO DRS MUSE PIXTABLE LIMITS Y HIGH"
/**
  @brief FITS header keyword contains the lower limit of the data in spectral direction.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_LLO "ESO DRS MUSE PIXTABLE LIMITS LAMBDA LOW"
/**
  @brief FITS header keyword contains the upper limit of the data in spectral direction.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_LHI "ESO DRS MUSE PIXTABLE LIMITS LAMBDA HIGH"
/**
  @brief FITS header keyword contains the lowest IFU number in the data.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_ILO "ESO DRS MUSE PIXTABLE LIMITS IFU LOW"
/**
  @brief FITS header keyword contains the highest IFU number in the data.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_IHI "ESO DRS MUSE PIXTABLE LIMITS IFU HIGH"
/**
  @brief FITS header keyword contains the lowest slice number in the data.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_SLO "ESO DRS MUSE PIXTABLE LIMITS SLICE LOW"
/**
  @brief FITS header keyword contains the highest slice number in the data.
  @note This keyword needs to be updated whenever the data changes!
 */
#define MUSE_HDR_PT_SHI "ESO DRS MUSE PIXTABLE LIMITS SLICE HIGH"
/* regexp to delete all pixtable limits */
#define MUSE_HDR_PT_LIMITS_REGEXP "^ESO DRS MUSE PIXTABLE LIMITS "

/* limits in spatial direction before applying DAR correction */
/** FITS header keyword contains the lower limit of the data in x-direction before DAR correction. */
#define MUSE_HDR_PT_PREDAR_XLO "ESO DRS MUSE PIXTABLE PREDAR LIMITS XLO"
/** FITS header keyword contains the upper limit of the data in x-direction before DAR correction. */
#define MUSE_HDR_PT_PREDAR_XHI "ESO DRS MUSE PIXTABLE PREDAR LIMITS XHI"
/** FITS header keyword contains the lower limit of the data in y-direction before DAR correction. */
#define MUSE_HDR_PT_PREDAR_YLO "ESO DRS MUSE PIXTABLE PREDAR LIMITS YLO"
/** FITS header keyword contains the upper limit of the data in y-direction before DAR correction. */
#define MUSE_HDR_PT_PREDAR_YHI "ESO DRS MUSE PIXTABLE PREDAR LIMITS YHI"

/* headers and comments to describe the status of the pixel table */
/** Flat-field corrected pixel table, number is the IFUs that had flat-fields */
#define MUSE_HDR_PT_FFCORR             "ESO DRS MUSE PIXTABLE FFCORR" /* int */
#define MUSE_HDR_PT_FFCORR_COMMENT     "Pixel table corrected for flat-field spectrum"
/** Merged IFUs that went into this pixel table */
#define MUSE_HDR_PT_MERGED             "ESO DRS MUSE PIXTABLE MERGED" /* int */
#define MUSE_HDR_PT_MERGED_COMMENT     "Merged IFUs that went into this pixel table"
/** Pixel table was sky-subtracted */
#define MUSE_HDR_PT_SKYSUB             "ESO DRS MUSE PIXTABLE SKYSUB" /* bool */
#define MUSE_HDR_PT_SKYSUB_COMMENT     "Pixel table was sky-subtracted"
/* headers and comments to describe the DAR status */
/** theoretical DAR correction done with this reference wavelength [Angstrom] */
#define MUSE_HDR_PT_DAR_NAME           "ESO DRS MUSE PIXTABLE DAR CORRECT"
#define MUSE_HDR_PT_DAR_COMMENT        "[Angstrom] ref. lambda for DAR correction"
#define MUSE_HDR_PT_DAR_C_SKIP         "skipped DAR correction!"
/** empirical DAR check done, found this maximum residual DAR offsets [arcsec] */
#define MUSE_HDR_PT_DAR_CHECK          "ESO DRS MUSE PIXTABLE DAR CHECK"
#define MUSE_HDR_PT_DAR_CHECK_C        "[arcsec] maximum residual DAR offsets found"
/** empirical DAR correction done, relative to this reference wavelength [Angstrom] */
#define MUSE_HDR_PT_DAR_CORR           "ESO DRS MUSE PIXTABLE DAR CORRECT RESIDUAL"
#define MUSE_HDR_PT_DAR_CORR_C         "[Angstrom] ref. lambda for residual DAR correction"
/** autocalibration checks */
#define MUSE_HDR_PT_AUTOCAL_NAME       "ESO DRS MUSE PIXTABLE AUTOCAL"
#define MUSE_HDR_PT_AUTOCAL_COMMENT    "%.3f (autocalibration clipping limit)"
/** radial-velocity correction done, with the given velocity shift and reference */
#define MUSE_HDR_PT_RVCORR             "ESO DRS MUSE PIXTABLE RVCORR"
#define MUSE_HDR_PT_RVCORR_C           "[km/s] %scentric correction was applied"
/** Pixel table was flux-calibrated */
#define MUSE_HDR_PT_FLUXCAL            "ESO DRS MUSE PIXTABLE FLUXCAL" /* bool */
#define MUSE_HDR_PT_FLUXCAL_COMMENT    "Pixel table was flux-calibrated"
/** Spatial world coordinate transformation applied to this pixel table */
#define MUSE_HDR_PT_WCS                "ESO DRS MUSE PIXTABLE WCS" /* string */
/** Value of @ref MUSE_HDR_PT_WCS with Gnomonic projection applied to this pixel table */
#define MUSE_HDR_PT_WCS_PROJ           "projected (intermediate)"
#define MUSE_HDR_PT_WCS_COMMENT_PROJ   "Gnomonic projection applied to this pixel table"
/** Value of @ref MUSE_HDR_PT_WCS after positioning this pixel table to sky coordinates */
#define MUSE_HDR_PT_WCS_POSI           "positioned (final)"
#define MUSE_HDR_PT_WCS_COMMENT_POSI   "Positioned this pixel table to sky coordinates"
/** Pixel table was weighted relative to other exposures */
#define MUSE_HDR_PT_WEIGHTED           "ESO DRS MUSE PIXTABLE WEIGHTED" /* bool */
#define MUSE_HDR_PT_WEIGHTED_COMMENT   "Pixel table was weighted relative to other exposures"
/** Combined exposures that went into this pixel table */
#define MUSE_HDR_PT_COMBINED           "ESO DRS MUSE PIXTABLE COMBINED" /* int */
#define MUSE_HDR_PT_COMBINED_COMMENT   "Combined exposures that went into this pixel table"
/** Spectral type code, set after converting from air to vacuum */
#define MUSE_HDR_PT_SPEC_TYPE          "ESO DRS MUSE PIXTABLE SPECTYPE" /* string */
#define MUSE_HDR_PT_SPEC_TYPE_COMMENT  "FITS spectral type code of wavelength"

/* regular expression to remove all pixtable-related keyword, *
 * e.g. when transforming to 2D image or 3D cube              */
#define MUSE_HDR_PT_REGEXP "^ESO DRS MUSE PIXTABLE "

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of MUSE pixel table

  Type to store MUSE pixel tables a table of coordinates and pixel values is
  stored together with a FITS header.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief The @ref muse_pixtable_def "pixel table".

    @copydetails muse_pixtable_def
   */
  cpl_table *table;

  /**
    @brief The FITS header.

    This contains the FITS keywords that are read from or written to the
    output file.
   */
  cpl_propertylist *header;

  /**
    @brief A flat-field spectrum.

    This may contain the (average) flat-field spectrum of an IFU or an
    exposure, in the format of a two-column table. The columns then are
    MUSE_PIXTABLE_FFLAMBDA and MUSE_PIXTABLE_FFDATA.
   */
  cpl_table *ffspec;
} muse_pixtable;

/*----------------------------------------------------------------------------*/
/**
  @brief    Type of a MUSE pixel table
 */
/*----------------------------------------------------------------------------*/
enum muse_pixtable_type {
  /** Unknown type of pixel table */
  MUSE_PIXTABLE_TYPE_UNKNOWN = 0,
  /** Pixel table with simple geometry information (no y coordinate) */
  MUSE_PIXTABLE_TYPE_SIMPLE,
  /** Pixel table with full geometry information */
  MUSE_PIXTABLE_TYPE_FULL
};

/*----------------------------------------------------------------------------*/
/**
  @brief    Type of operation to apply to a MUSE pixel table
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_PIXTABLE_OPERATION_SUBTRACT = 0,
  MUSE_PIXTABLE_OPERATION_MULTIPLY,
  MUSE_PIXTABLE_OPERATION_DIVIDE
} muse_pixtable_operation;

/*----------------------------------------------------------------------------*/
/**
  @brief    State of the astrometric calibration of a MUSE pixel table
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  /** Undefined state (or axes disagree) */
  MUSE_PIXTABLE_WCS_UNKNOWN = 0,
  /** Original pixel positions [pix] */
  MUSE_PIXTABLE_WCS_PIXEL,
  /** Native spherical coordinates [rad] */
  MUSE_PIXTABLE_WCS_NATSPH,
  /** Celestial spherical coordinates [deg] */
  MUSE_PIXTABLE_WCS_CELSPH
} muse_pixtable_wcs;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
/* utility functions dealing with the origin column */
uint32_t muse_pixtable_origin_encode(unsigned int, unsigned int, unsigned short, unsigned short, unsigned int);
unsigned int muse_pixtable_origin_get_x(uint32_t, muse_pixtable *, cpl_size);
unsigned int muse_pixtable_origin_get_y(uint32_t);
unsigned short muse_pixtable_origin_get_ifu(uint32_t);
unsigned short muse_pixtable_origin_get_slice(uint32_t);
unsigned int muse_pixtable_origin_set_offset(muse_pixtable *, cpl_polynomial *, unsigned short, unsigned short);
unsigned int muse_pixtable_origin_get_offset(muse_pixtable *, unsigned int, unsigned short, unsigned short);
cpl_error_code muse_pixtable_origin_copy_offsets(muse_pixtable *, muse_pixtable *, unsigned int);
cpl_error_code muse_pixtable_origin_decode_all(muse_pixtable *, unsigned short **, unsigned short **, unsigned char **, unsigned char **);
unsigned int muse_pixtable_get_expnum(muse_pixtable *, cpl_size);

/* the four main functions */
muse_pixtable *muse_pixtable_create(muse_image *, cpl_table *, cpl_table *, cpl_table *);
muse_pixtable *muse_pixtable_duplicate(muse_pixtable *);
void muse_pixtable_delete(muse_pixtable *);
cpl_error_code muse_pixtable_append_ff(muse_pixtable *, muse_image *, cpl_table *, cpl_table *, float);
cpl_error_code muse_pixtable_save(muse_pixtable *, const char *);
muse_pixtable *muse_pixtable_load_window(const char *, cpl_size, cpl_size);
muse_pixtable *muse_pixtable_load(const char *);
muse_pixtable *muse_pixtable_load_restricted_wavelength(const char *, double, double);
muse_pixtable *muse_pixtable_load_merge_channels(cpl_table *, double, double);

/* other utility functions for MUSE pixel tables */
int muse_pixtable_get_type(muse_pixtable *);
cpl_size muse_pixtable_get_nrow(const muse_pixtable *);
cpl_error_code muse_pixtable_compute_limits(muse_pixtable *);
cpl_error_code muse_pixtable_flux_multiply(muse_pixtable *, double);
cpl_error_code muse_pixtable_spectrum_apply(muse_pixtable *, const cpl_array *, const cpl_array *, muse_pixtable_operation);

cpl_error_code muse_pixtable_restrict_wavelength(muse_pixtable *, double, double);
cpl_error_code muse_pixtable_restrict_xpos(muse_pixtable *, double, double);
cpl_error_code muse_pixtable_restrict_ypos(muse_pixtable *, double, double);
cpl_error_code muse_pixtable_erase_ifu_slice(muse_pixtable *, unsigned char, unsigned short);
cpl_error_code muse_pixtable_and_selected_mask(muse_pixtable *, const muse_mask *, const cpl_propertylist *, const cpl_table *);

cpl_error_code muse_pixtable_dump(muse_pixtable *, cpl_size, cpl_size, unsigned char);
muse_pixtable_wcs muse_pixtable_wcs_check(muse_pixtable *);
cpl_boolean muse_pixtable_is_fluxcal(muse_pixtable *);
cpl_boolean muse_pixtable_is_skysub(muse_pixtable *);
cpl_boolean muse_pixtable_is_rvcorr(muse_pixtable *);

cpl_error_code muse_pixtable_reset_dq(muse_pixtable *, unsigned int);

muse_imagelist *muse_pixtable_to_imagelist(muse_pixtable *);
cpl_error_code muse_pixtable_from_imagelist(muse_pixtable *, muse_imagelist *);

muse_pixtable *muse_pixtable_extract_wavelength(muse_pixtable *, double,
                                                double);
muse_pixtable **muse_pixtable_extracted_get_slices(muse_pixtable *);
cpl_size muse_pixtable_extracted_get_size(muse_pixtable **);
void  muse_pixtable_extracted_delete(muse_pixtable **);

#endif /* MUSE_PIXTABLE_H */
