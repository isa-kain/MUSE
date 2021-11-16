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

#ifndef MUSE_ASTRO_H
#define MUSE_ASTRO_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
  @brief   Structure to store bary-, helio-, and geocentric velocity
           corrections.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  double bary; /**< barycentric correction */
  double helio; /**< heliocentric correction */
  double geo; /**< geocentric correction */
} muse_astro_rvcorr;

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
double muse_astro_compute_airmass(double, double, double, double, double);
double muse_astro_airmass(cpl_propertylist *);
double muse_astro_posangle(const cpl_propertylist *);
double muse_astro_parangle(const cpl_propertylist *);
double muse_astro_angular_distance(double, double, double, double);
double muse_astro_wavelength_vacuum_to_air(double);
muse_astro_rvcorr muse_astro_rvcorr_compute(const cpl_propertylist *);

#endif /* MUSE_ASTRO_H */
