mrb_io_modestr_to_flags(mrb_state *mrb, const char *mode)
{
  int flags = 0;
  const char *m = mode;

  switch (*m++) {
    case 'r':
      flags |= FMODE_READABLE;
      break;
    case 'w':
      flags |= FMODE_WRITABLE | FMODE_CREATE | FMODE_TRUNC;
      break;
    case 'a':
      flags |= FMODE_WRITABLE | FMODE_APPEND | FMODE_CREATE;
      break;
    default:
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "illegal access mode %S", mrb_str_new_cstr(mrb, mode));
  }

  while (*m) {
    switch (*m++) {
      case 'b':
        flags |= FMODE_BINMODE;
        break;
      case '+':
        flags |= FMODE_READWRITE;
        break;
      case ':':
        /* XXX: PASSTHROUGH*/
      default:
        mrb_raisef(mrb, E_ARGUMENT_ERROR, "illegal access mode %S", mrb_str_new_cstr(mrb, mode));
    }
  }

  return flags;
}
