/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2016 European Southern Observatory
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

#ifndef MUSE_TABLE_H
#define MUSE_TABLE_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

/*----------------------------------------------------------------------------*
 *                           Data structures                                  *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_table */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure to store a table together with a property list.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief    The table

    This component contain the table.
   */
  cpl_table *table;
  /**
    @brief    the header

    This component contains the properties that belong to the table data.
    These represent the primary header of the corresponding FITS file.
   */
  cpl_propertylist *header;
} muse_table;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_table *muse_table_new(void);
void muse_table_delete(muse_table *);
muse_table *muse_table_load(const char *, unsigned char);
cpl_error_code muse_table_save(muse_table *, const char *);

#endif /* MUSE_TABLE_H */
