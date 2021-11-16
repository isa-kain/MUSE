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

#ifndef MUSE_RESAMPLING_H
#define MUSE_RESAMPLING_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_cplwrappers.h"
#include "muse_datacube.h"
#include "muse_image.h"
#include "muse_pixgrid.h"
#include "muse_pixtable.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_resampling */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Resampling types
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  /** Nearest neighbor resampling */
  MUSE_RESAMPLE_NEAREST = 0,
  /** Weighted resampling using Renka weighting function */
  MUSE_RESAMPLE_WEIGHTED_RENKA = 1,
  /** Weighted resampling using inverse distance weighting function */
  MUSE_RESAMPLE_WEIGHTED_LINEAR,
  /** Weighted resampling using quadratic inverse distance weighting function */
  MUSE_RESAMPLE_WEIGHTED_QUADRATIC,
  /** Weighted resampling using a drizzle-like weighting scheme */
  MUSE_RESAMPLE_WEIGHTED_DRIZZLE,
  /** Weighted resampling using a lanczos-like restricted sinc for weighting */
  MUSE_RESAMPLE_WEIGHTED_LANCZOS,
  /** No resampling, just create pixel grid; last value, can be used for range checking */
  MUSE_RESAMPLE_NONE
} muse_resampling_type;

/*----------------------------------------------------------------------------*/
/**
  @brief    Cosmic ray rejection statistics type
 */
/*----------------------------------------------------------------------------*/
/* keep this in sync with crtypestring[] in muse_resampling.c! */
typedef enum {
  /** IRAF-like statistics for cosmic ray rejection, *
   *  using mean and the data variance               */
  MUSE_RESAMPLING_CRSTATS_IRAF = 0,
  /** Mean/standard deviation statistics for cosmic ray rejection */
  MUSE_RESAMPLING_CRSTATS_MEAN = 1,
  /** Median/MAD statistics for cosmic ray rejection */
  MUSE_RESAMPLING_CRSTATS_MEDIAN = 2
} muse_resampling_crstats_type;

typedef enum {
  /** linear air-wavelength (CTYPEj="AWAV") */
  MUSE_RESAMPLING_DISP_AWAV = 0,
  /** logarithmic air-wavelength (CTYPEj="AWAV-LOG") */
  MUSE_RESAMPLING_DISP_AWAV_LOG = 1,
  /** linear vacuum-wavelength (CTYPEj="WAVE") */
  MUSE_RESAMPLING_DISP_WAVE,
  /** logarithmic vacuum-wavelength (CTYPEj="WAVE-LOG") */
  MUSE_RESAMPLING_DISP_WAVE_LOG
} muse_resampling_dispersion_type;

/*----------------------------------------------------------------------------*/
/**
  @brief    Resampling parameters
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /** The resampling method to use */
  muse_resampling_type method;
  /** statistics type for cosmic ray rejection */
  muse_resampling_crstats_type crtype;
  /** high sigma to use for cosmic ray rejection */
  double crsigma;

  /** loop distance, used for all weighted resampling schemes */
  int ld;
  /** the pixfrac parameter of the drizzle method: down-scaling factor  *
   *  of input pixel size before computing drizzling weights; different *
   *  values for x-, y-, and lambda directions are possible             */
  double pfx, pfy, pfl;
  /** critical radius of the Renka-weighted method */
  double rc;

  /** simple delta sizes as parameters                                   *
   *  [arcsec] or [pix] for spatial direction, [Angstrom] for wavelength */
  double dx, dy, dlambda;
  /** the full 3-axis world coordinates for the output grid */
  cpl_wcs *wcs;
  /** type of wavelength scale in dispersion direction */
  muse_resampling_dispersion_type tlambda;
} muse_resampling_params;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_resampling_params *muse_resampling_params_new(muse_resampling_type);
cpl_error_code muse_resampling_params_set_pixfrac(muse_resampling_params *, const char *);
cpl_error_code muse_resampling_params_set_wcs(muse_resampling_params *, const cpl_propertylist *);
void muse_resampling_params_delete(muse_resampling_params *);

muse_euro3dcube *muse_resampling_euro3d(muse_pixtable *, muse_resampling_params *);
muse_datacube *muse_resampling_cube(muse_pixtable *, muse_resampling_params *, muse_pixgrid **);
muse_image *muse_resampling_collapse_pixgrid(muse_pixtable *, muse_pixgrid *, muse_datacube *, const muse_table *, muse_resampling_params *);

muse_image *muse_resampling_image(muse_pixtable *, muse_resampling_type, double, double);
cpl_table *muse_resampling_spectrum(muse_pixtable *, double);
cpl_table *muse_resampling_spectrum_iterate(muse_pixtable *, double, float, float, unsigned char);

#endif /* MUSE_RESAMPLING_H */
