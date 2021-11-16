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

#ifndef MUSE_LSF_H
#define MUSE_LSF_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_cplwrappers.h"
#include "muse_pixtable.h"
#include "muse_processing.h"
#include "muse_wcs.h"

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/
/** @addtogroup muse_lsf */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief Data cube/stacked image list containing the LSF for one IFU.

  x direction of each image is the pixel wavelength, y direction is the arc line
  wavelength.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /** @brief Primary header used to save the LSF cube to disk */
  cpl_propertylist *header; /* this has to be the first component! */
  /** @brief Stacked image list for LSF; one per slice */
  cpl_imagelist *img;
  /** @brief Common WCS information for each slice */
  muse_wcs *wcs;
} muse_lsf_cube;

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_pixtable *muse_lsf_create_arcpixtable(muse_imagelist *, cpl_table *, cpl_table *, cpl_table *, int, double);
cpl_error_code muse_lsf_fit_slice(const muse_pixtable *, cpl_image *, muse_wcs *, double);

muse_lsf_cube *muse_lsf_cube_new(double, cpl_size, cpl_size, const cpl_propertylist *);
void muse_lsf_cube_delete(muse_lsf_cube *);
cpl_error_code muse_lsf_cube_save(muse_lsf_cube *, const char *);
muse_lsf_cube *muse_lsf_cube_load(const char *, unsigned char);
muse_lsf_cube **muse_lsf_cube_load_all(muse_processing *aProcessing);
void muse_lsf_cube_delete_all(muse_lsf_cube **lsf);

cpl_error_code muse_lsf_apply(const cpl_image *, const muse_wcs *, cpl_array *, double);
cpl_error_code muse_lsf_apply_slice(muse_pixtable *, double, double aFlux,
                                    cpl_image *, muse_wcs *);
cpl_image *muse_lsf_average_cube_all(muse_lsf_cube **, muse_pixtable *);
muse_wcs *muse_lsf_cube_get_wcs_all(muse_lsf_cube **);
cpl_error_code muse_lsf_fold_rectangle(cpl_image *, const muse_wcs *, double);

/**@}*/

#endif /* MUSE_LSF_H */
