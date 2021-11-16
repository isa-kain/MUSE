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

#ifndef MUSE_WCS_H
#define MUSE_WCS_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>
#include <math.h>

#include "muse_cplwrappers.h"
#include "muse_datacube.h"
#include "muse_pixtable.h"
#include "muse_resampling.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
/** @addtogroup muse_wcs */
/**@{*/

/**
  @brief   regular expression for WCS properties

  Regular expression to copy/erase all keywords relevant to world coordinates
  this list is mostly taken from the FITS Standard v3.0 (Tab. 8.2, p. 77).
 */
#define MUSE_WCS_KEYS  "^C(TYPE|UNIT|RPIX|RVAL|DELT|SYER|RDER)|^CD[0-9]+_[0-9]+|" \
                       "^WCSAXES[ A-Z]|^WCSNAME[ A-Z]|^L[OA][NT]POLE$"
/**
  @brief   name for special astrometry detection image

  Extension name used to save the image that is used to detect the stars in the
  astrometric field.
 */
#define MUSE_WCS_DETIMAGE_EXTNAME "ASTROMETRY_DETECTION"
#define MUSE_WCS_SOURCE_TABLE_NAME "ASTROMETRY_SOURCES"

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/
extern const muse_cpltable_def muse_wcs_reference_def[];


/*----------------------------------------------------------------------------*/
/**
  @brief   WCS object to store data needed while computing the astrometric
           solution.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /* the datacube created for the exposure of the astrometric field */
  muse_datacube *cube;
  /* spatial center of the data in the pixel table */
  double xcenter, ycenter;
  /* right ascension and declination corresponding to the center of the data */
  double ra, dec;
  /* pixel values of the center of rotation */
  double crpix1, crpix2;
  /* table of detected sources */
  cpl_table *detected;
  /* the astrometric solution in the form of a FITS header */
  cpl_propertylist *wcs;
} muse_wcs_object;

/*----------------------------------------------------------------------------*/
/**
 * @brief Type of centroiding algorithm to use.
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_WCS_CENTROID_GAUSSIAN = 0, /**< Gaussian profile fit */
  MUSE_WCS_CENTROID_MOFFAT, /**< Moffat profile fit */
  MUSE_WCS_CENTROID_BOX /**< simplest version: marginal centroid in a box */
} muse_wcs_centroid_type;

/*----------------------------------------------------------------------------*/
/**
  @brief   A structure containing a spatial two-axis WCS.

  This structure is specially adapted to speed up coordinate transforms in the
  muse_wcs_*_fast() functions, it does not contain a full spatial WCS as
  cpl_wcs. But the latter is more complicated and time-consuming to handle.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  double crpix1, crpix2; /**< the reference point */
  double crval1, crval2; /**< the coordinate values at the reference point */
  double cd11, cd12, cd21, cd22; /**< the four elements of the CDi_j matrix */
  double cddet; /**< the deteminant of the CDi_j matrix */
  /** TRUE signifies an existing celestial spherical projection, *
   *  FALSE for other types                                      */
  cpl_boolean iscelsph;
} muse_wcs;

/*----------------------------------------------------------------------------*
 *                           Inline functions                                 *
 *----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/**
  @brief   Convert from pixel coordinates to celestial spherical coordinates.
  @param   aWCS   the input WCS structure
  @param   aX     the horizontal pixel position
  @param   aY     the vertical pixel position
  @param   aRA    the output right-ascension in degrees
  @param   aDEC   the output declination in degrees

  @warning This function does not do any safety checks. If you are not sure, if
           the WCS does not contain a proper gnomonic (TAN) setup, use
           muse_wcs_celestial_from_pixel() instead!
  @note This function is defined as static inline, i.e. should get integrated
        into the caller by the compiler, to maximise execution speed.

  See muse_wcs_celestial_from_pixel() for the origin of the equations.
 */
