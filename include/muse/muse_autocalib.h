/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2017 Simon Conseil (CRAL - Observatoire de Lyon)
 *           (C) 2017 European Southern Observatory
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

#ifndef MUSE_AUTOCALIB_H
#define MUSE_AUTOCALIB_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_image.h"
#include "muse_mask.h"
#include "muse_pixtable.h"
#include "muse_table.h"

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/
/** @addtogroup muse_autocalib */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
   @brief Type of autocalibration to use (if any)
*/
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_POSTPROC_AUTOCALIB_NONE = 0,
  MUSE_POSTPROC_AUTOCALIB_DEEPFIELD,
  MUSE_POSTPROC_AUTOCALIB_USER
} muse_autocalib_type;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
cpl_error_code muse_autocalib_slice_median(muse_pixtable *, double, muse_table **);
cpl_error_code muse_autocalib_apply(muse_pixtable *, muse_table *);
muse_mask *muse_autocalib_create_mask(muse_image *, double, const char *);

#endif /* MUSE_AUTOCALIB_H */
