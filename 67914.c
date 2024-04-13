file_add_class( VipsForeignClass *class, GSList **files )
{
	/* Append so we don't reverse the list of files. Sort will not reorder
	 * items of equal priority. 
	 */
	*files = g_slist_append( *files, class );

	return( NULL );
}
