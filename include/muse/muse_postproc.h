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

#ifndef MUSE_POSTPROC_H
#define MUSE_POSTPROC_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_autocalib.h"
#include "muse_flux.h"
#include "muse_lsf.h"
#include "muse_lsf_params.h"
#include "muse_pixtable.h"
#include "muse_processing.h"
#include "muse_resampling.h"
#include "muse_rvcorrect.h"
#include "muse_sky.h"
#include "muse_table.h"
#include "muse_wcs.h"
#include "muse_xcombine.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *                           Data structures                                  *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_postproc */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
 * @brief Type of per-exposure processing to run.
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_POSTPROC_SCIPOST = 0,
  MUSE_POSTPROC_STANDARD,
  MUSE_POSTPROC_ASTROMETRY
} muse_postproc_type;

/*----------------------------------------------------------------------------*/
/**
 * @brief Method of sky subtraction to apply.
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_POSTPROC_SKYMETHOD_NONE = 0,
  MUSE_POSTPROC_SKYMETHOD_MODEL,
  MUSE_POSTPROC_SKYMETHOD_SIMPLE
} muse_postproc_skymethod;

/*----------------------------------------------------------------------------*/
/**
 * @brief Method of DAR checking/correction to do
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_POSTPROC_DARCHECK_NONE = 0,
  MUSE_POSTPROC_DARCHECK_CHECK,
  MUSE_POSTPROC_DARCHECK_CORRECT
} muse_postproc_darcheck;

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of the post-processing properties.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  muse_postproc_type type; /**< the processing type */
  cpl_table *exposures; /**< the exposure table */
  double lambdamin; /**< blue limit of the wavelength range */
  double lambdamax; /**< red limit of the wavelength range */
  double lambdaref; /**< reference wavelength for DAR correction */
  muse_postproc_darcheck darcheck; /**< if to check or even correct DAR */
  muse_rvcorrect_type rvtype; /**< type of radial velocity correction to apply */
  muse_table *response; /**< flux response table */
  muse_table *telluric; /**< the telluric correction table */
  cpl_table *extinction; /**< the extinction curve table */
  cpl_propertylist *wcs; /**< FITS headers with astrometry calibration */
  cpl_boolean astrometry; /**< if true, carry out astrometric calibration */
  cpl_table *raman_lines; /**< the Raman line list */
  double raman_width; /**< wavelength range around Raman lines */
  muse_postproc_skymethod skymethod; /**< the sky subtraction method */
  cpl_table *sky_lines; /**< the sky lines table */
  cpl_table *sky_continuum; /**< the sky continuum spectrum table */
  muse_lsf_cube **lsf_cube; /**< the LSF cube of the instrument */
#ifdef USE_LSF_PARAMS
  muse_lsf_params **lsf_params; /**< the LSF parameters of the instrument (alternative to lsf_cube) */
#endif
  muse_mask *sky_mask; /**< The sky mask */
  muse_sky_params skymodel_params; /**< Parameters for the sky model fit */
  muse_autocalib_type autocalib; /**< Type of autocalibration to run, if any */
  muse_table *autocal_table; /**< User table with autocalibration factors */
  cpl_frame *refframe; /**< reference frame (flux response reference *
                        *   or astrometry position reference)        */
  cpl_table *tellregions; /**< telluric regions table */
  muse_flux_profile_type profile; /**< the profile to use for flux integration */
  muse_flux_selection_type select; /**< how to select the standard star */
  muse_spectrum_smooth_type smooth; /**< how to smooth the output response curve */
  double detsigma; /**< the detection sigma for the astrometric field */
  double radius; /**< initial radius in pixels for pattern matching *
                  *   identification in the astrometric field       */
  double faccuracy; /**< factor of initial accuracy relative to mean positional *
                     *   accuracy of the centroids in the the astrometric field */
  double rejsigma; /**< the rejection sigma for the iterative astrometric fit */
  int niter; /**< the number of iterations for the iterative fit */
  muse_wcs_centroid_type centroid; /**< the centroiding method to use *
                                    *   for astrometric calibration   */
  double crpix1, crpix2; /**< center of rotation in pixel coordinates */
} muse_postproc_properties;

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of the post-processing output auto-calibration.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  muse_mask *mask; /**< The effective output sky mask, for a given exposure */
  muse_table *table; /**< The results (slice factors) of the auto-calibration */
} muse_postproc_autocal_outputs;

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of the post-processing output sky data.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  muse_image *image; /**< The reconstructed image used for sky masking */
  muse_mask *mask; /**< The effective output sky mask, for a given exposure */
  cpl_table *spectrum; /**< The effective output sky spectrum, for a given exposure */
  cpl_table *lines; /**< The effective output sky lines list, for a given exposure */
  cpl_table *continuum; /**< The effective output sky continuum, for a given exposure */
} muse_postproc_sky_outputs;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_postproc_properties *muse_postproc_properties_new(muse_postproc_type);
void muse_postproc_properties_delete(muse_postproc_properties *);

cpl_boolean muse_postproc_check_save_param(const char *, const char *);
muse_resampling_type muse_postproc_get_resampling_type(const char *);
muse_resampling_crstats_type muse_postproc_get_cr_type(const char *);
muse_cube_type muse_postproc_get_cube_format(const char *);
muse_xcombine_types muse_postproc_get_weight_type(const char *);

void muse_postproc_offsets_scale(muse_pixtable *, const cpl_table *, const char *);
cpl_table *muse_postproc_load_nearest(const cpl_propertylist *, const cpl_frame *, float, float, double *, double *);
cpl_error_code muse_postproc_revert_ffspec_maybe(muse_pixtable *, const muse_table *);
muse_image *muse_postproc_whitelight(muse_pixtable *, double);
cpl_error_code muse_postproc_correct_raman(muse_pixtable *, muse_postproc_properties *, const cpl_table *, muse_datacube **);
void *muse_postproc_process_exposure(muse_postproc_properties *, unsigned int, muse_postproc_autocal_outputs *, muse_datacube **, muse_postproc_sky_outputs *, const cpl_table *);

cpl_propertylist *muse_postproc_cube_load_output_wcs(muse_processing *);
cpl_error_code muse_postproc_cube_resample_and_collapse(muse_processing *, muse_pixtable *, muse_cube_type, muse_resampling_params *, const char *);
cpl_error_code muse_postproc_qc_fwhm(muse_processing *, muse_datacube *);

#endif /* MUSE_POSTPROC_H */
