static int fmt_sp_to_mp(const struct v4l2_format *f_sp,
			struct v4l2_format *f_mp)
{
	struct v4l2_pix_format_mplane *pix_mp = &f_mp->fmt.pix_mp;
	const struct v4l2_pix_format *pix = &f_sp->fmt.pix;

	if (f_sp->type == V4L2_BUF_TYPE_VIDEO_CAPTURE)
		f_mp->type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	else if (f_sp->type == V4L2_BUF_TYPE_VIDEO_OUTPUT)
		f_mp->type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	else
		return -EINVAL;

	pix_mp->width = pix->width;
	pix_mp->height = pix->height;
	pix_mp->pixelformat = pix->pixelformat;
	pix_mp->field = pix->field;
	pix_mp->colorspace = pix->colorspace;
	pix_mp->num_planes = 1;
	pix_mp->plane_fmt[0].sizeimage = pix->sizeimage;
	pix_mp->plane_fmt[0].bytesperline = pix->bytesperline;

	return 0;
}
