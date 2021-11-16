/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2017-2018 European Southern Observatory
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

#ifndef MUSE_RAMAN_H
#define MUSE_RAMAN_H

#include <cpl.h>
#include "muse_lsf.h"

#define MUSE_RAMAN_RANGES 2 /* number of Raman wavelength ranges */

cpl_table *muse_raman_lines_load(muse_processing *);
cpl_error_code muse_raman_add_lsf(muse_pixtable *, cpl_table *, int, double, muse_lsf_cube **);

cpl_array *muse_raman_simulate(muse_pixtable *, cpl_array *, cpl_array *);
muse_image *muse_raman_simulate_image(const muse_image *, const cpl_array *);
cpl_array *muse_raman_fit(muse_pixtable *[MUSE_RAMAN_RANGES]);

#endif /* MUSE_RAMAN_H */
