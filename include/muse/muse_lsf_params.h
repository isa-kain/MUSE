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

#ifndef MUSE_LSF_PARAMS_H
#define MUSE_LSF_PARAMS_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_cplwrappers.h"
#include "muse_pixtable.h"
#include "muse_processing.h"

#define USE_LSF_PARAMS // define this for support of (old) LSF table support

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/
/** @addtogroup muse_lsf */
/**@{*/

#define MAX_HERMIT_ORDER 4

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of detector (slice) parameters.

  This structure is used to hold the detector parameters for the master and
  the individual sky calculation.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  int ifu;
  int slice;
  double refraction;
  double offset;
  /** @brief Reference wavelength for polynomial parametrizations. */
  double lambda_ref;
  /** @brief Relative detector sensitivity parametrization */
  cpl_array *sensitivity;
  /** @brief Slit width */
  double slit_width;
  /** @brief Bin width */
  double bin_width;
  /** @brief LSF width */
  cpl_array *lsf_width;
  /** @brief coefficients for the damped gauss-hermitean parametrization */
  cpl_array *hermit[MAX_HERMIT_ORDER];
} muse_lsf_params;
/*----------------------------------------------------------------------------*/
/**
   @brief Structure to define which slice parameters are fit.
*/
/*----------------------------------------------------------------------------*/
typedef struct {
  cpl_size n_param;
  cpl_size offset;
  cpl_size refraction;
  cpl_size sensitivity;
  cpl_size slit_width;
  cpl_size bin_width;
  cpl_size lsf_width;
  cpl_size hermit[MAX_HERMIT_ORDER];
} muse_lsf_fit_params;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/

muse_lsf_params *muse_lsf_params_new(cpl_size, cpl_size, cpl_size);
cpl_size muse_lsf_params_get_size(muse_lsf_params **);
void muse_lsf_params_delete(muse_lsf_params *);
void muse_lsf_params_delete_all(muse_lsf_params **);
cpl_error_code muse_lsf_params_save(const muse_lsf_params **, const char *);
muse_lsf_params **muse_lsf_params_load(const char *, muse_lsf_params **, int);
muse_lsf_params **muse_processing_lsf_params_load(muse_processing *, int);
muse_lsf_params *muse_lsf_params_get(muse_lsf_params **, int, int);

cpl_error_code muse_lsf_params_apply(const muse_lsf_params *,
                                     cpl_array *, double);
cpl_array *muse_lsf_params_spectrum(const cpl_array *, cpl_table *,
                                    const muse_lsf_params *);
muse_lsf_params *muse_lsf_params_fit(muse_pixtable *, cpl_table *, int);
double muse_lsf_fwhm_lambda(const muse_lsf_params *, double,
                            double, unsigned int);


#endif /* MUSE_LSF_PARAMS_H */
