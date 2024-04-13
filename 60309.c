static void yy_fatal_error (yyconst char* msg , yyscan_t yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	(void)yyg;
	(void) fprintf( stderr, "%s\n", msg );
	exit( YY_EXIT_FAILURE );
}
