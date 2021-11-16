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
#ifndef MUSE_QUALITY_H
#define MUSE_QUALITY_H

/*---------------------------------------------------------------------------*
 *                             Includes                                      *
 *---------------------------------------------------------------------------*/
#include <cpl.h>
#include "muse_image.h"

#include "muse_cplwrappers.h"

/*---------------------------------------------------------------------------*
 *                             Defines                                       *
 *---------------------------------------------------------------------------*/

/** @addtogroup muse_quality */
/**@{*/

/* Euro3D bad pixel states (be sure to use unsigned numbers) */
#define EURO3D_GOODPIXEL 0
#define EURO3D_TELLURIC   (1ul <<  0) /* uncorrected */
#define EURO3D_TELLCOR    (1ul <<  1) /* corrected */
#define EURO3D_COSMICRAY  (1ul <<  5) /* uncorrected */
#define EURO3D_LOWQEPIXEL (1ul <<  6)
#define EURO3D_HOTPIXEL   (1ul <<  8)
#define EURO3D_DARKPIXEL  (1ul <<  9)
#define EURO3D_SATURATED  (1ul << 12)
#define EURO3D_DEADPIXEL  (1ul << 13)
#define EURO3D_BADOTHER   (1ul << 14) /* other kind of bad pixel, used e.g. */
                                      /* for non-positive pixels in flats   */
#define EURO3D_OBJECT     (1ul << 25) /* for object masks, currently unused */
#define EURO3D_OUTLIER    (1ul << 26) /* for general outliers, see          *
                                       * muse_resampling_spectrum_iterate() */
#define EURO3D_NOTCH_NAD  (1ul << 27) /* for the NaD range of a notch filter */
#define EURO3D_MUSE_TRAP  (1ul << 29) /* currently unused */
#define EURO3D_MISSDATA   (1ul << 30)
#define EURO3D_OUTSDRANGE (1ul << 31)
#define EURO3D_ALLBITS 0xFFFFul

extern const muse_cpltable_def muse_badpix_table_def[];

/*----------------------------------------------------------------------------*
 *                           Inline functions                                 *
 *----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/**
  @brief    Check for usable pixel quality given a bitmask of usable states.
  @param    aState     pixel flags to compare
  @param    aInclude   bitmask of allowed bad pixel flags (in addition to
                       EURO3D_GOODPIXEL)
  @return   CPL_TRUE or CPL_FALSE
 */
/*---------------------------------------------------------------------------*/
static inline cpl_boolean
muse_quality_is_usable(uint32_t aState, uint32_t aInclude)
{
  return (aState == EURO3D_GOODPIXEL) ||
         !(aState ^ aInclude);
} /* muse_quality_is_usable() */

/**@}*/

/*---------------------------------------------------------------------------*
 *                          Function prototypes                              *
 *---------------------------------------------------------------------------*/
int muse_quality_dark_badpix(muse_image *, double, double);
int muse_quality_dark_refine_badpix(muse_image *, double, unsigned short);
int muse_quality_bad_columns(muse_image *, double, double);
int muse_quality_flat_badpix(muse_image *, cpl_table *, double, double);
int muse_quality_set_saturated(muse_image *);
cpl_table *muse_quality_convert_dq(cpl_image *);
cpl_error_code muse_quality_merge_badpix(cpl_table *, const cpl_table *);
int muse_quality_image_reject_using_dq(cpl_image *, cpl_image *, cpl_image *);

cpl_table *muse_quality_merge_badpix_from_file(const cpl_table *, const char *, const char *, int *);
cpl_error_code muse_quality_copy_badpix_table(const char *, const char *, int, const cpl_table *);

#endif /* MUSE_QUALITY_H */
