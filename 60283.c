    void re_yy_delete_buffer (YY_BUFFER_STATE  b , yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	if ( ! b )
		return;

	if ( b == YY_CURRENT_BUFFER ) /* Not sure if we should pop here. */
		YY_CURRENT_BUFFER_LVALUE = (YY_BUFFER_STATE) 0;

	if ( b->yy_is_our_buffer )
		re_yyfree((void *) b->yy_ch_buf ,yyscanner );

	re_yyfree((void *) b ,yyscanner );
}
