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

#ifndef MUSE_QUADRANTS_H
#define MUSE_QUADRANTS_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_image.h"

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
cpl_error_code muse_quadrants_overscan_stats(muse_image *, const char *, unsigned int);
cpl_boolean muse_quadrants_overscan_check(muse_image *, muse_image *, double);
cpl_error_code muse_quadrants_overscan_correct(muse_image *, muse_image *);
cpl_error_code muse_quadrants_overscan_polyfit_vertical(muse_image *, unsigned, unsigned char, double, double, double);
cpl_size *muse_quadrants_overscan_get_window(const muse_image *, unsigned char, unsigned int);

muse_image *muse_quadrants_trim_image(muse_image *);
cpl_error_code muse_quadrants_coords_to_raw(cpl_propertylist *, int *, int *);
cpl_boolean muse_quadrants_verify(cpl_propertylist *);
cpl_size *muse_quadrants_get_window(const muse_image *, unsigned char);

#endif /* MUSE_QUADRANTS_H */
