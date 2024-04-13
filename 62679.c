static int fmt_mp_to_sp(const struct v4l2_format *f_mp,
			struct v4l2_format *f_sp)
{
	const struct v4l2_pix_format_mplane *pix_mp = &f_mp->fmt.pix_mp;
	struct v4l2_pix_format *pix = &f_sp->fmt.pix;

	if (f_mp->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		f_sp->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	else if (f_mp->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE)
		f_sp->type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	else
		return -EINVAL;

	pix->width = pix_mp->width;
	pix->height = pix_mp->height;
	pix->pixelformat = pix_mp->pixelformat;
	pix->field = pix_mp->field;
	pix->colorspace = pix_mp->colorspace;
	pix->sizeimage = pix_mp->plane_fmt[0].sizeimage;
	pix->bytesperline = pix_mp->plane_fmt[0].bytesperline;

	return 0;
}
