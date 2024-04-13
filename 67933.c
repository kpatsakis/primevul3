vips_foreign_load_iscompat( VipsImage *a, VipsImage *b )
{
	if( a->Xsize != b->Xsize ||
		a->Ysize != b->Ysize ||
		a->Bands != b->Bands ||
		a->Coding != b->Coding ||
		a->BandFmt != b->BandFmt ) {
		vips_error( "VipsForeignLoad",
			"%s", _( "images do not match" ) ); 
		return( FALSE );
	}

	return( TRUE );
}
