static inline void dbgrect(struct video_device *vfd, char *s,
							struct v4l2_rect *r)
{
	dbgarg2("%sRect start at %dx%d, size=%dx%d\n", s, r->left, r->top,
						r->width, r->height);
};
