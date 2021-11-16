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

#ifndef MUSE_TRACING_H
#define MUSE_TRACING_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_image.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *                        parameters for tracing                              *
 *----------------------------------------------------------------------------*/
#define TRACE_BINSIZE 55  /* half length of the bin in which to locate the midpoint */

#define MUSE_TRACE_NPOLY 3 /* number of polynomial solutions *
                            * (center, left, and right)      */
enum muse_trace_poly {
  MUSE_TRACE_CENTER = 0, /**< center of slice */
  MUSE_TRACE_LEFT,       /**< left edge of slice */
  MUSE_TRACE_RIGHT       /**< right edge of slice */
};

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
cpl_vector *muse_trace_locate_slices(cpl_vector *, const unsigned short, double, const unsigned char);
double muse_trace_edgefinder(const cpl_vector *, double, double *, double *, cpl_boolean *, const unsigned char);
cpl_table *muse_trace(const muse_image *, int, double, int, cpl_table **);
int muse_trace_table_get_order(const cpl_table *);
cpl_polynomial **muse_trace_table_get_polys_for_slice(const cpl_table *, const unsigned short);
void muse_trace_polys_delete(cpl_polynomial **);

cpl_error_code muse_trace_plot_samples(cpl_table *, cpl_table *, unsigned short, unsigned short, unsigned char, muse_image *);
cpl_error_code muse_trace_plot_widths(cpl_table *, unsigned short, unsigned short, unsigned char);

#endif /* MUSE_TRACING_H */
