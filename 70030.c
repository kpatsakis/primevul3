static bool rds_recvmsg_zcookie(struct rds_sock *rs, struct msghdr *msg)
{
	struct rds_msg_zcopy_queue *q = &rs->rs_zcookie_queue;
	struct rds_msg_zcopy_info *info = NULL;
	struct rds_zcopy_cookies *done;
	unsigned long flags;

	if (!msg->msg_control)
		return false;

	if (!sock_flag(rds_rs_to_sk(rs), SOCK_ZEROCOPY) ||
	    msg->msg_controllen < CMSG_SPACE(sizeof(*done)))
		return false;

	spin_lock_irqsave(&q->lock, flags);
	if (!list_empty(&q->zcookie_head)) {
		info = list_entry(q->zcookie_head.next,
				  struct rds_msg_zcopy_info, rs_zcookie_next);
		list_del(&info->rs_zcookie_next);
	}
	spin_unlock_irqrestore(&q->lock, flags);
	if (!info)
		return false;
	done = &info->zcookies;
	if (put_cmsg(msg, SOL_RDS, RDS_CMSG_ZCOPY_COMPLETION, sizeof(*done),
		     done)) {
		spin_lock_irqsave(&q->lock, flags);
		list_add(&info->rs_zcookie_next, &q->zcookie_head);
		spin_unlock_irqrestore(&q->lock, flags);
		return false;
	}
	kfree(info);
	return true;
}
