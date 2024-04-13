bool RootWindowHostLinux::Dispatch(const base::NativeEvent& event) {
  XEvent* xev = event;

  CheckXEventForConsistency(xev);

  switch (xev->type) {
    case Expose:
      root_window_->ScheduleFullDraw();
      break;
    case KeyPress: {
      KeyEvent keydown_event(xev, false);
      root_window_->DispatchKeyEvent(&keydown_event);
      break;
    }
    case KeyRelease: {
      KeyEvent keyup_event(xev, false);
      root_window_->DispatchKeyEvent(&keyup_event);
      break;
    }
    case ButtonPress: {
      if (static_cast<int>(xev->xbutton.button) == kBackMouseButton ||
          static_cast<int>(xev->xbutton.button) == kForwardMouseButton) {
        client::UserGestureClient* gesture_client =
            client::GetUserGestureClient(root_window_);
        if (gesture_client) {
          gesture_client->OnUserGesture(
              static_cast<int>(xev->xbutton.button) == kBackMouseButton ?
              client::UserGestureClient::GESTURE_BACK :
              client::UserGestureClient::GESTURE_FORWARD);
        }
        break;
      }
    }  // fallthrough
    case ButtonRelease: {
      MouseEvent mouseev(xev);
      root_window_->DispatchMouseEvent(&mouseev);
      break;
    }
    case FocusOut:
      if (xev->xfocus.mode != NotifyGrab)
        root_window_->SetCapture(NULL);
      break;
    case ConfigureNotify: {
      DCHECK_EQ(xwindow_, xev->xconfigure.window);
      DCHECK_EQ(xwindow_, xev->xconfigure.event);
      gfx::Rect bounds(xev->xconfigure.x, xev->xconfigure.y,
                       xev->xconfigure.width, xev->xconfigure.height);
      bool size_changed = bounds_.size() != bounds.size();
      bounds_ = bounds;
      if (pointer_barriers_.get()) {
        UnConfineCursor();
        gfx::Point p = root_window_->last_mouse_location();
        XWarpPointer(xdisplay_, None,  xwindow_, 0, 0, 0, 0, p.x(), p.y());
        ConfineCursorToRootWindow();
      }
      if (size_changed)
        root_window_->OnHostResized(bounds.size());
      break;
    }
    case GenericEvent: {
      ui::TouchFactory* factory = ui::TouchFactory::GetInstance();
      if (!factory->ShouldProcessXI2Event(xev))
        break;

      ui::EventType type = ui::EventTypeFromNative(xev);
      XEvent last_event;
      int num_coalesced = 0;

      switch (type) {
        case ui::ET_TOUCH_PRESSED:
        case ui::ET_TOUCH_RELEASED:
        case ui::ET_TOUCH_MOVED: {
          TouchEvent touchev(xev);
          root_window_->DispatchTouchEvent(&touchev);
          break;
        }
        case ui::ET_MOUSE_MOVED:
        case ui::ET_MOUSE_DRAGGED:
        case ui::ET_MOUSE_PRESSED:
        case ui::ET_MOUSE_RELEASED:
        case ui::ET_MOUSEWHEEL:
        case ui::ET_MOUSE_ENTERED:
        case ui::ET_MOUSE_EXITED: {
          if (type == ui::ET_MOUSE_MOVED || type == ui::ET_MOUSE_DRAGGED) {
            num_coalesced = CoalescePendingXIMotionEvents(xev, &last_event);
            if (num_coalesced > 0)
              xev = &last_event;
          } else if (type == ui::ET_MOUSE_PRESSED) {
            XIDeviceEvent* xievent =
                static_cast<XIDeviceEvent*>(xev->xcookie.data);
            int button = xievent->detail;
            if (button == kBackMouseButton || button == kForwardMouseButton) {
              client::UserGestureClient* gesture_client =
                  client::GetUserGestureClient(root_window_);
              if (gesture_client) {
                bool reverse_direction =
                    ui::IsTouchpadEvent(xev) && ui::IsNaturalScrollEnabled();
                gesture_client->OnUserGesture(
                    (button == kBackMouseButton && !reverse_direction) ||
                    (button == kForwardMouseButton && reverse_direction) ?
                    client::UserGestureClient::GESTURE_BACK :
                    client::UserGestureClient::GESTURE_FORWARD);
              }
              break;
            }
          }
          MouseEvent mouseev(xev);
          root_window_->DispatchMouseEvent(&mouseev);
          break;
        }
        case ui::ET_SCROLL_FLING_START:
        case ui::ET_SCROLL_FLING_CANCEL:
        case ui::ET_SCROLL: {
          ScrollEvent scrollev(xev);
          root_window_->DispatchScrollEvent(&scrollev);
          break;
        }
        case ui::ET_UNKNOWN:
          break;
        default:
          NOTREACHED();
      }

      if (num_coalesced > 0)
        XFreeEventData(xev->xgeneric.display, &last_event.xcookie);
      break;
    }
    case MapNotify: {
      if (!IsWindowManagerPresent() && focus_when_shown_)
        XSetInputFocus(xdisplay_, xwindow_, RevertToNone, CurrentTime);
      break;
    }
    case ClientMessage: {
      Atom message_type = static_cast<Atom>(xev->xclient.data.l[0]);
      X11AtomCache* cache = X11AtomCache::GetInstance();
      if (message_type == cache->GetAtom(ui::ATOM_WM_DELETE_WINDOW)) {
        root_window_->OnRootWindowHostClosed();
      } else if (message_type == cache->GetAtom(ui::ATOM__NET_WM_PING)) {
        XEvent reply_event = *xev;
        reply_event.xclient.window = x_root_window_;

        XSendEvent(xdisplay_,
                   reply_event.xclient.window,
                   False,
                   SubstructureRedirectMask | SubstructureNotifyMask,
                   &reply_event);
      }
      break;
    }
    case MappingNotify: {
      switch (xev->xmapping.request) {
        case MappingModifier:
        case MappingKeyboard:
          XRefreshKeyboardMapping(&xev->xmapping);
          root_window_->OnKeyboardMappingChanged();
          break;
        case MappingPointer:
          ui::UpdateButtonMap();
          break;
        default:
          NOTIMPLEMENTED() << " Unknown request: " << xev->xmapping.request;
          break;
      }
      break;
    }
    case MotionNotify: {
      XEvent last_event;
      while (XPending(xev->xany.display)) {
        XEvent next_event;
        XPeekEvent(xev->xany.display, &next_event);
        if (next_event.type == MotionNotify &&
            next_event.xmotion.window == xev->xmotion.window &&
            next_event.xmotion.subwindow == xev->xmotion.subwindow &&
            next_event.xmotion.state == xev->xmotion.state) {
          XNextEvent(xev->xany.display, &last_event);
          xev = &last_event;
        } else {
          break;
        }
      }

      MouseEvent mouseev(xev);
      root_window_->DispatchMouseEvent(&mouseev);
      break;
    }
  }
  return true;
}
