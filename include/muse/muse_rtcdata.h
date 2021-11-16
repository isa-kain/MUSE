/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2016 European Southern Observatory
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

#ifndef MUSE_RTCDATA_H
#define MUSE_RTCDATA_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
#include <cpl.h>

/*----------------------------------------------------------------------------*
 *                              Defines                                       *
 *----------------------------------------------------------------------------*/

/* FITS header keywords to store AO performance indicators obtained *
 * from the SPARTA RTC                                              */
#define MUSE_HDR_RTC_STREHL      "ESO DRS MUSE RTC STREHL"
#define MUSE_HDR_RTC_STREHL_C    "Median Strehl ratio computed from RTC measurements"
#define MUSE_HDR_RTC_STREHLERR   "ESO DRS MUSE RTC STREHERR"
#define MUSE_HDR_RTC_STREHLERR_C "MAD of the Strehl ratio"

/*----------------------------------------------------------------------------*
 *                           Data structures                                  *
 *----------------------------------------------------------------------------*/

/** @addtogroup muse_rtcdata */
/**@{*/

/*----------------------------------------------------------------------------*/
/**
  @brief    Structure to store the SPARTA data tables of the MUSE
            NFM exposures.
. */
/*----------------------------------------------------------------------------*/
typedef struct {
  /**
    @brief    The atmospheric turbulence data member

    This component contains the atmospheric turbulence data as determined by
    the SPARTA real-time computer.
   */
  cpl_table *atm;
  /**
    @brief    The estimated atmospheric turbulence profile member.

    This component contains the atmospheric turbulence profile estimated
    by the SPARTA real-time computer.

    Currently this is data member is set to @c NULL because it is not used.
    It is present for a future extension!
   */
  cpl_table *cn2;
} muse_rtcdata;

/**@}*/

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
muse_rtcdata *muse_rtcdata_new(void);
void muse_rtcdata_delete(muse_rtcdata *aRtcdata);

muse_rtcdata *muse_rtcdata_load(const char *aFilename);

cpl_error_code muse_rtcdata_median_strehl(const muse_rtcdata *aRtcdata,
                                          double *aStrehlMedian,
                                          double *aStrehlMad);

#endif /* MUSE_RTCDATA_H */
