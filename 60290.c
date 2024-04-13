void *re_yyalloc (yy_size_t  size , yyscan_t yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	(void)yyg;
	return (void *) malloc( size );
}