/*---------------------------------------------------------------------------*/
static inline void
muse_wcs_celestial_from_pixel_fast(muse_wcs *aWCS, double aX, double aY,
                                   double *aRA, double *aDEC)
{
  /* linear transformation */
  double x = aWCS->cd11 * (aX - aWCS->crpix1) + aWCS->cd12 * (aY - aWCS->crpix2),
         y = aWCS->cd22 * (aY - aWCS->crpix2) + aWCS->cd21 * (aX - aWCS->crpix1);
  /* spherical projection */
  double phi = atan2(x, -y),
         theta = atan(CPL_MATH_DEG_RAD / sqrt(x*x + y*y));
  /* spherical coordinate shift/translation */
  /* dec is delta_p in Paper II (in radians), aWCS->crval1 is alpha_p (in degrees) */
  double dec = aWCS->crval2 / CPL_MATH_DEG_RAD; /* DEC in radians */
  *aRA = aWCS->crval1 + atan2(cos(theta) * sin(phi),
                              sin(theta) * cos(dec) + cos(theta) * sin(dec) * cos(phi))
                      * CPL_MATH_DEG_RAD;
  *aDEC = asin(sin(theta) * sin(dec) - cos(theta) * cos(dec) * cos(phi))
        * CPL_MATH_DEG_RAD;
} /* muse_wcs_celestial_from_pixel_fast() */

/*---------------------------------------------------------------------------*/
/**
  @brief   Convert from celestial spherical coordinates to pixel coordinates.
  @param   aWCS   the input WCS structure,
                  <b>with the crpix components in radians!</b>
  @param   aRA    the right-ascension <b>in radians!</b>
  @param   aDEC   the declination <b>in radians!</b>
  @param   aX     the output horizontal pixel position
  @param   aY     the output vertical pixel position

  @warning This function does not do any safety checks. If you are not sure, if
           the WCS does not contain a proper gnomonic (TAN) setup, use
           muse_wcs_pixel_from_celestial() instead!
  @note This function is defined as static inline, i.e. should get integrated
        into the caller by the compiler, to maximise execution speed.

  See muse_wcs_pixel_from_celestial() for the origin of the equations.
 */
/*---------------------------------------------------------------------------*/
static inline void
muse_wcs_pixel_from_celestial_fast(muse_wcs *aWCS, double aRA, double aDEC,
                                   double *aX, double *aY)
{
  /* spherical coordinate shift/translation */
  /* aRA is alpha in Paper II, aDEC is delta, aWCS->crval1 is alpha_p, *
   * aWCS->crval2 is delta_p, all of them in units of radians          */
  double phi = atan2(-cos(aDEC) * sin(aRA - aWCS->crval1),
                     sin(aDEC) * cos(aWCS->crval2)
                     - cos(aDEC) * sin(aWCS->crval2) * cos(aRA - aWCS->crval1))
             + 180 / CPL_MATH_DEG_RAD,
         theta = asin(sin(aDEC) * sin(aWCS->crval2)
                      + cos(aDEC) * cos(aWCS->crval2) * cos(aRA - aWCS->crval1)),
         R_theta = CPL_MATH_DEG_RAD / tan(theta);
  /* spherical deprojection */
  double x = R_theta * sin(phi),
         y = -R_theta * cos(phi);
  /* inverse linear transformation */
  *aX = (aWCS->cd22 * x - aWCS->cd12 * y) / aWCS->cddet + aWCS->crpix1;
  *aY = (aWCS->cd11 * y - aWCS->cd21 * x) / aWCS->cddet + aWCS->crpix2;
} /* muse_wcs_pixel_from_celestial_fast() */

/*---------------------------------------------------------------------------*/
/**
  @brief   Convert from pixel coordinates to projection plane coordinates.
  @param   aWCS    the input WCS structure
  @param   aX      the horizontal pixel position
  @param   aY      the vertical pixel position
  @param   aXOut   the output x position
  @param   aYOut   the output y position

  @warning This function does not do any safety checks. If you are not sure, if
           the WCS does not contain a linear setup, use
           muse_wcs_projplane_from_pixel() instead!
  @note This function is defined as static inline, i.e. should get integrated
        into the caller by the compiler, to maximise execution speed.

  See muse_wcs_projplane_from_pixel() for the origin of the equations.
 */
