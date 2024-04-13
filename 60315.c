    static yy_state_type yy_try_NUL_trans  (yy_state_type yy_current_state , yyscan_t yyscanner)
{
	int yy_is_jam;
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner; /* This var may be unused depending upon options. */
	char *yy_cp = yyg->yy_c_buf_p;

	YY_CHAR yy_c = 1;
	if ( yy_accept[yy_current_state] )
		{
		yyg->yy_last_accepting_state = yy_current_state;
		yyg->yy_last_accepting_cpos = yy_cp;
		}
	while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
		{
		yy_current_state = (int) yy_def[yy_current_state];
		if ( yy_current_state >= 45 )
			yy_c = yy_meta[(unsigned int) yy_c];
		}
	yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
	yy_is_jam = (yy_current_state == 44);

	(void)yyg;
	return yy_is_jam ? 0 : yy_current_state;
}
