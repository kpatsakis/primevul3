static int ccp_copy_to_sb(struct ccp_cmd_queue *cmd_q,
			  struct ccp_dm_workarea *wa, u32 jobid, u32 sb,
			  u32 byte_swap)
{
	return ccp_copy_to_from_sb(cmd_q, wa, jobid, sb, byte_swap, false);
}