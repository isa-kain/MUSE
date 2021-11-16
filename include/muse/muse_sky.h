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

#ifndef MUSE_SKY_H
#define MUSE_SKY_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_cplwrappers.h"
#include "muse_lsf.h"
#include "muse_pixtable.h"
#include "muse_resampling.h"
#include "muse_utils.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
extern const muse_cpltable_def muse_fluxspectrum_def[];
extern const muse_cpltable_def muse_relativespectrum_def[];
extern const muse_cpltable_def muse_extinction_def[];

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/
/** @addtogroup muse_skysub */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
   @brief Structure to define the major settable sky parameters
*/
/*----------------------------------------------------------------------------*/
typedef struct {
  double ignore;
  double fraction;
  double sampling;
  double csampling;
  double crsigmac; /* CR rejection sigma level for cube-based cleaning */
  double crsigmas; /* CR rejection sigma level for spectrum-based cleaning */
} muse_sky_params;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
/* common functions (muse_sky_common.c) */
cpl_table *muse_sky_spectrum_from_cube(muse_datacube *, const cpl_mask *);
muse_mask *muse_sky_create_skymask(muse_image *, double, double, const char *);
cpl_error_code muse_sky_save_continuum(muse_processing *, const cpl_table *, cpl_propertylist *);

/* functions about lines (muse_sky_lines.c) */
cpl_error_code muse_sky_lines_set_range(cpl_table *, double, double);
cpl_error_code muse_sky_lines_save(muse_processing *, const cpl_table *, cpl_propertylist *);
cpl_table *muse_sky_lines_load(muse_processing *);
cpl_error_code muse_sky_lines_apply_strength(cpl_table *, const cpl_array *);
cpl_error_code muse_sky_lines_cut(cpl_table *, double);
cpl_table *muse_sky_lines_create(const cpl_table *, const cpl_table *, double);

/* functions about master sky creation (muse_sky_master.c) */
cpl_error_code muse_sky_lines_fit(cpl_table *, cpl_table *, cpl_image *, muse_wcs *);
cpl_table *muse_sky_continuum_create(cpl_table *, cpl_table *, cpl_image *, muse_wcs *, double);
cpl_table *muse_sky_continuum_load(muse_processing *);

/* functios about QC parameters (muse_sky_qc.c) */
void muse_sky_qc_lines(cpl_propertylist *, cpl_table *, const char *);
void muse_sky_qc_continuum(cpl_propertylist *, cpl_table *, const char *);

/* functions about sky subtraction (muse_sky_subtract.c) */
cpl_array *muse_sky_lines_spectrum(const cpl_array *, cpl_table *, const cpl_image *, const muse_wcs *);
cpl_error_code muse_sky_subtract_lines(muse_pixtable *, cpl_table *, muse_lsf_cube **);
cpl_error_code muse_sky_subtract_continuum(muse_pixtable *, cpl_table *);

#endif /* MUSE_SKY_H */
