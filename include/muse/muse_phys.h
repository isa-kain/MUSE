/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2015 European Southern Observatory
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

#ifndef MUSE_PHYS_H
#define MUSE_PHYS_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_pixtable.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
#define MUSE_PHYS_hPa_TO_mmHg 0.75006158 /* conversion from hPa (or mbar)  *
                                          * to mmHg, needed for Filippenko */

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_phys */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief   Flags with special bit setup to pass to muse_phys_air_to_vacuum().

  The 2nd bit determines the air parameter determination, higher bits the method
  to use for conversion. Use (flags & 0xC) to access the methods.
 */
/*----------------------------------------------------------------------------*/
enum muse_phys_flags {
  /* either standard or measured air parameters, force the 1 in *
   * the first bit, so that one /has/ to make a choice for this */
  MUSE_PHYS_AIR_STANDARD = 1, /**< use standard air parameters */
  MUSE_PHYS_AIR_MEASURED = 3, /**< use measured air parameters */

  /* the method to convert it, bits 3 and 4 */
  MUSE_PHYS_METHOD_CIDDOR =          0, /**< use Ciddor formulae for conversion */
  MUSE_PHYS_METHOD_OWENS =      1 << 2, /**< use Owens formulae for conversion */
  MUSE_PHYS_METHOD_EDLEN =      2 << 2, /**< use Edlen formulae for conversion */
  MUSE_PHYS_METHOD_FILIPPENKO = 3 << 2, /**< use Filippenko formulae for conversion */
};

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
double muse_phys_nrindex_owens_saturation_pressure(double temp);
void muse_phys_nrindex_owens_coeffs(double, double, double, double *, double *);
double muse_phys_nrindex_owens(double, double, double);
double muse_phys_nrindex_edlen(double, double, double, double);
double muse_phys_nrindex_ciddor(double, double, double, double, double);
double muse_phys_nrindex_filippenko(double, double, double, double);

cpl_error_code muse_phys_air_to_vacuum(muse_pixtable *, unsigned int);

#endif /* MUSE_PHYS_H */
