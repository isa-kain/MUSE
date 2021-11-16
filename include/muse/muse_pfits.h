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

#ifndef MUSE_PFITS_H
#define MUSE_PFITS_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include "muse_instrument.h"

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/
/* maximum keyword length for FITS headers, including '\0' */
#define KEYWORD_LENGTH 81

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
/* DFS stuff */
const char *muse_pfits_get_arcfile(const cpl_propertylist *);
const char *muse_pfits_get_origfile(const cpl_propertylist *);
const char *muse_pfits_get_pipefile(const cpl_propertylist *);
const char *muse_pfits_get_ancestor(const cpl_propertylist *);
const char *muse_pfits_get_dpr_type(const cpl_propertylist *);
const char *muse_pfits_get_dpr_catg(const cpl_propertylist *);
const char *muse_pfits_get_pro_type(const cpl_propertylist *);
const char *muse_pfits_get_pro_catg(const cpl_propertylist *);
const char *muse_pfits_get_raw_filename(const cpl_propertylist *, unsigned int);
const char *muse_pfits_get_pipe_id(const cpl_propertylist *, unsigned int);
const char *muse_pfits_get_progid(const cpl_propertylist *);
const char *muse_pfits_get_targname(const cpl_propertylist *);
long muse_pfits_get_obsid(const cpl_propertylist *);

/* AO/GALACSI properties */
cpl_boolean muse_pfits_get_ho_loop(const cpl_propertylist *);
cpl_boolean muse_pfits_get_tt_loop(const cpl_propertylist *);
cpl_boolean muse_pfits_get_ir_loop(const cpl_propertylist *);
const char *muse_pfits_get_ao_mirror_name(const cpl_propertylist *, unsigned int);
const char *muse_pfits_get_ao_opti_name(const cpl_propertylist *, unsigned int);

/* instrument properties */
muse_ins_mode muse_pfits_get_mode(const cpl_propertylist *);
const char *muse_pfits_get_insmode(const cpl_propertylist *);
cpl_boolean muse_pfits_has_ifu(const cpl_propertylist *, unsigned char);
const char *muse_pfits_get_drot_mode(const cpl_propertylist *);
double muse_pfits_get_drot_posang(const cpl_propertylist *);
double muse_pfits_get_drot_start(const cpl_propertylist *);
double muse_pfits_get_drot_end(const cpl_propertylist *);

const char *muse_pfits_get_extname(const cpl_propertylist *);
const char *muse_pfits_get_bunit(const cpl_propertylist *);

/* general exposure properties */
cpl_size muse_pfits_get_naxis(const cpl_propertylist *, unsigned int);
double muse_pfits_get_ra(const cpl_propertylist *);
double muse_pfits_get_dec(const cpl_propertylist *);
double muse_pfits_get_equinox(const cpl_propertylist *);
double muse_pfits_get_lst(const cpl_propertylist *);
double muse_pfits_get_mjdobs(const cpl_propertylist *);
const char *muse_pfits_get_dateobs(const cpl_propertylist *);
double muse_pfits_get_exptime(const cpl_propertylist *);

/* WCS properties */
double muse_pfits_get_crpix(const cpl_propertylist *, unsigned int);
double muse_pfits_get_crval(const cpl_propertylist *, unsigned int);
double muse_pfits_get_cd(const cpl_propertylist *, unsigned int, unsigned int);
const char *muse_pfits_get_ctype(const cpl_propertylist *, unsigned int);
const char *muse_pfits_get_cunit(const cpl_propertylist *, unsigned int);

/* CCD properties */
int muse_pfits_get_read_id(const cpl_propertylist *);
const char *muse_pfits_get_read_name(const cpl_propertylist *);
int muse_pfits_get_binx(const cpl_propertylist *);
int muse_pfits_get_biny(const cpl_propertylist *);
const char *muse_pfits_get_chip_name(const cpl_propertylist *);
const char *muse_pfits_get_chip_id(const cpl_propertylist *);
const char *muse_pfits_get_chip_date(const cpl_propertylist *);
cpl_boolean muse_pfits_get_chip_live(const cpl_propertylist *);
double muse_pfits_get_ron(const cpl_propertylist *, unsigned char);
double muse_pfits_get_gain(const cpl_propertylist *, unsigned char);
int muse_pfits_get_out_output_x(const cpl_propertylist *, unsigned char);
int muse_pfits_get_out_output_y(const cpl_propertylist *, unsigned char);
int muse_pfits_get_out_nx(const cpl_propertylist *, unsigned char);
int muse_pfits_get_out_ny(const cpl_propertylist *, unsigned char);
int muse_pfits_get_out_prescan_x(const cpl_propertylist *, unsigned char);
int muse_pfits_get_out_prescan_y(const cpl_propertylist *, unsigned char);
int muse_pfits_get_out_overscan_x(const cpl_propertylist *, unsigned char);
int muse_pfits_get_out_overscan_y(const cpl_propertylist *, unsigned char);

/* telescope properties */
double muse_pfits_get_geolat(const cpl_propertylist *);
double muse_pfits_get_geolon(const cpl_propertylist *);
double muse_pfits_get_geoelev(const cpl_propertylist *);
double muse_pfits_get_focu_scale(const cpl_propertylist *);
double muse_pfits_get_airmass_start(const cpl_propertylist *);
double muse_pfits_get_airmass_end(const cpl_propertylist *);
double muse_pfits_get_altang(const cpl_propertylist *);
double muse_pfits_get_parang_start(const cpl_propertylist *);
double muse_pfits_get_parang_end(const cpl_propertylist *);

/* seeing FWHM estimators */
double muse_pfits_get_agx_avg(const cpl_propertylist *);
double muse_pfits_get_agx_rms(const cpl_propertylist *);
double muse_pfits_get_agy_avg(const cpl_propertylist *);
double muse_pfits_get_agy_rms(const cpl_propertylist *);
double muse_pfits_get_ia_fwhm(const cpl_propertylist *);
double muse_pfits_get_ia_fwhmlin(const cpl_propertylist *aHeaders);

/* ambient telescope properties */
double muse_pfits_get_temp(const cpl_propertylist *);
double muse_pfits_get_rhum(const cpl_propertylist *);
double muse_pfits_get_pres_start(const cpl_propertylist *);
double muse_pfits_get_pres_end(const cpl_propertylist *);
double muse_pfits_get_fwhm_start(const cpl_propertylist *);
double muse_pfits_get_fwhm_end(const cpl_propertylist *);

/* diode, lamp, and shutter properties */
double muse_pfits_get_pam_intensity(const cpl_propertylist *, int);
double muse_pfits_get_pam_stdev(const cpl_propertylist *, int);
const char *muse_pfits_get_pam2_filter(const cpl_propertylist *);
int muse_pfits_get_lampnum(const cpl_propertylist *);
const char *muse_pfits_get_lamp_name(const cpl_propertylist *, int);
int muse_pfits_get_lamp_status(const cpl_propertylist *, int);
double muse_pfits_get_lamp_ontime(const cpl_propertylist *, int);
const char *muse_pfits_get_shut_name(const cpl_propertylist *, int);
int muse_pfits_get_shut_status(const cpl_propertylist *, int);

/* encoders */
int muse_pfits_get_posenc(const cpl_propertylist *, unsigned short);
double muse_pfits_get_pospos(const cpl_propertylist *, unsigned short);

#endif /* MUSE_PFITS_H */
