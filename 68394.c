static void lo_rw_aio_complete(struct kiocb *iocb, long ret, long ret2)
{
	struct loop_cmd *cmd = container_of(iocb, struct loop_cmd, iocb);

	if (cmd->css)
		css_put(cmd->css);
	cmd->ret = ret;
	lo_rw_aio_do_completion(cmd);
}
