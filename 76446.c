    void SetList(VertexAttribInfoList* new_list) {
      DCHECK(new_list);

      if (list_) {
        list_->erase(it_);
      }

      it_ = new_list->insert(new_list->end(), this);
      list_ = new_list;
    }
