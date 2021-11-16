/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2005-2017 European Southern Observatory
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

#ifndef MUSE_PIXGRID_H
#define MUSE_PIXGRID_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include "muse_pixtable.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/* use bits 0-52 for the value (the pixel table row), this *
 * allows to convert pixel tables with up to 9e15 pixels   *
 * (about 25 million MUSE exposures!) into a pixel grid    */
#define PT_IDX_MASK 0x1FFFFFFFFFFFFFll
/* use bits 53-62 to store the thread ID, this  *
 * allows parallelization with up to 1024 cores */
#define XMAP_BITMASK 0x3FFll /* 1023 */
#define XMAP_LSHIFT 53ll

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_pixgrid */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief   The pixel extension map.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  unsigned int npix; /*<< number of pixels in this grid point */
  cpl_size *pix;     /*<< the row number(s) in the pixel table */
} muse_pixels_ext;

/*----------------------------------------------------------------------------*/
/**
  @brief   The pixel grid.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  cpl_size *pix;           /*<< The pixel grid array, elements can be        *
                            *   0:        empty                              *
                            *   positive: row_number in the pixel table      *
                            *   negative: -(i_ext+1) in the extension array, *
                            *             bits 53-62 contain the map index   */
  cpl_size nx;             /*<< horizontal spatial size */
  cpl_size ny;             /*<< vertical spatial size */
  cpl_size nz;             /*<< size in dispersion direction */
  unsigned short nmaps;    /*<< number of extension maps */
  cpl_size *nxmap;         /*<< number of filled pixels in the extension maps */
  cpl_size *nxalloc;       /*<< number of allocated pixels in the extension maps */ // XXX needed?!?
  muse_pixels_ext **xmaps; /*<< the extension maps */
} muse_pixgrid;

/*----------------------------------------------------------------------------*
 *                           Inline functions                                 *
 *----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/**
  @brief   Get the grid index determined from all three coordinates.
  @param   aGrid           Pointer to pixel grid.
  @param   aX              x index
  @param   aY              y index
  @param   aZ              z index
  @param   aAllowOutside   if true, return a positive value for pixels nominally
                           outside the grid
  @return  Index to be used in the other pixel grid functions, or -1 on error.

  @note This function is defined as static inline, i.e. should get integrated
        into the caller by the compiler, to maximise execution speed.
*/
/*---------------------------------------------------------------------------*/
static inline cpl_size
muse_pixgrid_get_index(muse_pixgrid *aGrid, cpl_size aX, cpl_size aY,
                       cpl_size aZ, cpl_boolean aAllowOutside)
{
  if (!aAllowOutside &&
      (aX < 0 || aX >= aGrid->nx || aY < 0 || aY >= aGrid->ny ||
       aZ < 0 || aZ >= aGrid->nz)) {
    return -1;
  }
  if (aX < 0) {
    aX = 0;
  }
  if (aX >= aGrid->nx) {
    aX = aGrid->nx - 1;
  }
  if (aY < 0) {
    aY = 0;
  }
  if (aY >= aGrid->ny) {
    aY = aGrid->ny - 1;
  }
  if (aZ < 0) {
    aZ = 0;
  }
  if (aZ >= aGrid->nz) {
    aZ = aGrid->nz - 1;
  }
  return aX + aGrid->nx * (aY + aGrid->ny * aZ);
} /* muse_pixgrid_get_index() */

/*---------------------------------------------------------------------------*/
/**
  @brief   Return the number of rows stored in one pixel.
  @param   aGrid    Pointer to pixel grid.
  @param   aIndex   Pixel index, as computed by muse_pixgrid_get_index().

  @note This function is defined as static inline, i.e. should get integrated
        into the caller by the compiler, to maximise execution speed.
*/
/*---------------------------------------------------------------------------*/
static inline cpl_size
muse_pixgrid_get_count(muse_pixgrid *aGrid, cpl_size aIndex)
{
  if (aIndex < 0) {
    return 0;
  }
  /* get entry in pixel grid */
  cpl_size p = aGrid->pix[aIndex];
  if (p == 0) { /* points nowhere --> no pixels */
    return 0;
  }
  if (p > 0) { /* points to pixel table --> 1 pixel */
    return 1;
  }
  /* p is negative, so points to an extension map, get its index */
  unsigned short ix = (-p >> XMAP_LSHIFT) & XMAP_BITMASK;
  p = (-p - 1) & PT_IDX_MASK;
  /* the npix component now gives the number of pixels in this grid index */
  return aGrid->xmaps[ix][p].npix;
} /* muse_pixgrid_get_count() */

/*---------------------------------------------------------------------------*/
/**
  @brief   Return a pointer to the rows stored in one pixel.
  @param   aGrid    Pointer to pixel grid.
  @param   aIndex   Pixel index, as computed by muse_pixgrid_get_index().

  @note This function is defined as static inline, i.e. should get integrated
        into the caller by the compiler, to maximise execution speed.
*/
/*---------------------------------------------------------------------------*/
static inline const cpl_size *
muse_pixgrid_get_rows(muse_pixgrid *aGrid, cpl_size aIndex)
{
  if (aIndex < 0) {
    return 0;
  }
  /* get entry in pixel grid */
  cpl_size p = aGrid->pix[aIndex];
  if (p == 0) { /* points nowhere --> no array */
    return NULL;
  }
  if (p > 0) { /* points to pixel table */
    return aGrid->pix + aIndex;
  }
  /* p is negative, so points to an extension map, get its array */
  unsigned short ix = (-p >> XMAP_LSHIFT) & XMAP_BITMASK;
  p = (-p - 1) & PT_IDX_MASK;
  /* the pix component provides the array of pixel table rows in this index */
  return aGrid->xmaps[ix][p].pix;
} /* muse_pixgrid_get_rows() */

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_pixgrid *muse_pixgrid_create(muse_pixtable *, cpl_propertylist *, cpl_size, cpl_size, cpl_size);
muse_pixgrid *muse_pixgrid_2d_create(cpl_table *, double, double, double, double, float *);
void muse_pixgrid_delete(muse_pixgrid *);

#endif /* MUSE_PIXGRID_H */
