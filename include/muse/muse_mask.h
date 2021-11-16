/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2013 European Southern Observatory
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef MUSE_MASK_H
#define MUSE_MASK_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

/** @addtogroup muse_mask */
/**@{*/

/*----------------------------------------------------------------------------*
 *                           Data structures                                  *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Handling of "mask" files.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief    The mask data

    This extension contain the mask data.
   */
  cpl_mask *mask;
  /**
    @brief    the FITS header

    This contains the FITS keywords that are read from or written to the
    output file.
   */
  cpl_propertylist *header;
} muse_mask;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_mask *muse_mask_new(void);
muse_mask *muse_mask_load(const char *);
cpl_error_code muse_mask_save(muse_mask *, const char *);
void muse_mask_delete(muse_mask *);

#endif /* MUSE_MASK_H */
