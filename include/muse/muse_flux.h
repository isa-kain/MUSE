/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2005-2017 European Southern Observatory
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

#ifndef MUSE_FLUX_H
#define MUSE_FLUX_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_image.h"
#include "muse_pixtable.h"
#include "muse_resampling.h"
#include "muse_table.h"
#include "muse_utils.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
/* flux scaling units expressed according to the FITS Standard v3.0, §4.3 */
static const char kMuseFluxUnitString[] = "10**(-20)*erg/s/cm**2/Angstrom";
static const char kMuseFluxStatString[] = "(10**(-20)*erg/s/cm**2/Angstrom)**2";
static const double kMuseFluxUnitFactor = 1e20;
static const double kMuseFluxStatFactor = 1e40;

/* FITS headers for storing detected sources found *
 * by the standard star flux integration routines  */
#define MUSE_HDR_FLUX_NOBJ        "ESO DRS MUSE FLUX NOBJ"
#define MUSE_HDR_FLUX_NOBJ_C      "Number of objects detected in standard star cube"
#define MUSE_HDR_FLUX_OBJn_X      "ESO DRS MUSE FLUX OBJ%d X"
#define MUSE_HDR_FLUX_OBJn_X_C    "[pix] Object x position in cube"
#define MUSE_HDR_FLUX_OBJn_Y      "ESO DRS MUSE FLUX OBJ%d Y"
#define MUSE_HDR_FLUX_OBJn_Y_C    "[pix] Object y position in cube"
#define MUSE_HDR_FLUX_OBJn_RA     "ESO DRS MUSE FLUX OBJ%d RA"
#define MUSE_HDR_FLUX_OBJn_RA_C   "[deg] Approximate RA coordinate of object"
#define MUSE_HDR_FLUX_OBJn_DEC    "ESO DRS MUSE FLUX OBJ%d DEC"
#define MUSE_HDR_FLUX_OBJn_DEC_C  "[deg] Approximate DEC coordinate of object"
#define MUSE_HDR_FLUX_OBJn_FLUX   "ESO DRS MUSE FLUX OBJ%d INTFLUX"
#define MUSE_HDR_FLUX_OBJn_FLUX_C "[10**(-20)*erg/s/cm**2] Total flux integrated for this object"
#define MUSE_HDR_FLUX_NSEL        "ESO DRS MUSE FLUX NSEL"
#define MUSE_HDR_FLUX_NSEL_C      "Object (%s) selected as standard star"

/* FITS header to mark flat-field spectrum corrected products */
#define MUSE_HDR_FLUX_FFCORR   "ESO DRS MUSE FLUX FFCORR" /* bool */
#define MUSE_HDR_FLUX_FFCORR_C "Data was flat-field spectrum corrected"

/*----------------------------------------------------------------------------*
 *                           Data structures                                  *
 *----------------------------------------------------------------------------*/
extern const muse_cpltable_def muse_response_tellbands_def[];

/** @addtogroup muse_flux */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
 * @brief Flux object to store data needed while computing the flux calibration.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /** the datacube created for the standard star exposure */
  muse_datacube *cube;
  /** the three extension muse_image to store the *
   *  integrated fluxes and their error estimates */
  muse_image *intimage;
  /** the flux reference curve of the standard star */
  cpl_table *reference;
  /** the sensitivity (ratio of expected to measured flux in magnitudes), *
   *  plus additional data in table form                                  */
  cpl_table *sensitivity;
  /** the flux response curve in table form (with header) */
  muse_table *response;
  /** the telluric correction in table form (with header) */
  muse_table *telluric;
  /** the regions of the telluric bands in table form */
  cpl_table *tellbands;
  /** the right ascension of the reference object in degrees */
  double raref;
  /** the declination of the reference object in degrees */
  double decref;
} muse_flux_object;

/*----------------------------------------------------------------------------*/
/**
 * @brief Type of table interpolation to use.
 *
 * There are several types of tables containing something like a "response"
 * curve which contain at least a column "lambda" relevant to the MUSE pipeline.
 * They all have to be interpolated (linearly) in wavelength. Depending on their
 * content they have to be handled in slightly different ways.
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_FLUX_RESP_FILTER = 0, /**< filter response curve; column response */
  MUSE_FLUX_RESP_FLUX, /**< flux response curve; columns response, resperr */
  MUSE_FLUX_RESP_STD_FLUX, /**< standard star spectrum; columns flux, fluxerr */
  MUSE_FLUX_RESP_EXTINCT, /**< extinction curve; column extinction */
  MUSE_FLUX_TELLURIC /**< telluric correction table: columns ftelluric, ftellerr */
} muse_flux_interpolation_type;

/*----------------------------------------------------------------------------*/
/**
 * @brief Type of optimal profile to use.
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_FLUX_PROFILE_MOFFAT = 0, /**< Moffat profile */
  MUSE_FLUX_PROFILE_SMOFFAT, /**< "smoothed" Moffat profile */
  MUSE_FLUX_PROFILE_GAUSSIAN, /**< Gaussian profile */
  MUSE_FLUX_PROFILE_CIRCLE, /**< circular flux integration, with background annulus */
  MUSE_FLUX_PROFILE_EQUAL_SQUARE, /**< simplest version: non-optimal square integration */
  MUSE_FLUX_PROFILE_AUTO /**< automatic best selection (smoffat for WFM, circle for NFM) */
} muse_flux_profile_type;

/*----------------------------------------------------------------------------*/
/**
 * @brief Type of star selection to use.
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_FLUX_SELECT_BRIGHTEST = 0, /**< take the brightest star as standard */
  MUSE_FLUX_SELECT_NEAREST /**< take the nearest source as standard */
} muse_flux_selection_type;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_flux_object *muse_flux_object_new(void);
void muse_flux_object_delete(muse_flux_object *aFluxObj);

cpl_error_code muse_flux_reference_table_check(cpl_table *);
double muse_flux_response_interpolate(const cpl_table *, double, double *, muse_flux_interpolation_type);
muse_image *muse_flux_integrate_cube(muse_datacube *, cpl_apertures *, muse_flux_profile_type);
cpl_error_code muse_flux_integrate_std(muse_pixtable *, muse_flux_profile_type, muse_flux_object *);
cpl_error_code muse_flux_response_compute(muse_flux_object *, muse_flux_selection_type, double, const cpl_table *, const cpl_table *, const cpl_table *);
cpl_error_code muse_flux_get_response_table(muse_flux_object *, muse_spectrum_smooth_type);
cpl_error_code muse_flux_get_telluric_table(muse_flux_object *);
cpl_error_code muse_flux_compute_qc(muse_flux_object *);
cpl_error_code muse_flux_compute_qc_zp(muse_flux_object *, const muse_table *, const char *);

cpl_error_code muse_flux_calibrate(muse_pixtable *, const muse_table *, const cpl_table *, const muse_table *);

#endif /* MUSE_FLUX_H */
