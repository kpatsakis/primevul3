int HttpStreamParser::DoReadBodyComplete(int result) {
  if (result == 0 && !IsResponseBodyComplete() && CanFindEndOfResponse()) {
    if (chunked_decoder_.get())
      result = ERR_INCOMPLETE_CHUNKED_ENCODING;
    else
      result = ERR_CONTENT_LENGTH_MISMATCH;
  }

  if (result > 0 && chunked_decoder_.get()) {
    result = chunked_decoder_->FilterBuf(user_read_buf_->data(), result);
    if (result == 0 && !chunked_decoder_->reached_eof()) {
      io_state_ = STATE_READ_BODY;
      return OK;
    }
  }

  if (result > 0)
    response_body_read_ += result;

  if (result <= 0 || IsResponseBodyComplete()) {
    io_state_ = STATE_DONE;

    int additional_save_amount = read_buf_->offset() - read_buf_unused_offset_;
    int save_amount = 0;
    if (chunked_decoder_.get()) {
      save_amount = chunked_decoder_->bytes_after_eof();
    } else if (response_body_length_ >= 0) {
      int64 extra_data_read = response_body_read_ - response_body_length_;
      if (extra_data_read > 0) {
        save_amount = static_cast<int>(extra_data_read);
        if (result > 0)
          result -= save_amount;
      }
    }

    CHECK_LE(save_amount + additional_save_amount, kMaxBufSize);
    if (read_buf_->capacity() < save_amount + additional_save_amount) {
      read_buf_->SetCapacity(save_amount + additional_save_amount);
    }

    if (save_amount) {
      memcpy(read_buf_->StartOfBuffer(), user_read_buf_->data() + result,
             save_amount);
    }
    read_buf_->set_offset(save_amount);
    if (additional_save_amount) {
      memmove(read_buf_->data(),
              read_buf_->StartOfBuffer() + read_buf_unused_offset_,
              additional_save_amount);
      read_buf_->set_offset(save_amount + additional_save_amount);
    }
    read_buf_unused_offset_ = 0;
  } else {
    io_state_ = STATE_BODY_PENDING;
    user_read_buf_ = NULL;
    user_read_buf_len_ = 0;
  }

  return result;
}
