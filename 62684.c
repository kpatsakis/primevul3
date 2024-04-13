static inline void v4l_print_ext_ctrls(unsigned int cmd,
	struct video_device *vfd, struct v4l2_ext_controls *c, int show_vals)
{
	__u32 i;

	if (!(vfd->debug & V4L2_DEBUG_IOCTL_ARG))
		return;
	dbgarg(cmd, "");
	printk(KERN_CONT "class=0x%x", c->ctrl_class);
	for (i = 0; i < c->count; i++) {
		if (show_vals && !c->controls[i].size)
			printk(KERN_CONT " id/val=0x%x/0x%x",
				c->controls[i].id, c->controls[i].value);
		else
			printk(KERN_CONT " id=0x%x,size=%u",
				c->controls[i].id, c->controls[i].size);
	}
	printk(KERN_CONT "\n");
};
