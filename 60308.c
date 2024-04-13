int yr_parse_re_string(
  const char* re_string,
  int flags,
  RE** re,
  RE_ERROR* error)
{
  yyscan_t yyscanner;
  jmp_buf recovery_state;
  RE_LEX_ENVIRONMENT lex_env;

  lex_env.last_error_code = ERROR_SUCCESS;

  yr_thread_storage_set_value(&recovery_state_key, &recovery_state);

  if (setjmp(recovery_state) != 0)
    return ERROR_INTERNAL_FATAL_ERROR;

  FAIL_ON_ERROR(yr_re_create(re));

  (*re)->flags = flags;

  re_yylex_init(&yyscanner);
  re_yyset_extra(*re,yyscanner);
  re_yy_scan_string(re_string,yyscanner);
  yyparse(yyscanner, &lex_env);
  re_yylex_destroy(yyscanner);

  if (lex_env.last_error_code != ERROR_SUCCESS)
  {
    yr_re_destroy(*re);
    *re = NULL;

    strlcpy(
        error->message,
        lex_env.last_error_message,
        sizeof(error->message));

    return lex_env.last_error_code;
  }

  return ERROR_SUCCESS;
}
