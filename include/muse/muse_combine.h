/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2005-2013 European Southern Observatory
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

#ifndef MUSE_COMBINE_H
#define MUSE_COMBINE_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_dfs.h"
#include "muse_image.h"
#include "muse_imagelist.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/
/* combination methods,                                     *
 * keep in sync with kCombinationStrings in muse_combine.c! */
enum combinationMethods {
  MUSE_COMBINE_SUM,
  MUSE_COMBINE_AVERAGE,
  MUSE_COMBINE_MEDIAN,
  MUSE_COMBINE_MINMAX,
  MUSE_COMBINE_SIGCLIP,
  MUSE_COMBINE_NONE, /* no combination, for muse_scibasic */
  MUSE_COMBINE_UNKNOWN
};

typedef struct {
  enum combinationMethods combine; /**< image combination method */
  int nLow;
  int nHigh;
  int nKeep;
  double lSigma;
  double hSigma;
  cpl_boolean scale; /**< scale relative to expsure time of first image? */
} muse_combinepar;

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_image *muse_combine_sum_create(muse_imagelist *);
muse_image *muse_combine_average_create(muse_imagelist *);
muse_image *muse_combine_median_create(muse_imagelist *);
muse_image *muse_combine_minmax_create(muse_imagelist *, int, int, int);
muse_image *muse_combine_sigclip_create(muse_imagelist *, double, double);

muse_combinepar *muse_combinepar_new(cpl_parameterlist *, const char *);
void muse_combinepar_delete(muse_combinepar *);
muse_image *muse_combine_images(muse_combinepar *, muse_imagelist *);

#endif /* MUSE_COMBINE_H */
