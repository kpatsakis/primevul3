static int are_comps_same_dimensions(opj_image_t * image)
{
    unsigned int i;
    for (i = 1; i < image->numcomps; i++) {
        if (image->comps[0].dx != image->comps[i].dx ||
                image->comps[0].dy != image->comps[i].dy) {
            return OPJ_FALSE;
        }
    }
    return OPJ_TRUE;
}
