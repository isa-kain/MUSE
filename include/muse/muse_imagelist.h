/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2008-2012 European Southern Observatory
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

#ifndef MUSE_IMAGELIST_H
#define MUSE_IMAGELIST_H

#include <cpl.h>
#include "muse_artifacts.h"
#include "muse_image.h"

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_imagelist */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure definition for a collection of muse_images.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief    The list of muse_images.

    This is the array of the 3-extension + header images that make up this list.
   */
  muse_image **list;

  /** The number of muse_images in this list. */
  unsigned int size;
} muse_imagelist;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_imagelist *muse_imagelist_new(void);
void muse_imagelist_delete(muse_imagelist *);
muse_image *muse_imagelist_get(muse_imagelist *, unsigned int);
cpl_error_code muse_imagelist_set(muse_imagelist *, muse_image *, unsigned int);
muse_image *muse_imagelist_unset(muse_imagelist *, unsigned int);
int muse_imagelist_is_uniform(muse_imagelist *);
unsigned int muse_imagelist_get_size(muse_imagelist *);
void muse_imagelist_dump_statistics(muse_imagelist *);
cpl_error_code muse_imagelist_scale_exptime(muse_imagelist *);
cpl_bivector *muse_imagelist_compute_ron(muse_imagelist *, int, int);

#endif /* MUSE_IMAGELIST_H */
