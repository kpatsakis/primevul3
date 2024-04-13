vips_foreign_load_temp( VipsForeignLoad *load )
{
	const guint64 disc_threshold = vips_get_disc_threshold();
	const guint64 image_size = VIPS_IMAGE_SIZEOF_IMAGE( load->out );

	/* If this is a partial operation, we can open directly.
	 */
	if( load->flags & VIPS_FOREIGN_PARTIAL ) {
#ifdef DEBUG
		printf( "vips_foreign_load_temp: partial temp\n" );
#endif /*DEBUG*/

		return( vips_image_new() );
	}

	/* If it can do sequential access and it's been requested, we can open
	 * directly.
	 */
	if( (load->flags & VIPS_FOREIGN_SEQUENTIAL) && 
		load->access != VIPS_ACCESS_RANDOM ) {
#ifdef DEBUG
		printf( "vips_foreign_load_temp: partial sequential temp\n" );
#endif /*DEBUG*/

		return( vips_image_new() );
	}

	/* ->memory used to be called ->disc and default TRUE. If it's been
	 * forced FALSE, set memory TRUE.
	 */
	if( !load->disc )
		load->memory = TRUE;

	/* We open via disc if:
	 * - 'memory' is off
	 * - the uncompressed image will be larger than 
	 *   vips_get_disc_threshold()
	 */
	if( !load->memory && 
		image_size > disc_threshold ) {
#ifdef DEBUG
		printf( "vips_foreign_load_temp: disc temp\n" );
#endif /*DEBUG*/

		return( vips_image_new_temp_file( "%s.v" ) );
	}

#ifdef DEBUG
	printf( "vips_foreign_load_temp: memory temp\n" );
#endif /*DEBUG*/

	/* Otherwise, fall back to a memory buffer.
	 */
	return( vips_image_new_memory() );
}
