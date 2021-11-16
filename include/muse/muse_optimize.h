/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2005-2011 European Southern Observatory
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
#ifndef MUSE_OPTIMIZE_H
#define MUSE_OPTIMIZE_H
#include <cpl.h>

/**
   @addtogroup muse_utils
   @{
*/
/** 
    @brief Optimization control parameters. 
*/
typedef struct {
  /** 
      @brief Relative error desired in the sum of squares.
      Default value (when set to -1): 30 * DBL_EPSILON

      Termination occurs when both the actual and predicted relative
      reductions in the sum of squares are at most this value. Therefore, it
      measures the relative error desired in the sum of squares.
  */
  double ftol;
  /** 
      @brief Relative error between last two approximations. 
      Default value (when set to -1): 30 * DBL_EPSILON
      
      Termination occurs when the relative error between two consecutive
      iterates is at most this value. Therefore, it measures the relative
      error desired in the approximate solution.
  */
  double xtol;
  /** 
      @brief Orthogonality desired between fvec and its derivs. 
      Default value (when set to -1): 30 * DBL_EPSILON
      
      Termination occurs when the cosine of the angle between the output array
      and any column of the jacobian is at most this value in absolute
      value. Therefore, it measures the orthogonality desired between the
      function vector and the columns of the jacobian.
  */
  double gtol;
  /** 
      @brief Maximum number of iterations. 
      Default value (when set to -1): 100
  */
  int maxcall;
  /**
     @brief Flag to switch on debugging messages.
     Default value: CPL_FALSE
  */
  int debug;
} muse_cpl_optimize_control_t;

/**
   @brief User provided function to evaluate in muse_cpl_optimize_lvmq().
   @param aData Data forwarded from the fit algorithm call.
   @param aPar  Current fit parameter
   @param aRetval Return value vector
   @return CPL_ERROR_NONE if everything went OK. Any other value is 
           considered as error and will stop the minimization 
           muse_cpl_optimize_lvmq().

   @par Sample prototype:
   @verbatim
cpl_error_code
muse_evaluate_sample(void *aData, cpl_array *aPar, cpl_array *aRetval);@endverbatim
 */
typedef  cpl_error_code 
(muse_cpl_evaluate_func)(void *aData, cpl_array *aPar, cpl_array* aRetval);

cpl_error_code 
muse_cpl_optimize_lvmq(void *aData, cpl_array *aPar, int aSize,
		       muse_cpl_evaluate_func *aFunction, 
		       muse_cpl_optimize_control_t *aCtrl);
/**@}*/

#endif /* MUSE_OPTIMIZE_H */
