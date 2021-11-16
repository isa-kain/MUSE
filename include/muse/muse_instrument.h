/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set sw=2 sts=2 et cin: */
/*
 * This file is part of the MUSE Instrument Pipeline
 * Copyright (C) 2008-2018 European Southern Observatory
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

#ifndef MUSE_INSTRUMENT_H
#define MUSE_INSTRUMENT_H

/*----------------------------------------------------------------------------*
 *                              Includes                                      *
 *----------------------------------------------------------------------------*/
/* none for this file */

/*----------------------------------------------------------------------------*
 *                              Telescope properties                          *
 *----------------------------------------------------------------------------*/
/* these are the values that we got for the effective pupil area of   *
 * the VLT for the focal position of MUSE, and the semi-retracted sky *
 * baffle diameter as apparently used with MUSE                       */
static const double kVLTPupilDiameter = 798.62; /* [cm] */
static const double kVLTSkyBaffleDeployed = 150.0; /* [cm] */
/* now the real area that we want, this is                              *
 *    (kVLTPupilDiameter**2 - kVLTSkyBaffleDeployed**2) * PI / 4        *
 * [use way more digits than the measurement accuracy would suggest...] */
static const double kVLTArea = 483250.7024654051; /* [cm**2] */

/*----------------------------------------------------------------------------*
 *                              Instrument properties                         *
 *                                                                            *
 * These variables are defined as static because they are included in several *
 * C source files and linking will otherwise not work.                        *
 *----------------------------------------------------------------------------*/
/* nominal sizes of MUSE spaxels in both WFM and NFM */
static const double kMuseSpaxelSizeX_WFM = 0.2; /* as defined by the...      */
static const double kMuseSpaxelSizeY_WFM = 0.2; /* ... geometry table pixels */
static const double kMuseSpaxelSizeX_NFM = 0.02519; /* computed from NFM ... */
static const double kMuseSpaxelSizeY_NFM = 0.02542; /* astrometric fields    */

/* Expected average FWHM [pix] of a point-like lamp emission line on the CCD */
static const double kMuseAverageFWHM_x = 1.6;
static const double kMuseAverageFWHM_y = 1.6;

/* Output positions as they should be recorded in the FITS header (OUTn X/Y). *
 * Values for the lower quadrants are always 1, so they are not listed here.  */
static const int kMuseOutputXRight = 4096;
static const int kMuseOutputYTop = 4112;
/* standard size of pre- and overscans */
static const int kMusePreOverscanSize = 32;

/* all values above this in raw data are considered saturated */
static const float kMuseSaturationLimit = 65500.;

/* number of IFUs per exposure */
static const int kMuseNumIFUs = 24;

/* number of slices in each MUSE CCD/IFU */
static const unsigned short kMuseSlicesPerCCD = 48;
/* maximum offset of the first slice to the CCD edge to be expected [pix] */
static const int kMuseSliceSearchRegion = 200;
/* maximum slice width ever to be expected [pix] */
static const int kMuseSliceMaxWidth = 100;
/* nominal width of a slice [pix] within the MUSE field of view */
static const int kMuseSliceNominalWidth = 75;
/* low and high values for likely slice widths to be found (in INM data, *
 * values are between 74.2+/-0.2 and 79.2+/-0.3; take a bit more here)   */
static const float kMuseSliceLoLikelyWidth = 72.2;
static const float kMuseSliceHiLikelyWidth = 82.2;
/* max expected width of a slice edge [pix] */
/* XXX effectively switch this off by choosing a high number,       *
 * otherwise tracing inaccuracies in the blue mark many pixels dark */
static const int kMuseSliceMaxEdgeWidth = 10;

/* MUSE nominal spectral sampling in Angstrom */
static const float kMuseSpectralSamplingA = 1.25;
/* slice width in wavelength direction, in Angstrom. See 
   R.Bacon, Analysis of the spectral PSF, 2009, VLT-TRE-MUS-14670-0870.  */
static const double kMuseSliceSlitWidthA = 2.61;

/* MUSE nominal spectral range in Angstrom (extended wavelength range, WFM!) */
static const float kMuseNominalLambdaMin = 4650.;
static const float kMuseNominalLambdaMax = 9300.;
/* approximate lower limit of nominal wavelength range with detectable flux */
static const float kMuseNominalCutoff = 4750;
static const float kMuseAOCutoff = 4690;
/* where the blue cutoff filter really kicks in */
static const float kMuseNominalCutoffKink = 4775.;
static const float kMuseAOCutoffKink = 4700.;
static const float kMuseNFMCutoffKink = 4770.;
/* most extreme wavelength ranges possibly recorded on the MUSE CCDs */
static const float kMuseLambdaMinX = 4150.;
static const float kMuseLambdaMaxX = 9800.;
/* useful wavelength ranges:                         *
 * common, and illuminated ranges between all slices */
