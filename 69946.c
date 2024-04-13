vips_giflib_file_read( GifFileType *gfile, GifByteType *buffer, int n )
{
	VipsForeignLoadGif *gif = (VipsForeignLoadGif *) gfile->UserData;
	VipsForeignLoadGifFile *file = (VipsForeignLoadGifFile *) gif;

	if( feof( file->fp ) )
		gif->eof = TRUE;

	return( (int) fread( (void *) buffer, 1, n, file->fp ) );
}
