/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2005-2018 European Southern Observatory
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

#ifndef MUSE_UTILS_H
#define MUSE_UTILS_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>
#include <math.h> /* isnormal, isfinite */

#include "muse_dfs.h"
#include "muse_image.h"
#include "muse_pixtable.h"
#include "muse_processing.h"

/* make compilation fail when FILENAME_MAX is not defined, as that is used *
 * in various parts of the code to allocated char* for path/filenames      */
#ifndef FILENAME_MAX
#  error FILENAME_MAX undefined!
#endif

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
/* Two macros that can be used to work around compiler warnings of the type    *
 * "warning: unused parameter 'arg'" that occur in functions that are not      *
 * yet fully implemented or in test programs that don't really need arguments. *
 * This trick was taken from SQLite where it was tested on a large variety of  *
 * compilers.                                                                  */
#define UNUSED_ARGUMENT(arg) (void)(arg)
#define UNUSED_ARGUMENTS(arg1,arg2) UNUSED_ARGUMENT(arg1),UNUSED_ARGUMENT(arg2);

/* work around missing isfinite() */
#if !HAVE_ISFINITE
#if HAVE_ISNAN && HAVE_ISINF
#define isfinite(x) (!isnan(x) && !isinf(x))
#else /* last resort, primitive own implementation */
#define isfinite(x) ((x != nan) && (x != -nan) && (x != inf) && (x != -inf))
#endif
#endif

/* work around missing isnormal() */
#if !HAVE_ISNORMAL
#if HAVE_ISNAN && HAVE_ISINF
#define isnormal(x) (!isnan(x) && !isinf(x) && (x != 0.))
#else /* last resort, primitive own implementation */
#define isnormal(x) ((x != nan) && (x != -nan) && (x != inf) && (x != -inf) \
                     && (x != 0.) && (x != -0.))
#endif
#endif

/* work around missing drand48() */
#if !HAVE_DRAND48
/* make a drand48() replacement using rand() which is already in C89 */
#define drand48() ((double)(rand()) / RAND_MAX)
#endif

/** @addtogroup muse_utils */
/**@{*/

/* MUSE specific error codes */
/* failures to load any muse_image extension */
#define MUSE_ERROR_READ_DATA             CPL_ERROR_EOL+1
#define MUSE_ERROR_READ_DQ               CPL_ERROR_EOL+2
#define MUSE_ERROR_READ_STAT             CPL_ERROR_EOL+3
/* tracing errors */
#define MUSE_ERROR_SLICE_LEFT_MISSING    CPL_ERROR_EOL+4
#define MUSE_ERROR_SLICE_RIGHT_MISSING   CPL_ERROR_EOL+5
#define MUSE_ERROR_SLICE_EDGE_MISSING    CPL_ERROR_EOL+6
/* detector aliveness check failed after raw data couldn't be read */
#define MUSE_ERROR_CHIP_NOT_LIVE         CPL_ERROR_EOL+10

/* regular expression to delete ESO format keywords */
#define ESO_HDU_HEADERS_REGEXP "HDU(CLASS|CLAS1|CLAS2|CLAS3|DOC|VERS)$" \
                               "|^SCIDATA$|^QUAL(DATA|MASK)$|^ERRDATA$"

/*----------------------------------------------------------------------------*
 *                           Data structures                                  *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Background handling when computing centroids
 */
/*----------------------------------------------------------------------------*/
typedef enum {
   MUSE_UTILS_CENTROID_NORMAL = 0, /**< no background treatment */
   MUSE_UTILS_CENTROID_MEAN,       /**< subtract average as background */
   MUSE_UTILS_CENTROID_MEDIAN      /**< subtract median value as background */
} muse_utils_centroid_type;

/*----------------------------------------------------------------------------*/
/**
 * @brief Type of spectrum smoothing to use (e.g. for the response curve)
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_SPECTRUM_SMOOTH_NONE = 0, /**< no smoothing at all! */
  MUSE_SPECTRUM_SMOOTH_MEDIAN, /**< sliding median of +/- 15 Angstrom width */
  MUSE_SPECTRUM_SMOOTH_PPOLY /**< piecewise cubic polynomial plus *
                              *   median of +/- 15 Angstrom width */
} muse_spectrum_smooth_type;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
const char *muse_get_license(void);

unsigned char muse_utils_get_ifu(const cpl_propertylist *);
int muse_utils_get_extension_for_ifu(const char *, unsigned char);

cpl_frameset *muse_frameset_find(const cpl_frameset *, const char *, unsigned char, cpl_boolean);
cpl_frameset *muse_frameset_find_tags(const cpl_frameset *, const cpl_array *, unsigned char, cpl_boolean);
cpl_frameset *muse_frameset_check_raw(const cpl_frameset *, const cpl_array *, unsigned char);
cpl_frameset *muse_frameset_sort_raw_other(const cpl_frameset *, int, const char *, cpl_boolean);
cpl_frame *muse_frameset_find_master(const cpl_frameset *, const char *, unsigned char);
cpl_error_code muse_utils_frameset_merge_frames(cpl_frameset *, cpl_boolean);

muse_table *muse_table_load_filter(muse_processing *, const char *);
double muse_utils_filter_fraction(const muse_table *, double, double);
cpl_error_code muse_utils_filter_copy_properties(cpl_propertylist *, const muse_table *, double);

char *muse_utils_header_get_lamp_names(cpl_propertylist *, char);
cpl_array *muse_utils_header_get_lamp_numbers(cpl_propertylist *);
cpl_matrix *muse_matrix_new_gaussian_2d(int, int, double);
cpl_image *muse_utils_image_fit_polynomial(const cpl_image *, unsigned short, unsigned short);
cpl_error_code muse_utils_image_get_centroid_window(cpl_image *, int, int, int, int, double *, double *, muse_utils_centroid_type);
cpl_error_code muse_utils_get_centroid(const cpl_matrix *, const cpl_vector *, const cpl_vector *, double *, double *, muse_utils_centroid_type);
cpl_error_code muse_utils_fit_multigauss_1d(const cpl_vector *, const cpl_bivector *, cpl_vector *, double *, cpl_vector *, cpl_vector *, double *, double *, cpl_matrix **);
cpl_error_code muse_utils_fit_moffat_2d(const cpl_matrix *, const cpl_vector *, const cpl_vector *, cpl_array *, cpl_array *, const cpl_array *, double *, double *);
cpl_polynomial *muse_utils_iterate_fit_polynomial(cpl_matrix *, cpl_vector *, cpl_vector *, cpl_table *, const unsigned int, const double, double *, double *);
double muse_utils_pixtable_fit_line_gaussian(muse_pixtable *, double, double, double, float, float, unsigned char, cpl_array *, cpl_array *);
cpl_error_code muse_utils_spectrum_smooth(muse_table *, muse_spectrum_smooth_type);

cpl_error_code muse_utils_copy_modified_header(cpl_propertylist *, cpl_propertylist *, const char *, const char *);
cpl_error_code muse_utils_set_hduclass(cpl_propertylist *, const char *, const char *, const char *, const char *);

void muse_utils_memory_dump(const char *);

muse_image *muse_fov_load(const char *aFilename);
cpl_image *muse_convolve_image(const cpl_image *aImage, const cpl_matrix *aKernel);

#endif /* MUSE_UTILS_H */
