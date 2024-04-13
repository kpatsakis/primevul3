void yyfatal(
    yyscan_t yyscanner,
    const char *error_message)
{
  jmp_buf* recovery_state = (jmp_buf*) yr_thread_storage_get_value(
      &recovery_state_key);

  longjmp(*recovery_state, 1);
}
