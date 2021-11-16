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

#ifndef MUSE_WAVECALIB_H
#define MUSE_WAVECALIB_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_image.h"
#include "muse_imagelist.h"
#include "muse_pfits.h"
#include "muse_table.h"

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_wavecalib */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
 * @brief Type of weighting to use in the wavelength calibration fit.
 */
/*----------------------------------------------------------------------------*/
/* keep this in sync with muse_wave_weighting_string[] in muse_wavecalib.c! */
typedef enum {
  MUSE_WAVE_WEIGHTING_UNIFORM = 0, /**< no weighting, i.e. uniform weights */
  MUSE_WAVE_WEIGHTING_CERR, /**< weighting by inverse centroiding error */
  MUSE_WAVE_WEIGHTING_SCATTER, /**< weighting by inverse RMS scatter of each line */
  MUSE_WAVE_WEIGHTING_CERRSCATTER /**< weighting by combined inverse centroiding  *
                                   *   error and inverse RMS scatter of each line */
} muse_wave_weighting_type;

/*----------------------------------------------------------------------------*/
/**
  @brief   Structure containing wavelength calibration parameters.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /** Polynomial order to model slice curvature (horizontal) */
  unsigned short xorder;
  /** Polynomial order in dispersion direction (vertical) */
  unsigned short yorder;
  /** Criterion for an arc line detection: sigma of median deviation of a *
   *peak above the median background in the S/N image                     */
  double detsigma;
  /** Expected dispersion variation [Angstrom/pixel] for pattern matching */
  double ddisp;
  /** Tolerance with which to correlate line list and detected lines during *
   *  pattern matching.                                                     */
  double tolerance;
  /** Sigma level for iterative rejection of deviant fits for each arc line *
   * within each slice. If negative, let the algorithm choose a default.    */
  double linesigma;
  /** Boolean flag to generate residuals table in the residuals component */
  cpl_boolean rflag;
  /** Table that is filled with debug information on wavelength calibration *
   *  residuals                                                             */
  cpl_table *residuals;
  /** Sigma level for iterative rejection of deviant datapoints during the *
   *  final polynomial wavelength solution within each slice. If negative, *
   *  let the algorithm select a default.                                  */
  double fitsigma;
  /** Target RMS that when reached signifies convergence of the iterative fit */
  double targetrms;
  /** Type of weighting to use in the final polynomial wavelength solution *
   *  fit                                                                  */
  muse_wave_weighting_type fitweighting;
  /** Instrument mode */
  muse_ins_mode mode;
} muse_wave_params;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_wave_params *muse_wave_params_new(cpl_propertylist *);
void muse_wave_params_delete(muse_wave_params *);

cpl_table *muse_wave_calib(muse_image *, cpl_table *, cpl_table *, muse_wave_params *);
cpl_table *muse_wave_calib_lampwise(muse_imagelist *, cpl_table *, cpl_table *, muse_wave_params *);

cpl_boolean muse_wave_lines_check(muse_table *);
cpl_vector *muse_wave_lines_get(cpl_table *, int, double);
cpl_vector *muse_wave_lines_get_for_lamp(cpl_table *, const char *, int, double);
const char *muse_wave_lines_get_lampname(cpl_table *, const int);
cpl_boolean muse_wave_lines_covered_by_data(double, muse_ins_mode);
cpl_table *muse_wave_lines_search(muse_image *, double, const unsigned short, const unsigned char);
cpl_error_code muse_wave_lines_identify(cpl_table *, cpl_vector *, const muse_wave_params *);

cpl_table *muse_wave_line_handle_singlet(muse_image *, cpl_table *, unsigned int, cpl_polynomial *, cpl_polynomial **,   const muse_wave_params *, const unsigned short, int);
cpl_table *muse_wave_line_handle_multiplet(muse_image *, cpl_table *, unsigned int, cpl_polynomial *, cpl_polynomial **, const muse_wave_params *, const unsigned short, int);
cpl_error_code muse_wave_line_fit_single(muse_image *, int, double, int, double, cpl_table *, int);
cpl_error_code muse_wave_line_fit_multiple(muse_image *, int, cpl_bivector *, cpl_vector *, int, double, cpl_table *, int);
cpl_error_code muse_wave_line_fit_iterate(cpl_table *, double, const muse_wave_params *aParams);

cpl_error_code muse_wave_poly_fit(cpl_matrix *, cpl_vector *, cpl_vector *, cpl_polynomial **, double *, muse_wave_params *, const unsigned short);

cpl_table *muse_wave_table_create(const unsigned short, const unsigned short, const unsigned short);
cpl_error_code muse_wave_table_add_poly(cpl_table *, cpl_polynomial *, double, unsigned short, unsigned short, const unsigned short);
cpl_error_code muse_wave_table_get_orders(const cpl_table *, unsigned short *, unsigned short *);
cpl_polynomial *muse_wave_table_get_poly_for_slice(const cpl_table *, unsigned short);

cpl_image *muse_wave_map(muse_image *, const cpl_table *, const cpl_table *);

cpl_error_code muse_wave_plot_residuals(cpl_table *, unsigned char, unsigned short, unsigned int, cpl_boolean, cpl_vector *);
cpl_error_code muse_wave_plot_column(cpl_table *, cpl_table *, unsigned char, unsigned short, unsigned int, unsigned int, cpl_boolean);

#endif /* MUSE_WAVECALIB_H */
