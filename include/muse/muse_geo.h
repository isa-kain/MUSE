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

#ifndef MUSE_GEO_H
#define MUSE_GEO_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_cplwrappers.h"
#include "muse_image.h"
#include "muse_imagelist.h"

/** @addtogroup muse_geo */
/**@{*/

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
/* This lists some constants that are not really hard instrument properties *
 * (and hence do not belong into muse_instrument.h) but where set up by     *
 * experience with a few (full) geometry datasets sind spring of 2013.      */

/* define some standard gap sizes to fall back onto; these numbers *
 * were derived as the peaks of the histograms of the valid        *
 * (positive) gap sizes of the datasets 2013-06 and 2013-08        */
static const double kMuseGeoMiddleGap = 0.04, /* [pix] for central gap */
                    kMuseGeoOuterGap = 0.07; /* [pix] for left and right gaps */
/* approximate vertical size of an IFU, guessed from real data */
static const double kMuseGeoIFUVGap = 1.5780; /* [mm] */

/* maximum slice angle to expect in the creation of the geometry   *
 * table (the real angles should be much smaller, but angles above *
 * this absolute value will definitely be wrong )                  */
static const double kMuseGeoSliceMaxAngle = 5.; /* [deg] */

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/
extern const muse_cpltable_def muse_geo_measurements_def[];
extern const muse_cpltable_def muse_geo_table_def[];

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition of MUSE geometry table

  Type to propagate MUSE geometry table information through the geometrical
  calibration.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief The @ref muse_geo_table_def "geometry table".

    @copydetails muse_geo_table_def
   */
  cpl_table *table;

  /**
    @brief The VLT focal plane scale factor of the data.
    output file.
   */
  double scale;
} muse_geo_table;

/*----------------------------------------------------------------------------*/
/**
 * @brief Type of centroiding algorithm to use.
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_GEO_CENTROID_BARYCENTER = 0, /**< barycenter centroiding */
  MUSE_GEO_CENTROID_GAUSSIAN /**< Gaussian profile fit */
} muse_geo_centroid_type;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
cpl_table *muse_geo_table_extract_ifu(const cpl_table *, const unsigned char);
double muse_geo_table_ifu_area(const cpl_table *, const unsigned char, double);

cpl_vector *muse_geo_lines_get(const cpl_table *);
cpl_table *muse_geo_measure_spots(muse_image *, muse_imagelist *, const cpl_table *, const cpl_table *, const cpl_vector *, double, muse_geo_centroid_type);

cpl_error_code muse_geo_compute_pinhole_local_distance(cpl_array *, cpl_table *);
double muse_geo_compute_pinhole_global_distance(cpl_array *, double, double, double);

muse_geo_table *muse_geo_table_new(cpl_size, double);
muse_geo_table *muse_geo_table_duplicate(const muse_geo_table *);
void muse_geo_table_delete(muse_geo_table *);

muse_geo_table *muse_geo_determine_initial(cpl_table *, const cpl_table *);
muse_geo_table *muse_geo_determine_horizontal(const muse_geo_table *);
cpl_error_code muse_geo_refine_horizontal(muse_geo_table *, cpl_table *);
muse_geo_table *muse_geo_determine_vertical(const muse_geo_table *);
cpl_error_code muse_geo_finalize(muse_geo_table *);
cpl_error_code muse_geo_correct_slices(muse_geo_table *, cpl_propertylist *, double);

cpl_error_code muse_geo_qc_global(const muse_geo_table *, cpl_propertylist *);

#endif /* MUSE_GEO_H */