static const float kMuseUsefulELambdaMin = 4600.;
static const float kMuseUsefulNLambdaMin = 4750.;
static const float kMuseUsefulAOLambdaMin = 4700.;
static const float kMuseUsefulNFMLambdaMin = 4750.;
static const float kMuseUsefulLambdaMax = 9350.;
/* range where the NaD notch filters absorbs significant flux */
static const float kMuseNaLambdaMin = 5755.; /* for WFM-AO-E */
static const float kMuseNaLambdaMax = 6008.;
static const float kMuseNa2LambdaMin = 5805.; /* for WFM-AO-N */
static const float kMuseNa2LambdaMax = 5966.;
static const float kMuseNaGLambdaMin = 5780.; /* for NFM-AO-N with...     */
static const float kMuseNaGLambdaMax = 6050.; /* ... GALACSI notch filter */
/* maximum centering error [pix] to allow for arc lines */
static const float kMuseArcMaxCenteringError = 1.25;
/* minimum and maximum FWHM [pix] to allow for arc lines */
static const float kMuseArcMinFWHM = 1.0;
static const float kMuseArcMaxFWHM = 5.0;
/* minimum flux (Gaussian area) measured for arc lines */
static const float kMuseArcMinFlux = 50.0;

/* typical MUSE datacube size in pixels, assuming "natural" sampling */
static const int kMuseTypicalCubeSizeX = 300;
static const int kMuseTypicalCubeSizeY = 288;

/* The sky slice number for the CCD slice /index/, e.g. to get      *
 * sky-slice number for CCD slice 12, use kMuseGeoSliceSky[12 - 1]. *
 * This relation was taken from the "Global Positioning System"     *
 * document (VLT-TRE-MUS-14670-0657 v1.06, dated 2010-12-14).       */
static const unsigned char kMuseGeoSliceSky[] = {
   9,  8,  1, 10,  7,  2, 11,  6,  3, 12,  5,  4, /* stack 4 */
  21, 20, 13, 22, 19, 14, 23, 18, 15, 24, 17, 16, /* stack 3 */
  33, 32, 25, 34, 31, 26, 35, 30, 27, 36, 29, 28, /* stack 2 */
  45, 44, 37, 46, 43, 38, 47, 42, 39, 48, 41, 40  /* stack 1 */
};

/* numbers from CU multi-pinhole mask (CUmpm / MultiP) design */
static const float kMuseCUmpmDX = 2.9450, /* +/- 0.0010 mm horizontal hole distance */
                   kMuseCUmpmDY = 0.6135; /* +/- 0.0010 mm vertical hole distance */

/* expected number of pinholes per slice */
static const unsigned char kMuseCUmpmSpotsPerSlice = 3;

/*----------------------------------------------------------------------------*
 * Instrument constants related to the photo diodes/pico amplfiers (PAM/AMPL) *
 *----------------------------------------------------------------------------*/
/* spectral sensitivity of photo diode 1 (BN-DSR-100F-1, SN 20478), *
 * wavelengths in [Angstrom], sensitivity in [A/W]                  */
static const double kMuseAmpl1[][2] = {
  {  2500., 0.12042285 },
  {  2600., 0.10860598 },
  {  2700., 0.10029241 },
  {  2800., 0.1010473 },
  {  2900., 0.11258141 },
  {  3000., 0.12635038 },
  {  3100., 0.13448966 },
  {  3200., 0.13880026 },
  {  3300., 0.1423209 },
  {  3400., 0.14222383 },
  {  3500., 0.14018333 },
  {  3600., 0.13863897 },
  {  3700., 0.14285144 },
  {  3800., 0.15266077 },
  {  3900., 0.16535348 },
  {  4000., 0.1770276 },
  {  4100., 0.18605701 },
  {  4200., 0.19385006 },
  {  4300., 0.20142353 },
  {  4400., 0.20986195 },
  {  4500., 0.21819498 },
  {  4600., 0.22561278 },
  {  4700., 0.23293989 },
  {  4800., 0.23908501 },
  {  4900., 0.24622256 },
  {  5000., 0.25217267 },
  {  5100., 0.25847973 },
  {  5200., 0.2651326 },
  {  5300., 0.27124827 },
  {  5400., 0.27774291 },
  {  5500., 0.28288282 },
  {  5600., 0.28884646 },
  {  5700., 0.29560289 },
  {  5800., 0.30107169 },
  {  5900., 0.30661891 },
  {  6000., 0.31317781 },
  {  6100., 0.3185658 },
  {  6200., 0.3247822 },
  {  6300., 0.33013667 },
  {  6400., 0.33590527 },
  {  6500., 0.34120136 },
  {  6600., 0.34646811 },
  {  6700., 0.35219427 },
  {  6800., 0.35768868 },
  {  6900., 0.36601971 },
  {  7000., 0.36940652 },
  {  7100., 0.3735187 },
  {  7200., 0.37902808 },
  {  7300., 0.38475409 },
  {  7400., 0.39095198 },
  {  7500., 0.39615039 },
  {  7600., 0.40184217 },
  {  7700., 0.40682911 },
  {  7800., 0.41245076 },
  {  7900., 0.41851005 },
  {  8000., 0.42465427 },
  {  8100., 0.4292767 },
  {  8200., 0.43596086 },
  {  8300., 0.44151703 },
  {  8400., 0.4458525 },
  {  8500., 0.45129183 },
  {  8600., 0.45772061 },
  {  8700., 0.46303733 },
  {  8800., 0.4683445 },
  {  8900., 0.4728841 },
  {  9000., 0.47827183 },
  {  9100., 0.48365046 },
  {  9200., 0.48835047 },
  {  9300., 0.49350086 },
  {  9400., 0.49763584 },
  {  9500., 0.50120751 },
  {  9600., 0.50337176 },
  {  9700., 0.50266209 },
  {  9800., 0.49823605 },
  {  9900., 0.48887253 },
  { 10000., 0.47199137 },
  { 10100., 0.44744711 },
  { 10200., 0.41367036 },
  { 10300., 0.37072293 },
  { 10400., 0.31936173 },
  { 10500., 0.26252744 },
  { 10600., 0.2092668 },
  { 10700., 0.16926374 },
  { 10800., 0.13819477 },
  { 10900., 0.11170627 },
  { 11000., 0.089087876 },
  { 0.,     0. } /* signify end of curve */
};
/* spectral sensitivity of photo diode 2 (PD-9304-1, SN 16536aw1), *
 * wavelengths in [Angstrom], sensitivity in [A/W]                 */
