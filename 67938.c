vips_foreign_operation_init( void )
{
	extern GType vips_foreign_load_rad_get_type( void ); 
	extern GType vips_foreign_save_rad_file_get_type( void ); 
	extern GType vips_foreign_save_rad_buffer_get_type( void ); 
	extern GType vips_foreign_load_mat_get_type( void ); 
	extern GType vips_foreign_load_ppm_get_type( void ); 
	extern GType vips_foreign_save_ppm_get_type( void ); 
	extern GType vips_foreign_load_png_get_type( void ); 
	extern GType vips_foreign_load_png_buffer_get_type( void ); 
	extern GType vips_foreign_save_png_file_get_type( void ); 
	extern GType vips_foreign_save_png_buffer_get_type( void ); 
	extern GType vips_foreign_load_csv_get_type( void ); 
	extern GType vips_foreign_save_csv_get_type( void ); 
	extern GType vips_foreign_load_matrix_get_type( void ); 
	extern GType vips_foreign_save_matrix_get_type( void ); 
	extern GType vips_foreign_print_matrix_get_type( void ); 
	extern GType vips_foreign_load_fits_get_type( void ); 
	extern GType vips_foreign_save_fits_get_type( void ); 
	extern GType vips_foreign_load_analyze_get_type( void ); 
	extern GType vips_foreign_load_openexr_get_type( void ); 
	extern GType vips_foreign_load_openslide_get_type( void ); 
	extern GType vips_foreign_load_jpeg_file_get_type( void ); 
	extern GType vips_foreign_load_jpeg_buffer_get_type( void ); 
	extern GType vips_foreign_save_jpeg_file_get_type( void ); 
	extern GType vips_foreign_save_jpeg_buffer_get_type( void ); 
	extern GType vips_foreign_save_jpeg_mime_get_type( void ); 
	extern GType vips_foreign_load_tiff_file_get_type( void ); 
	extern GType vips_foreign_load_tiff_buffer_get_type( void ); 
	extern GType vips_foreign_save_tiff_file_get_type( void ); 
	extern GType vips_foreign_save_tiff_buffer_get_type( void ); 
	extern GType vips_foreign_load_vips_get_type( void ); 
	extern GType vips_foreign_save_vips_get_type( void ); 
	extern GType vips_foreign_load_raw_get_type( void ); 
	extern GType vips_foreign_save_raw_get_type( void ); 
	extern GType vips_foreign_save_raw_fd_get_type( void ); 
	extern GType vips_foreign_load_magick_file_get_type( void ); 
	extern GType vips_foreign_load_magick_buffer_get_type( void ); 
	extern GType vips_foreign_load_magick7_file_get_type( void ); 
	extern GType vips_foreign_load_magick7_buffer_get_type( void ); 
	extern GType vips_foreign_save_dz_file_get_type( void ); 
	extern GType vips_foreign_save_dz_buffer_get_type( void ); 
	extern GType vips_foreign_load_webp_file_get_type( void ); 
	extern GType vips_foreign_load_webp_buffer_get_type( void ); 
	extern GType vips_foreign_save_webp_file_get_type( void ); 
	extern GType vips_foreign_save_webp_buffer_get_type( void ); 
	extern GType vips_foreign_load_pdf_get_type( void ); 
	extern GType vips_foreign_load_pdf_file_get_type( void ); 
	extern GType vips_foreign_load_pdf_buffer_get_type( void ); 
	extern GType vips_foreign_load_svg_get_type( void ); 
	extern GType vips_foreign_load_svg_file_get_type( void ); 
	extern GType vips_foreign_load_svg_buffer_get_type( void ); 
	extern GType vips_foreign_load_gif_get_type( void ); 
	extern GType vips_foreign_load_gif_file_get_type( void ); 
	extern GType vips_foreign_load_gif_buffer_get_type( void ); 

	vips_foreign_load_csv_get_type(); 
	vips_foreign_save_csv_get_type(); 
	vips_foreign_load_matrix_get_type(); 
	vips_foreign_save_matrix_get_type(); 
	vips_foreign_print_matrix_get_type(); 
	vips_foreign_load_raw_get_type(); 
	vips_foreign_save_raw_get_type(); 
	vips_foreign_save_raw_fd_get_type(); 
	vips_foreign_load_vips_get_type(); 
	vips_foreign_save_vips_get_type(); 

#ifdef HAVE_ANALYZE
	vips_foreign_load_analyze_get_type(); 
#endif /*HAVE_ANALYZE*/

#ifdef HAVE_PPM
	vips_foreign_load_ppm_get_type(); 
	vips_foreign_save_ppm_get_type(); 
#endif /*HAVE_PPM*/

#ifdef HAVE_RADIANCE
	vips_foreign_load_rad_get_type(); 
	vips_foreign_save_rad_file_get_type(); 
	vips_foreign_save_rad_buffer_get_type(); 
#endif /*HAVE_RADIANCE*/

#ifdef HAVE_POPPLER
	vips_foreign_load_pdf_get_type(); 
	vips_foreign_load_pdf_file_get_type(); 
	vips_foreign_load_pdf_buffer_get_type(); 
#endif /*HAVE_POPPLER*/

#ifdef HAVE_RSVG
	vips_foreign_load_svg_get_type(); 
	vips_foreign_load_svg_file_get_type(); 
	vips_foreign_load_svg_buffer_get_type(); 
#endif /*HAVE_RSVG*/

#ifdef HAVE_GIFLIB
	vips_foreign_load_gif_get_type(); 
	vips_foreign_load_gif_file_get_type(); 
	vips_foreign_load_gif_buffer_get_type(); 
#endif /*HAVE_GIFLIB*/

#ifdef HAVE_GSF
	vips_foreign_save_dz_file_get_type(); 
	vips_foreign_save_dz_buffer_get_type(); 
#endif /*HAVE_GSF*/

#ifdef HAVE_PNG
	vips_foreign_load_png_get_type(); 
	vips_foreign_load_png_buffer_get_type(); 
	vips_foreign_save_png_file_get_type(); 
	vips_foreign_save_png_buffer_get_type(); 
#endif /*HAVE_PNG*/

#ifdef HAVE_MATIO
	vips_foreign_load_mat_get_type(); 
#endif /*HAVE_MATIO*/

#ifdef HAVE_JPEG
	vips_foreign_load_jpeg_file_get_type(); 
	vips_foreign_load_jpeg_buffer_get_type(); 
	vips_foreign_save_jpeg_file_get_type(); 
	vips_foreign_save_jpeg_buffer_get_type(); 
	vips_foreign_save_jpeg_mime_get_type(); 
#endif /*HAVE_JPEG*/

#ifdef HAVE_LIBWEBP
	vips_foreign_load_webp_file_get_type(); 
	vips_foreign_load_webp_buffer_get_type(); 
	vips_foreign_save_webp_file_get_type(); 
	vips_foreign_save_webp_buffer_get_type(); 
#endif /*HAVE_LIBWEBP*/

#ifdef HAVE_TIFF
	vips_foreign_load_tiff_file_get_type(); 
	vips_foreign_load_tiff_buffer_get_type(); 
	vips_foreign_save_tiff_file_get_type(); 
	vips_foreign_save_tiff_buffer_get_type(); 
#endif /*HAVE_TIFF*/

#ifdef HAVE_OPENSLIDE
	vips_foreign_load_openslide_get_type(); 
#endif /*HAVE_OPENSLIDE*/

#ifdef HAVE_MAGICK
	vips_foreign_load_magick_file_get_type(); 
	vips_foreign_load_magick_buffer_get_type(); 
#endif /*HAVE_MAGICK*/

#ifdef HAVE_MAGICK7
	vips_foreign_load_magick7_file_get_type(); 
	vips_foreign_load_magick7_buffer_get_type(); 
#endif /*HAVE_MAGICK7*/

#ifdef HAVE_CFITSIO
	vips_foreign_load_fits_get_type(); 
	vips_foreign_save_fits_get_type(); 
#endif /*HAVE_CFITSIO*/

#ifdef HAVE_OPENEXR
	vips_foreign_load_openexr_get_type(); 
#endif /*HAVE_OPENEXR*/

	vips__foreign_load_operation = 
		g_quark_from_static_string( "vips-foreign-load-operation" ); 
}