/*---------------------------------------------------------------------------*/
static inline void
muse_wcs_projplane_from_pixel_fast(muse_wcs *aWCS, double aX, double aY,
                                   double *aXOut, double *aYOut)
{
  /* linear transformation */
  *aXOut = aWCS->cd11 * (aX - aWCS->crpix1) + aWCS->cd12 * (aY - aWCS->crpix2)
         + aWCS->crval1;
  *aYOut = aWCS->cd22 * (aY - aWCS->crpix2) + aWCS->cd21 * (aX - aWCS->crpix1)
         + aWCS->crval2;
} /* muse_wcs_projplane_from_pixel_fast() */

/*---------------------------------------------------------------------------*/
/**
  @brief   Convert from projection plane coordinates to pixel coordinates.
  @param   aWCS    the input WCS structure
  @param   aX      the x position
  @param   aY      the y position
  @param   aXOut   the output horizontal pixel position
  @param   aYOut   the output vertical pixel position

  @warning This function does not do any safety checks. If you are not sure, if
           the WCS does not contain a linear setup, use
           muse_wcs_pixel_from_projplane() instead!
  @note This function is defined as static inline, i.e. should get integrated
        into the caller by the compiler, to maximise execution speed.

  See muse_wcs_pixel_from_projplane() for the origin of the equations.
 */
/*---------------------------------------------------------------------------*/
static inline void
muse_wcs_pixel_from_projplane_fast(muse_wcs *aWCS, double aX, double aY,
                                   double *aXOut, double *aYOut)
{
  /* inverse linear transformation */
  *aXOut = (aWCS->cd22 * (aX - aWCS->crval1) - aWCS->cd12 * (aY - aWCS->crval2))
           / aWCS->cddet + aWCS->crpix1;
  *aYOut = (aWCS->cd11 * (aY - aWCS->crval2) - aWCS->cd21 * (aX - aWCS->crval1))
           / aWCS->cddet + aWCS->crpix2;
} /* muse_wcs_pixel_from_projplane_fast() */

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_wcs_object *muse_wcs_object_new(void);
void muse_wcs_object_delete(muse_wcs_object *);

cpl_table *muse_wcs_centroid_stars(muse_image *, float, muse_wcs_centroid_type);
cpl_error_code muse_wcs_locate_sources(muse_pixtable *, float, muse_wcs_centroid_type, muse_wcs_object *);
cpl_error_code muse_wcs_solve(muse_wcs_object *, cpl_table *, float, float, int, float);
cpl_error_code muse_wcs_optimize_solution(muse_wcs_object *, float, muse_wcs_centroid_type, cpl_table *, float, float, int, float);
cpl_propertylist *muse_wcs_create_default(const cpl_propertylist *);
cpl_propertylist *muse_wcs_apply_cd(const cpl_propertylist *, const cpl_propertylist *);
cpl_error_code muse_wcs_project_tan(muse_pixtable *, const cpl_propertylist *);
cpl_error_code muse_wcs_position_celestial(muse_pixtable *, double, double);

cpl_error_code muse_wcs_celestial_from_pixel(cpl_propertylist *, double, double, double *, double *);
cpl_error_code muse_wcs_pixel_from_celestial(cpl_propertylist *, double, double, double *, double *);
cpl_error_code muse_wcs_projplane_from_celestial(cpl_propertylist *, double, double, double *, double *);
cpl_error_code muse_wcs_projplane_from_pixel(cpl_propertylist *, double, double, double *, double *);
cpl_error_code muse_wcs_pixel_from_projplane(cpl_propertylist *, double, double, double *, double *);

cpl_error_code muse_wcs_get_angles(cpl_propertylist *, double *, double *);
cpl_error_code muse_wcs_get_scales(cpl_propertylist *, double *, double *);

muse_wcs *muse_wcs_new(cpl_propertylist *);

cpl_error_code muse_wcs_copy_keywords(const cpl_propertylist *, cpl_propertylist *, char, const char *);

#endif /* MUSE_WCS_H */
