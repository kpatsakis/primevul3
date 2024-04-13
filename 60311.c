static void yy_flex_strncpy (char* s1, yyconst char * s2, int n , yyscan_t yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	(void)yyg;

	int i;
	for ( i = 0; i < n; ++i )
		s1[i] = s2[i];
}