static const double kMuseAmpl2[][2] = {
  {  3800., 0.017470643 },
  {  3900., 0.028092218 },
  {  4000., 0.042101743 },
  {  4100., 0.057172072 },
  {  4200., 0.072846443 },
  {  4300., 0.088312251 },
  {  4400., 0.10446471 },
  {  4500., 0.12060106 },
  {  4600., 0.13705176 },
  {  4700., 0.15291203 },
  {  4800., 0.16783374 },
  {  4900., 0.18414104 },
  {  5000., 0.2002173 },
  {  5100., 0.21605915 },
  {  5200., 0.23228403 },
  {  5300., 0.24804161 },
  {  5400., 0.26350352 },
  {  5500., 0.27958034 },
  {  5600., 0.2948433 },
  {  5700., 0.3098418 },
  {  5800., 0.32511413 },
  {  5900., 0.3396649 },
  {  6000., 0.35432272 },
  {  6100., 0.36892068 },
  {  6200., 0.38268224 },
  {  6300., 0.3965248 },
  {  6400., 0.41013499 },
  {  6500., 0.42306177 },
  {  6600., 0.43585509 },
  {  6700., 0.44871039 },
  {  6800., 0.46041305 },
  {  6900., 0.47379706 },
  {  7000., 0.48452311 },
  {  7100., 0.49428201 },
  {  7200., 0.50518957 },
  {  7300., 0.51604378 },
  {  7400., 0.52577538 },
  {  7500., 0.53560665 },
  {  7600., 0.54571763 },
  {  7700., 0.55427559 },
  {  7800., 0.56320912 },
  {  7900., 0.57262044 },
  {  8000., 0.58063817 },
  {  8100., 0.58978081 },
  {  8200., 0.59650748 },
  {  8300., 0.60433599 },
  {  8400., 0.61148995 },
  {  8500., 0.61874995 },
  {  8600., 0.62610294 },
  {  8700., 0.63217003 },
  {  8800., 0.63791206 },
  {  8900., 0.6449613 },
  {  9000., 0.65062382 },
  {  9100., 0.65685802 },
  {  9200., 0.66314835 },
  {  9300., 0.6684782 },
  {  9400., 0.67349904 },
  {  9500., 0.67880835 },
  {  9600., 0.68235343 },
  {  9700., 0.68390407 },
  {  9800., 0.68117654 },
  {  9900., 0.67307881 },
  { 10000., 0.65663219 },
  { 10100., 0.62937674 },
  { 10200., 0.5886552 },
  { 10300., 0.53176402 },
  { 10400., 0.46039876 },
  { 10500., 0.37838965 },
  { 10600., 0.29937021 },
  { 10700., 0.23830659 },
  { 10800., 0.19124631 },
  { 10900., 0.1523348 },
  { 11000., 0.1193815 },
  { 0.,     0. } /* signify end of curve */
};
/* physical size of the sensitive area of each photo diode */
static const double kMuseAmplPhysicalSize = 1.; /* [cm**2] */

/*----------------------------------------------------------------------------*
 *                          Special variable types                            *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
  @brief    MUSE instrument observing mode

  This enum is ordered so that
  - the non-AO extended mode comes first
  - non-AO modes come first and the AO modes later
  - extended mode AO comes between both WFM nominal modes
  - WFM comes before NFM
 */
/*----------------------------------------------------------------------------*/
typedef enum {
  MUSE_MODE_WFM_NONAO_E = 0,
  MUSE_MODE_WFM_NONAO_N = 1,
  MUSE_MODE_WFM_AO_E = 2,
  MUSE_MODE_WFM_AO_N = 3,
  MUSE_MODE_NFM_AO_N = 4
} muse_ins_mode;

/*----------------------------------------------------------------------------*
 *                           Function prototypes                              *
 *----------------------------------------------------------------------------*/
/* none for this file */

#endif /* MUSE_INSTRUMENT_H */
