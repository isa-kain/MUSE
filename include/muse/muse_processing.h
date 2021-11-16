/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2008-2016 European Southern Observatory
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

#ifndef MUSE_PROCESSING_H
#define MUSE_PROCESSING_H

#include <cpl.h>
#include "muse_image.h"
#include "muse_mask.h"
#include "muse_table.h"

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/
/** @addtogroup muse_processing */
/**@{*/

/** Counter for output frames, used to determine file names. */
typedef struct {
  const char *tag; /**< output tag */
  int ifu;         /**< IFU number */
  int counter;     /**< number of frames for this tag/IFU written so far */
} muse_processing_framecounter;

/** Structure to define processing parameters. */
typedef struct {
  const char *name;              /**< Recipe name. */
  cpl_array *intags;             /**< Tag(s) to select input frames. */
  cpl_recipe *recipe;            /**< Pointer to recipe */
  cpl_frameset *inframes;        /**< Frames available as input. */
  cpl_frameset *usedframes;      /**< Frames actually used as input. */
  cpl_frameset *outframes;       /**< Result (product) frames. */
  cpl_parameterlist *parameters; /**< Parameter list of the recipe. */
  muse_processing_framecounter *counter; /**< Counter for output frames. */
} muse_processing;

/** Frames modes used in the MUSE pipeline. */
typedef enum {
  MUSE_FRAME_MODE_ALL = 0, /**< any frame that is not further specified */
  MUSE_FRAME_MODE_MASTER = 1, /**< the one combined master frame of this tag */
  MUSE_FRAME_MODE_DATEOBS = 2, /**< one of the output frames of this type, *
                                *   sorted based on observing dates        */
  MUSE_FRAME_MODE_SUBSET = 3, /**< build header using a subset of the frames *
                               *   but keep an indexed filename              */
  MUSE_FRAME_MODE_SEQUENCE = 4 /**< sequence of master files, all based on *
                                *   all inputs                             */
} muse_frame_mode;

/** Table format types supported by the MUSE pipeline. */
typedef enum {
  MUSE_TABLE_TYPE_CPL = 0,  /**< Normal FITS tables handled by CPL */
  MUSE_TABLE_TYPE_PIXTABLE, /**< MUSE pixel table */
  MUSE_TABLE_TYPE_MUSE      /**< normal FITS tables, read with headers */
} muse_table_type;

/** Cube format types supported by the MUSE pipeline. */
typedef enum {
  MUSE_CUBE_TYPE_EURO3D = 0, /**< Euro3D format */
  MUSE_CUBE_TYPE_FITS,       /**< FITS NAXIS=3 format with 2 extensions */
  MUSE_CUBE_TYPE_EURO3D_X,   /**< Euro3D format with reconstructed image(s) *
                              *   in extra FITS extensions                  */
  MUSE_CUBE_TYPE_FITS_X,     /**< FITS NAXIS=3 format with 2 extensions plus *
                              *   reconstructed image(s) in extra extensions */
  MUSE_CUBE_TYPE_LSF,        /**< MUSE LSF cube */
  MUSE_CUBE_TYPE_SDP         /**< MUSE FITS cube with ESO SDP headers */
} muse_cube_type;

/**@}*/

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_processing *muse_processing_new(const char *, cpl_recipe *);
void muse_processing_delete(muse_processing *);

void muse_processing_append_used(muse_processing *, cpl_frame *, cpl_frame_group, int);

cpl_frame *muse_processing_new_frame(muse_processing *, int, cpl_propertylist *, const char *, cpl_frame_type);
int muse_processing_save_image(muse_processing *, int, muse_image *, const char *);
cpl_error_code muse_processing_save_cube(muse_processing *, int, void *, const char *, muse_cube_type);
int muse_processing_save_cimage(muse_processing *, int, cpl_image *, cpl_propertylist *, const char *);
cpl_table *muse_processing_load_ctable(muse_processing *, const char *, unsigned char);
muse_table *muse_processing_load_table(muse_processing *, const char *, unsigned char);
cpl_error_code muse_processing_save_table(muse_processing *, int, void *, cpl_propertylist *, const char *, muse_table_type);
muse_mask *muse_processing_load_mask(muse_processing *, const char *);
int muse_processing_save_mask(muse_processing *, int, muse_mask *, const char *);
cpl_propertylist *muse_processing_load_header(muse_processing *, const char *);
cpl_error_code muse_processing_save_header(muse_processing *, int, cpl_propertylist *, const char *);

cpl_boolean muse_processing_check_intags(muse_processing *, const char *, int);
cpl_error_code muse_processing_check_input(muse_processing *, unsigned char);

cpl_table *muse_processing_sort_exposures(muse_processing *);

/* ---------------------------------------------------------------------*/
/* The following prototypes are defined in the recipes and provide an
   interface to the formal description of the recipe additional to the
   plugin interface. The typedefs are used to ensure that the doc tool used
   the same prototypes as the  recipe.
*/
/* ---------------------------------------------------------------------*/
typedef cpl_error_code
muse_processing_prepare_header_func(const char *, cpl_propertylist *);

typedef cpl_frame_level
muse_processing_get_frame_level_func(const char *);

typedef muse_frame_mode
muse_processing_get_frame_mode_func(const char *);

void muse_processinginfo_register(cpl_recipe *, cpl_recipeconfig *, muse_processing_prepare_header_func *, muse_processing_get_frame_level_func *, muse_processing_get_frame_mode_func *);

cpl_error_code muse_processing_prepare_header(const cpl_recipe *, const char *, cpl_propertylist *);
cpl_error_code muse_processing_prepare_property(cpl_propertylist *, const char *, cpl_type, const char *);

cpl_frame_level muse_processing_get_frame_level(const cpl_recipe *, const char *);
int muse_processing_get_frame_mode(const cpl_recipe *, const char *);
cpl_recipeconfig *muse_processing_get_recipeconfig(cpl_recipe *);
void muse_processinginfo_delete(cpl_recipe *);

void muse_processing_recipeinfo(cpl_plugin *);

#endif /* MUSE_PROCESSING_H */
