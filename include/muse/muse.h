/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2008-2015 European Southern Observatory
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

#ifndef MUSE_H
#define MUSE_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include "muse_artifacts.h"
#include "muse_astro.h"
#include "muse_basicproc.h"
#include "muse_combine.h"
#include "muse_cplwrappers.h"
#include "muse_dar.h"
#include "muse_datacube.h"
#include "muse_dfs.h"
#include "muse_findstars.h"
#include "muse_flux.h"
#include "muse_geo.h"
#include "muse_image.h"
#include "muse_mask.h"
#include "muse_pfits.h"
#include "muse_phys.h"
#include "muse_pixgrid.h"
#include "muse_pixtable.h"
#include "muse_postproc.h"
#include "muse_processing.h"
#include "muse_quadrants.h"
#include "muse_quality.h"
#include "muse_raman.h"
#include "muse_resampling.h"
#include "muse_rtcdata.h"
#include "muse_rvcorrect.h"
#include "muse_sky.h"
#include "muse_table.h"
#include "muse_tracing.h"
#include "muse_optimize.h"
#include "muse_utils.h"
#include "muse_wavecalib.h"
#include "muse_wcs.h"
#include "muse_xcombine.h"
#include "muse_data_format_z.h"

#include <muse_lsf_params.h>
#ifdef USE_LSF_PARAMS
#include <muse_sky_old.h>
#endif

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
/* none for this file */

#endif /* MUSE_H */
