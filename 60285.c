    static void re_yy_init_buffer  (YY_BUFFER_STATE  b, FILE * file , yyscan_t yyscanner)

{
	int oerrno = errno;
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	re_yy_flush_buffer(b ,yyscanner);

	b->yy_input_file = file;
	b->yy_fill_buffer = 1;

    /* If b is the current buffer, then re_yy_init_buffer was _probably_
     * called from re_yyrestart() or through yy_get_next_buffer.
     * In that case, we don't want to reset the lineno or column.
     */
    if (b != YY_CURRENT_BUFFER){
        b->yy_bs_lineno = 1;
        b->yy_bs_column = 0;
    }

        b->yy_is_interactive = 0;
    
	errno = oerrno;
}
