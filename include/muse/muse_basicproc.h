/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2008-2014 European Southern Observatory
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

#ifndef MUSE_BASICPROC_H
#define MUSE_BASICPROC_H

#include <cpl.h>

#include "muse_artifacts.h"
#include "muse_datacube.h"
#include "muse_imagelist.h"
#include "muse_processing.h"
#include "muse_pixtable.h"

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_basicproc */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure of basic processing parameters
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  char *overscan; /**< overscan handling method */
  char *rejection; /**< rejection method (and possibly parameters) */
  double ovscsigma; /**< sigma deviation above which the pipeline aborts */
  unsigned int ovscignore; /**< edge pixels to ignore in the overscans */

  /** image-based cosmic ray rejection method to use */
  char *crmethod;
  unsigned dcrxbox; /**< DCR-like: search box size in x */
  unsigned dcrybox; /**< DCR-like: search box size in y */
  unsigned dcrpasses; /**< DCR-like: maximum number of cleaning passes */
  /** DCR-like: detection threshold in factors of standard deviation */
  float dcrthres;

  cpl_boolean keepflat; /**< flag to store the master-flat in "flatimage" */
  muse_image *flatimage; /**< master-flat field image that was applied */
} muse_basicproc_params;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_basicproc_params *muse_basicproc_params_new(cpl_parameterlist *, const char *);
muse_basicproc_params *muse_basicproc_params_new_from_propertylist(const cpl_propertylist *);
void muse_basicproc_params_delete(muse_basicproc_params *);

muse_imagelist *muse_basicproc_load(muse_processing *, unsigned char, muse_basicproc_params *);
muse_imagelist *muse_basicproc_load_reduced(muse_processing *, unsigned char);

cpl_error_code muse_basicproc_darkmodel(muse_image *);

cpl_table *muse_basicproc_get_illum(muse_imagelist *, cpl_table *, cpl_table *, cpl_table *);
cpl_error_code muse_basicproc_apply_illum(muse_pixtable *, cpl_table *);
cpl_error_code muse_basicproc_apply_twilight(muse_pixtable *, muse_datacube *);
cpl_error_code muse_basicproc_mask_notch_filter(muse_pixtable *, unsigned char);
muse_imagelist *muse_basicproc_combine_images_lampwise(muse_processing *, unsigned char, muse_basicproc_params *, cpl_frameset ***);
cpl_error_code muse_basicproc_shift_pixtable(muse_pixtable *, cpl_array *, double, double, float, float, unsigned char);
cpl_error_code muse_basicproc_stats_append_header(cpl_image *, cpl_propertylist *, const char *, unsigned);
cpl_error_code muse_basicproc_stats_append_header_window(cpl_image *, cpl_propertylist *, const char *, unsigned, int, int, int, int);

cpl_error_code muse_basicproc_qc_saturated(muse_image *, const char *);

cpl_error_code muse_basicproc_process_rtcdata(muse_imagelist *);

#endif /* MUSE_BASICPROC_H */
