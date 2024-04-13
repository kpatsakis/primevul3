vips__foreign_convert_saveable( VipsImage *in, VipsImage **ready,
	VipsSaveable saveable, VipsBandFormat *format, VipsCoding *coding,
	VipsArrayDouble *background )
{
	/* in holds a reference to the output of our chain as we build it.
	 */
	g_object_ref( in );

	/* For coded images, can this class save the coding we are in now? 
	 * Nothing to do.
	 */
	if( in->Coding != VIPS_CODING_NONE &&
		coding[in->Coding] ) {
		*ready = in;
		return( 0 );
	}

	/* For uncoded images, if this saver supports ANY bands and this 
	 * format we have nothing to do.
	 */
	if( in->Coding == VIPS_CODING_NONE &&
	        saveable == VIPS_SAVEABLE_ANY &&
		format[in->BandFmt] == in->BandFmt ) {
		*ready = in;
		return( 0 );
	}

	/* Otherwise ... we need to decode and then (possibly) recode at the
	 * end.
	 */

	/* If this is an VIPS_CODING_LABQ, we can go straight to RGB.
	 */
	if( in->Coding == VIPS_CODING_LABQ ) {
		VipsImage *out;

		if( vips_LabQ2sRGB( in, &out, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	/* If this is an VIPS_CODING_RAD, we unpack to float. This could be
	 * scRGB or XYZ. 
	 */
	if( in->Coding == VIPS_CODING_RAD ) {
		VipsImage *out;

		if( vips_rad2float( in, &out, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	/* If the saver supports RAD, we need to go to scRGB or XYZ. 
	 */
	if( coding[VIPS_CODING_RAD] ) {
		if( in->Type != VIPS_INTERPRETATION_scRGB &&
			in->Type != VIPS_INTERPRETATION_XYZ ) {
			VipsImage *out;

			if( vips_colourspace( in, &out, 
				VIPS_INTERPRETATION_scRGB, NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
	}

	/* If this image is CMYK and the saver is RGB-only, use lcms to try to
	 * import to XYZ. This will only work if the image has an embedded
	 * profile. 
	 */
	if( in->Type == VIPS_INTERPRETATION_CMYK &&
		in->Bands >= 4 &&
		(saveable == VIPS_SAVEABLE_RGB ||
		 saveable == VIPS_SAVEABLE_RGBA ||
		 saveable == VIPS_SAVEABLE_RGBA_ONLY) ) { 
		VipsImage *out;

		if( vips_icc_import( in, &out, 
			"pcs", VIPS_PCS_XYZ,
			NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;

		/* We've imported to PCS, we must remove the embedded profile,
		 * since it no longer matches the image.
		 *
		 * For example, when converting CMYK JPG to RGB PNG, we need 
		 * to remove the CMYK profile on import, or the png writer will 
		 * try to attach it when we write the image as RGB.
		 */
		vips_image_remove( in, VIPS_META_ICC_NAME );
	}

	/* If this is something other than CMYK or RAD, eg. maybe a LAB image,
	 * we need to transform to RGB.
	 */
	if( !coding[VIPS_CODING_RAD] &&
		in->Bands >= 3 &&
		in->Type != VIPS_INTERPRETATION_CMYK &&
		vips_colourspace_issupported( in ) &&
		(saveable == VIPS_SAVEABLE_RGB ||
		 saveable == VIPS_SAVEABLE_RGBA ||
		 saveable == VIPS_SAVEABLE_RGBA_ONLY ||
		 saveable == VIPS_SAVEABLE_RGB_CMYK) ) { 
		VipsImage *out;
		VipsInterpretation interpretation;

		/* Do we make RGB or RGB16? We don't want to squash a 16-bit
		 * RGB down to 8 bits if the saver supports 16. 
		 */
		if( vips_band_format_is8bit( format[in->BandFmt] ) )
			interpretation = VIPS_INTERPRETATION_sRGB;
		else
			interpretation = VIPS_INTERPRETATION_RGB16;

		if( vips_colourspace( in, &out, interpretation, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	/* VIPS_SAVEABLE_RGBA_ONLY does not support 1 or 2 bands ... convert 
	 * to sRGB. 
	 */
	if( !coding[VIPS_CODING_RAD] &&
		in->Bands < 3 &&
		vips_colourspace_issupported( in ) &&
		saveable == VIPS_SAVEABLE_RGBA_ONLY ) { 
		VipsImage *out;
		VipsInterpretation interpretation;

		/* Do we make RGB or RGB16? We don't want to squash a 16-bit
		 * RGB down to 8 bits if the saver supports 16. 
		 */
		if( vips_band_format_is8bit( format[in->BandFmt] ) )
			interpretation = VIPS_INTERPRETATION_sRGB;
		else
			interpretation = VIPS_INTERPRETATION_RGB16;

		if( vips_colourspace( in, &out, interpretation, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	/* Get the bands right. We must do this after all colourspace
	 * transforms, since they can change the number of bands. 
	 */
	if( in->Coding == VIPS_CODING_NONE ) {
		/* Do we need to flatten out an alpha channel? There needs to
		 * be an alpha there now, and this writer needs to not support
		 * alpha.
		 */
		if( (in->Bands == 2 ||
			(in->Bands == 4 && 
			 in->Type != VIPS_INTERPRETATION_CMYK)) &&
			(saveable == VIPS_SAVEABLE_MONO ||
			 saveable == VIPS_SAVEABLE_RGB ||
			 saveable == VIPS_SAVEABLE_RGB_CMYK) ) {
			VipsImage *out;

			if( vips_flatten( in, &out, 
				"background", background,
				NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}

		/* Other alpha removal strategies ... just drop the extra
		 * bands.
		 */

		else if( in->Bands > 3 && 
			(saveable == VIPS_SAVEABLE_RGB ||
			 (saveable == VIPS_SAVEABLE_RGB_CMYK &&
			  in->Type != VIPS_INTERPRETATION_CMYK)) ) { 
			VipsImage *out;

			/* Don't let 4 bands though unless the image really is
			 * a CMYK.
			 *
			 * Consider a RGBA png being saved as JPG. We can
			 * write CMYK jpg, but we mustn't do that for RGBA
			 * images.
			 */
			if( vips_extract_band( in, &out, 0, 
				"n", 3,
				NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
		else if( in->Bands > 4 && 
			((saveable == VIPS_SAVEABLE_RGB_CMYK &&
			  in->Type == VIPS_INTERPRETATION_CMYK) ||
			 saveable == VIPS_SAVEABLE_RGBA ||
			 saveable == VIPS_SAVEABLE_RGBA_ONLY) ) {
			VipsImage *out;

			if( vips_extract_band( in, &out, 0, 
				"n", 4,
				NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
		else if( in->Bands > 1 && 
			saveable == VIPS_SAVEABLE_MONO ) {
			VipsImage *out;

			if( vips_extract_band( in, &out, 0, NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}

		/* Else we have VIPS_SAVEABLE_ANY and we don't chop bands down.
		 */
	}

	/* Handle the ushort interpretations.
	 *
	 * RGB16 and GREY16 use 0-65535 for black-white. If we have an image
	 * tagged like this, and it has more than 8 bits (we leave crazy uchar
	 * images tagged as RGB16 alone), we'll need to get it ready for the
	 * saver.
	 */
	if( (in->Type == VIPS_INTERPRETATION_RGB16 ||
		 in->Type == VIPS_INTERPRETATION_GREY16) &&
		!vips_band_format_is8bit( in->BandFmt ) ) {
		/* If the saver supports ushort, cast to ushort. It may be
		 * float at the moment, for example.
		 *
		 * If the saver does not support ushort, automatically shift
		 * it down. This is the behaviour we want for saving an RGB16
		 * image as JPG, for example.
		 */
		if( format[VIPS_FORMAT_USHORT] == VIPS_FORMAT_USHORT ) {
			VipsImage *out;

			if( vips_cast( in, &out, VIPS_FORMAT_USHORT, NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
		else {
			VipsImage *out;

			if( vips_rshift_const1( in, &out, 8, NULL ) ) { 
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;

			/* That could have produced an int image ... make sure 
			 * we are now uchar.
			 */
			if( vips_cast( in, &out, VIPS_FORMAT_UCHAR, NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
	}

	/* Cast to the output format.
	 */
	{
		VipsImage *out;

		if( vips_cast( in, &out, format[in->BandFmt], NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	/* Does this class want a coded image? Search the coding table for the
	 * first one.
	 */
	if( coding[VIPS_CODING_NONE] ) {
		/* Already NONE, nothing to do.
		 */
	}
	else if( coding[VIPS_CODING_LABQ] ) {
		VipsImage *out;

		if( vips_Lab2LabQ( in, &out, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}
	else if( coding[VIPS_CODING_RAD] ) {
		VipsImage *out;

		if( vips_float2rad( in, &out, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	*ready = in;

	return( 0 );
}
