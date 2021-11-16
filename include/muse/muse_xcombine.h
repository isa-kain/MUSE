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

#ifndef MUSE_XCOMBINE_H
#define MUSE_XCOMBINE_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_pixtable.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_xcombine */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Xposure combination types
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  /** weight exposures by exposure time */
  MUSE_XCOMBINE_EXPTIME = 0,
  /** weight exposures by fwhm (and exposure time) */
  MUSE_XCOMBINE_FWHM    = 1,
  /** weight exposures by given weight in header (DRS.MUSE.WEIGHT, and exposure time) */
  MUSE_XCOMBINE_HEADER,
  /** do not weight exposures at all */
  MUSE_XCOMBINE_NONE
} muse_xcombine_types;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
double *muse_xcombine_find_offsets(const cpl_table *, const char *);

cpl_error_code muse_xcombine_weights(muse_pixtable **, muse_xcombine_types);
muse_pixtable *muse_xcombine_tables(muse_pixtable **, const cpl_table *);

#endif /* MUSE_XCOMBINE_H */
