void ff_init_block_index(MpegEncContext *s){ //FIXME maybe rename
    const int linesize   = s->current_picture.f->linesize[0]; //not s->linesize as this would be wrong for field pics
    const int uvlinesize = s->current_picture.f->linesize[1];
    const int width_of_mb = (4 + (s->avctx->bits_per_raw_sample > 8)) - s->avctx->lowres;
    const int height_of_mb = 4 - s->avctx->lowres;

    s->block_index[0]= s->b8_stride*(s->mb_y*2    ) - 2 + s->mb_x*2;
    s->block_index[1]= s->b8_stride*(s->mb_y*2    ) - 1 + s->mb_x*2;
    s->block_index[2]= s->b8_stride*(s->mb_y*2 + 1) - 2 + s->mb_x*2;
    s->block_index[3]= s->b8_stride*(s->mb_y*2 + 1) - 1 + s->mb_x*2;
    s->block_index[4]= s->mb_stride*(s->mb_y + 1)                + s->b8_stride*s->mb_height*2 + s->mb_x - 1;
    s->block_index[5]= s->mb_stride*(s->mb_y + s->mb_height + 2) + s->b8_stride*s->mb_height*2 + s->mb_x - 1;

    s->dest[0] = s->current_picture.f->data[0] + (int)((s->mb_x - 1U) <<  width_of_mb);
    s->dest[1] = s->current_picture.f->data[1] + (int)((s->mb_x - 1U) << (width_of_mb - s->chroma_x_shift));
    s->dest[2] = s->current_picture.f->data[2] + (int)((s->mb_x - 1U) << (width_of_mb - s->chroma_x_shift));

    if(!(s->pict_type==AV_PICTURE_TYPE_B && s->avctx->draw_horiz_band && s->picture_structure==PICT_FRAME))
    {
        if(s->picture_structure==PICT_FRAME){
        s->dest[0] += s->mb_y *   linesize << height_of_mb;
        s->dest[1] += s->mb_y * uvlinesize << (height_of_mb - s->chroma_y_shift);
        s->dest[2] += s->mb_y * uvlinesize << (height_of_mb - s->chroma_y_shift);
        }else{
            s->dest[0] += (s->mb_y>>1) *   linesize << height_of_mb;
            s->dest[1] += (s->mb_y>>1) * uvlinesize << (height_of_mb - s->chroma_y_shift);
            s->dest[2] += (s->mb_y>>1) * uvlinesize << (height_of_mb - s->chroma_y_shift);
            av_assert1((s->mb_y&1) == (s->picture_structure == PICT_BOTTOM_FIELD));
        }
    }
}
