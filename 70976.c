resolve_symlinks_in_ops (void)
{
  SetupOp *op;

  for (op = ops; op != NULL; op = op->next)
    {
      const char *old_source;

      switch (op->type)
        {
        case SETUP_RO_BIND_MOUNT:
        case SETUP_DEV_BIND_MOUNT:
        case SETUP_BIND_MOUNT:
          old_source = op->source;
          op->source = realpath (old_source, NULL);
          if (op->source == NULL)
            {
              if (op->flags & ALLOW_NOTEXIST && errno == ENOENT)
                op->source = old_source;
              else
                die_with_error("Can't find source path %s", old_source);
            }
          break;
        default:
          break;
        }
    }
}
