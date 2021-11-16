/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/* 
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2005-2015 European Southern Observatory
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

/* This file was automatically generated */

#ifndef MUSE_TABLEFORMATS_Z_H
#define MUSE_TABLEFORMATS_Z_H

#include "muse_cplwrappers.h"

extern const muse_cpltable_def muse_euro3dcube_e3d_data_def[];
extern const muse_cpltable_def muse_euro3dcube_e3d_grp_def[];
extern const muse_cpltable_def muse_dataspectrum_def[];

#ifdef XML_TABLEDEF_TRACE_TABLE /* currently not used */
extern const muse_cpltable_def muse_trace_table_def[];
#endif

#ifdef XML_TABLEDEF_TRACE_SAMPLES /* currently not used */
extern const muse_cpltable_def muse_trace_samples_def[];
#endif

#ifdef XML_TABLEDEF_WAVECAL_TABLE /* currently not used */
extern const muse_cpltable_def muse_wavecal_table_def[];
#endif

#ifdef XML_TABLEDEF_WAVECAL_RESIDUALS /* currently not used */
extern const muse_cpltable_def muse_wavecal_residuals_def[];
#endif
extern const muse_cpltable_def muse_lsf_profile_def[];

#ifdef XML_TABLEDEF_GEOMETRY_TABLE /* currently not used */
extern const muse_cpltable_def muse_geometry_table_def[];
#endif

#ifdef XML_TABLEDEF_SPOTS_TABLE /* currently not used */
extern const muse_cpltable_def muse_spots_table_def[];
#endif
extern const muse_cpltable_def muse_autocal_results_def[];
extern const muse_cpltable_def muse_sky_lines_lines_def[];
extern const muse_cpltable_def muse_sky_lines_oh_transitions_def[];
extern const muse_cpltable_def muse_raman_lines_lines_def[];

#ifdef XML_TABLEDEF_FLUX_TABLE /* currently not used */
extern const muse_cpltable_def muse_flux_table_def[];
#endif

#ifdef XML_TABLEDEF_STD_FLUX_TABLE /* currently not used */
extern const muse_cpltable_def muse_std_flux_table_def[];
#endif

#ifdef XML_TABLEDEF_STD_RESPONSE /* currently not used */
extern const muse_cpltable_def muse_std_response_def[];
#endif

#ifdef XML_TABLEDEF_STD_TELLURIC /* currently not used */
extern const muse_cpltable_def muse_std_telluric_def[];
#endif

#ifdef XML_TABLEDEF_EXTINCT_TABLE /* currently not used */
extern const muse_cpltable_def muse_extinct_table_def[];
#endif

#ifdef XML_TABLEDEF_TELLURIC_REGIONS /* currently not used */
extern const muse_cpltable_def muse_telluric_regions_def[];
#endif
#define MUSE_LINE_CATALOG_LAMBDA "lambda"
#define MUSE_LINE_CATALOG_FLUX "flux"
#define MUSE_LINE_CATALOG_ION "ion"
#define MUSE_LINE_CATALOG_QUALITY "quality"
extern const muse_cpltable_def muse_line_catalog_def[];
#define MUSE_BADPIX_X "xpos"
#define MUSE_BADPIX_Y "ypos"
#define MUSE_BADPIX_DQ "status"
#define MUSE_BADPIX_VALUE "value"
extern const muse_cpltable_def muse_badpix_table_def[];

#ifdef XML_TABLEDEF_ASTROMETRY_REFERENCE /* currently not used */
extern const muse_cpltable_def muse_astrometry_reference_def[];
#endif

#ifdef XML_TABLEDEF_FILTER_LIST /* currently not used */
extern const muse_cpltable_def muse_filter_list_def[];
#endif
#define MUSE_OFFSETS_DATEOBS "DATE_OBS"
#define MUSE_OFFSETS_MJDOBS "MJD_OBS"
#define MUSE_OFFSETS_DRA "RA_OFFSET"
#define MUSE_OFFSETS_DDEC "DEC_OFFSET"
#define MUSE_OFFSETS_FSCALE "FLUX_SCALE"
extern const muse_cpltable_def muse_offset_list_def[];
#define MUSE_SRCLIST_ID "Id"
#define MUSE_SRCLIST_X "X"
#define MUSE_SRCLIST_Y "Y"
#define MUSE_SRCLIST_FLUX "Flux"
#define MUSE_SRCLIST_SHARPNESS "Sharpness"
#define MUSE_SRCLIST_ROUNDNESS "Roundness"
#define MUSE_SRCLIST_RA "RA"
#define MUSE_SRCLIST_DEC "DEC"
#define MUSE_SRCLIST_RACORR "RA_CORR"
#define MUSE_SRCLIST_DECCORR "DEC_CORR"
extern const muse_cpltable_def muse_source_list_def[];

#endif /* MUSE_TABLEFORMATS_Z_H */
