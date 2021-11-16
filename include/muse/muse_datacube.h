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

#ifndef MUSE_DATACUBE_H
#define MUSE_DATACUBE_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_cplwrappers.h"
#include "muse_image.h"
#include "muse_imagelist.h"
#include "muse_table.h"

/** @addtogroup muse_resampling */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of a MUSE datacube

  Type to store a datacube of MUSE data: three cubes for data, variance, and
  bad pixel status, as well as the main FITS headers, and data for reconstructed
  images.
  The cpl_imagelist type is used to represent each individual datacube.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief    the FITS header

    This contains the FITS keywords carried over from the raw exposure(s)
    and all keywords added during processing. Specifically, this contains
    the WCS of the final exposure (that also gets duplicated into the
    extensions when saving the cube).
   */
  cpl_propertylist *header; /* this has to be the first component! */

  /**
    @brief    the reconstructed image data

    This contains the reconstructed images.
   */
  muse_imagelist *recimages; /* this has to be the second component! */

  /**
    @brief    the reconstructed image filter names

    This contains the filter names of all reconstructed images.
   */
  cpl_array *recnames; /* this has to be the third component! */

  /**
    @brief   the cube containing the actual data values
   */
  cpl_imagelist *data;

  /**
    @brief   the optional cube containing the bad pixel status
   */
  cpl_imagelist *dq;

  /**
    @brief   the cube containing the data variance
   */
  cpl_imagelist *stat;
} muse_datacube;

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of a Euro3D datacube

  Type to store a datacube in Euro3D format: two tables plus FITS headers, as
  well as the main FITS headers, and data for reconstructed images.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief    the primary FITS header

    This contains the FITS keywords carried over from the raw exposure(s)
    and all keywords added during processing.
   */
  cpl_propertylist *header; /* this has to be the first component! */

  /**
    @brief    the reconstructed image data

    This contains the reconstructed images.
   */
  muse_imagelist *recimages; /* this has to be the second component! */

  /**
    @brief    the reconstructed image filter names

    This contains the filter names of all reconstructed images.
   */
  cpl_array *recnames; /* this has to be the third component! */

  /**
    @brief   the table containing the actual Euro3D data
   */
  cpl_table *dtable;

  /**
    @brief    the data FITS header

    This contains the FITS keywords related the data in the Euro3D format,
    specifically the spectral WCS.
   */
  cpl_propertylist *hdata;

  /**
    @brief   the table containing the Euro3D groups
   */
  cpl_table *gtable;

  /**
    @brief    the group FITS header

    This contains the FITS keywords related the groups in the Euro3D format,
   */
  cpl_propertylist *hgroup;
} muse_euro3dcube;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
cpl_error_code muse_euro3dcube_save(muse_euro3dcube *, const char *);
void muse_euro3dcube_delete(muse_euro3dcube *);
cpl_error_code muse_datacube_convert_dq(muse_datacube *);
muse_datacube *muse_datacube_load(const char *);
cpl_error_code muse_datacube_concat(muse_datacube *, const muse_datacube *);
cpl_error_code muse_datacube_save(muse_datacube *, const char *);
cpl_error_code muse_datacube_save_recimages(const char *, muse_imagelist *, cpl_array *);
void muse_datacube_delete(muse_datacube *);

muse_image *muse_euro3dcube_collapse(muse_euro3dcube *, const muse_table *);
muse_image *muse_datacube_collapse(muse_datacube *, const muse_table *);

#endif /* MUSE_DATACUBE_H */
