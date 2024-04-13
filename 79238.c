bool MatchesQueryArg(QueryArg arg, bool value) {
  if (arg == NOT_SET)
    return true;

  return (arg == MATCH_TRUE && value) || (arg == MATCH_FALSE && !value);
}
