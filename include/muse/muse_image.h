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

#ifndef MUSE_IMAGE_H
#define MUSE_IMAGE_H

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_image */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of MUSE three extension FITS file

  Type to store MUSE typical three extension FITS files for the basic data
  reduction before converting everything to Euro3D structures.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief    the data extension

    This extension contain the real data.
   */
  cpl_image *data;

  /**
    @brief    the data quality extension

    This represents the data quality spectrum of the Euro3D format.
    Effectively, it is a 32bit bad pixel mask where every value has a
    different meaning (0: good pixel, ..., 15: unknown type of bad pixel,
    16-29: undefined, 30: missing data, 31: outside data range).
   */
  cpl_image *dq;

  /**
    @brief    the statistics extension

    This represents a variance frame, and stores error estimates of the
    data extension as sigma^2.
   */
  cpl_image *stat;

  /**
    @brief    the FITS header

    This contains the FITS keywords that are read from or written to the
    output file.
   */
  cpl_propertylist *header;
} muse_image;

/**@}*/

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
/* extension names for the three images */
#define EXTNAME_DATA "DATA"
#define EXTNAME_DQ "DQ"
#define EXTNAME_STAT "STAT"
#define EXTNAME_DATA_COMMENT "This extension contains data values"
#define EXTNAME_DQ_COMMENT "This extension contains bad pixel status values"
#define EXTNAME_STAT_COMMENT "This extension contains data variance"

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_image *muse_image_new(void);
void muse_image_delete(muse_image *);
muse_image *muse_image_load(const char *);
muse_image *muse_image_load_from_extensions(const char *, unsigned char);
muse_image *muse_image_load_from_raw(const char *, int);
cpl_error_code muse_image_save(muse_image *, const char *);
muse_image *muse_image_duplicate(const muse_image *);
int muse_image_subtract(muse_image *, muse_image *);
int muse_image_divide(muse_image *, muse_image *);
int muse_image_scale(muse_image *, double);

int muse_image_variance_create(muse_image *, muse_image *);
cpl_error_code muse_image_adu_to_count(muse_image *);

cpl_error_code muse_image_reject_from_dq(muse_image *);
cpl_error_code muse_image_dq_to_nan(muse_image *);

cpl_mask *muse_image_create_border_mask(const muse_image *, unsigned int);
cpl_mask *muse_image_create_corner_mask(const muse_image *, unsigned char, float);

#endif /* MUSE_IMAGE_H */
