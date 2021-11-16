/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2015 European Southern Observatory
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

#ifndef MUSE_IDP_H
#define MUSE_IDP_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

#include <muse_processing.h>
#include <muse_datacube.h>

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/* IDP unit strings */
static const char kMuseIdpFluxDataUnit[] = "10**(-20)erg.s**(-1).cm**(-2).angstrom**(-1)";
static const char kMuseIdpFluxStatUnit[] = "10**(-40)erg**2.s**(-2).cm**(-4).angstrom**(-2)";
static const char kMuseIdpWavelengthUnit[] = "angstrom";


/*----------------------------------------------------------------------------*
 *                           Data structures                                  *
 *----------------------------------------------------------------------------*/

// XXX: Should this just be implemented as a property list?
typedef struct muse_idp_properties muse_idp_properties;

struct muse_idp_properties {
    unsigned int ncombine;
    double exptime;
    double texptime;
    double mjd_end;
    double fovcenter[2];
    double wlenrange[2];
    double wlerror;
    double specres;
    double skyres;
    double skyrerr;
    double pixnoise;
    double abmaglimit;
    cpl_array *obid;
    cpl_array *progid;
    cpl_propertylist *prov;
    cpl_array *asson;
    cpl_array *assoc;     /* deprecated */
    cpl_boolean fluxcal;
    const char *prodcatg;
    const char *procsoft;
    const char *obstech;
    const char *referenc;
};

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/

muse_idp_properties *muse_idp_properties_new(void);
void muse_idp_properties_delete(muse_idp_properties *aProperties);

muse_idp_properties *muse_idp_properties_collect(muse_processing *aProcessing,
                                                 const muse_datacube *aCube,
                                                 const char *aTag);
cpl_error_code muse_idp_properties_update(cpl_propertylist *aHeader,
                                          const muse_idp_properties *aProperties);
cpl_error_code muse_idp_properties_update_fov(muse_image *fov);

#endif /* MUSE_IDP_H */
