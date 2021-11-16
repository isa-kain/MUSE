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

#ifndef MUSE_CPLWRAPPERS_H
#define MUSE_CPLWRAPPERS_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *                           Data structures                                  *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_cplwrappers */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
 * @brief Definition of a cpl table structure.
 *
 * Each field describes one column. The total structure is an NULL terminated
 * array of those columns.
 *
 * @note The four string components of the structure are declared as constant,
 *       so they cannot be changed after initial setup.
 */
/*----------------------------------------------------------------------------*/
typedef struct {
  /** @brief Column name. */
  const char *name;
  /** @brief Column type (use CPL_TYPE_POINTER for array columns). */
  cpl_type type;
  /** @brief Column unit, or NULL. */
  const char *unit;
  /** @brief Default print format, or NULL. */
  const char *format;
  /** @brief Table column description, or NULL */
  const char *description;
  /** @brief Is column required? */
  const int required;
} muse_cpltable_def;

/*----------------------------------------------------------------------------*/
/**
 * @brief Type for the framework that called the recipe.
 */
/*----------------------------------------------------------------------------*/

typedef enum {
  /** Unknown framework type */
  MUSE_CPLFRAMEWORK_UNKNOWN = 0,
  /** Recipe was called from EsoREX */
  MUSE_CPLFRAMEWORK_ESOREX = 1,
  /** Recipe was called from python-cpl */
  MUSE_CPLFRAMEWORK_PYTHONCPL = 2,
  /** Recipe was called from gasgano */
  MUSE_CPLFRAMEWORK_GASGANO = 3
} muse_cplframework_type;

/*----------------------------------------------------------------------------*/
/**
 * @brief Matrix element comparison function type definition.
 */
/*----------------------------------------------------------------------------*/
typedef int (*muse_cplmatrix_element_compare_func)(double aValue1, double aValue2);

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
cpl_error_code muse_cplimage_or(cpl_image *, const cpl_image *, unsigned int);
cpl_image *muse_cplimage_concat_x(const cpl_image *, const cpl_image *);
cpl_image *muse_cplimage_concat_y(const cpl_image *, const cpl_image *);
cpl_image *muse_cplimage_filter_median_subtract(cpl_image *, unsigned int, unsigned int);
cpl_vector *muse_cplimage_slope_window(const cpl_image *, const cpl_size *);
double muse_cplimage_get_percentile(const cpl_image *, double);
cpl_error_code muse_cplimage_copy_within_mask(cpl_image *, const cpl_image *, const cpl_mask *);

cpl_image *muse_cplimagelist_collapse_or_create(const cpl_imagelist *);

cpl_mask *muse_cplmask_adapt_to_image(const cpl_mask *, const cpl_image *);
cpl_error_code muse_cplmask_fill_window(cpl_mask *, cpl_size, cpl_size, cpl_size, cpl_size, cpl_binary);

cpl_matrix *muse_cplmatrix_multiply_create(const cpl_matrix *, const cpl_matrix *);
cpl_array *muse_cplmatrix_where(const cpl_matrix *, double, muse_cplmatrix_element_compare_func);
cpl_matrix *muse_cplmatrix_extract_selected(const cpl_matrix *, const cpl_array *);

double muse_cplvector_get_adev_const(const cpl_vector *, double);
double muse_cplvector_get_median_dev(cpl_vector *, double *);
double muse_cplvector_get_semiquartile(cpl_vector *);
cpl_error_code muse_cplvector_threshold(cpl_vector *, double, double, double, double);
cpl_error_code muse_cplvector_erase_element(cpl_vector *, int);
cpl_size muse_cplvector_count_unique(const cpl_vector *);
cpl_vector *muse_cplvector_get_unique(const cpl_vector *);

cpl_table *muse_cpltable_new(const muse_cpltable_def *, cpl_size);
cpl_table *muse_cpltable_load(const char *, const char *, const muse_cpltable_def []);
cpl_error_code muse_cpltable_check(const cpl_table *, const muse_cpltable_def *);
cpl_error_code muse_cpltable_append_file(const cpl_table *, const char *, const char *, const muse_cpltable_def[]);
cpl_array *muse_cpltable_extract_column(cpl_table *, const char *);
cpl_error_code muse_cpltable_copy_array(cpl_table *, const char *, const cpl_array *);
cpl_array *muse_cpltable_get_array_copy(cpl_table *, const char *, cpl_size);
cpl_size muse_cpltable_find_sorted(const cpl_table *, const char *, double);

cpl_array *muse_cplarray_new_from_image(const cpl_image *);
cpl_error_code muse_cplarray_poly1d(cpl_array *, const cpl_array *);
double muse_cplarray_poly1d_double(double, const cpl_array *);
cpl_array *muse_cplarray_extract(cpl_array *, cpl_size, cpl_size);
cpl_error_code muse_cplarray_add_window(cpl_array *a, cpl_size, const cpl_array *);
cpl_error_code muse_cplarray_sort(cpl_array *, cpl_boolean order);
cpl_bivector *muse_cplarray_histogram(const cpl_array *, double, double, double);
cpl_size muse_cplarray_find_sorted(const cpl_array *, double);
cpl_boolean muse_cplarray_has_duplicate(const cpl_array *);
cpl_error_code muse_cplarray_dump_name(const cpl_array *, const char *);
cpl_error_code muse_cplarray_erase_invalid(cpl_array *);
cpl_size muse_cplarray_erase_outliers(cpl_array *, const cpl_bivector *, cpl_size, double);
cpl_array *muse_cplarray_diff(const cpl_array *, int);
cpl_error_code muse_cplarray_erf(cpl_array *);
cpl_error_code muse_cplarray_exp(cpl_array *);
cpl_array *muse_cplarray_interpolate_linear(const cpl_array *, const cpl_array *, const cpl_array *);
cpl_array *muse_cplarray_interpolate_table_linear(const cpl_array *, const cpl_table *, const char *, const char *);
cpl_array *muse_cplarray_new_from_delimited_string(const char *, const char *);
cpl_array *muse_cplarray_string_to_double(const cpl_array *);

cpl_parameter *muse_cplparamerterlist_find_prefix(cpl_parameterlist *, const char *, const char *);
cpl_parameterlist *muse_cplparameterlist_from_propertylist(const cpl_propertylist *, int);
cpl_parameterlist *muse_cplparameterlist_duplicate(const cpl_parameterlist *);

cpl_error_code muse_cplpropertylist_update_long_long(cpl_propertylist *, const char *, cpl_size);
cpl_error_code muse_cplpropertylist_update_fp(cpl_propertylist *, const char *, double);

cpl_error_code muse_cplframeset_erase_all(cpl_frameset *);
cpl_error_code muse_cplframeset_erase_duplicate(cpl_frameset *);

void muse_cplerrorstate_dump_some(unsigned, unsigned, unsigned);
muse_cplframework_type muse_cplframework(void);

#endif /* MUSE_CPLWRAPPERS_H */
