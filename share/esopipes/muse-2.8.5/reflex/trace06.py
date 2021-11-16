#!/usr/bin/env python
import reflex
import sys
import os
import_ok = 1
try:
    from astropy.io import fits as pyfits
except ImportError:
    try:
        import pyfits
    except ImportError:
        import_ok = 0
try:
    import math
except ImportError:
    import_ok = 0


if __name__ == '__main__':

    parser = reflex.ReflexIOParser()

    # Define inputs
    parser.add_option("-i", "--in_sof", dest="in_sof")
    # Define outputs
    parser.add_output("-o", "--out_sof", dest="out_sof")

    inputs = parser.get_inputs()
    outputs = parser.get_outputs()

    # Retrieve input
    in_sof = inputs.in_sof

    # Get the input files
    files = in_sof.files

    # Do the stuff
    if import_ok == 0:
        outputs.out_sof = inputs.in_sof
        parser.write_outputs()
        sys.exit()

    # Read the content of TRACE_TABLE_06 (static, if present)
    for file in files:
        if file.category == 'TRACE_TABLE_06':
            static = pyfits.open(file.name, mode='readonly')
            newdata = static[1].data

    # Read the content of the TRATE_TABLE-06.fits, produced by the pipeline
    for file in files:
        if os.path.basename(file.name) == 'TRACE_TABLE-06.fits':
            hdulist = pyfits.open(file.name, mode='update')
            DataTable = hdulist[1].data
            Width = DataTable.field('Width')
            total = sum(Width)
            # If there is a NaN, it replaces the content of TRATE_TABLE-06.fits
            # with that of the static table.
            if math.isnan(total):
                hdulist[1].data = newdata
                hdulist.flush()
                DataTable = hdulist[1].data
                Width = DataTable.field('Width')
                total = sum(Width)

    outputs.out_sof = inputs.in_sof
    parser.write_outputs()

    sys.exit()
