PP_Bool PPB_ImageData_Impl::Describe(PP_ImageDataDesc* desc) {
  desc->format = format_;
  desc->size.width = width_;
  desc->size.height = height_;
  desc->stride = width_ * 4;
  return PP_TRUE;
}
