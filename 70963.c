close_ops_fd (void)
{
  SetupOp *op;

  for (op = ops; op != NULL; op = op->next)
    {
      if (op->fd != -1)
        {
          (void) close (op->fd);
          op->fd = -1;
        }
    }
}
