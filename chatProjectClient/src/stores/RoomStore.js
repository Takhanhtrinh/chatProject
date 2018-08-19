import {EventEmitter} from 'events'

import {ACTION_ADD_ROOM, ACTION_ROOM_SELECTED} from '../Constant';
import messageStore from '../stores/MessageStore'
import userStore from '../stores/UserStore'

import Dispatcher from './Dispatcher'

class RoomStore extends EventEmitter {
  constructor() {
    super();
    this.list = new Map();
  }
  actionHandler(action) {
    let actionType = action.actionType;
    switch (actionType) {
      case ACTION_ADD_ROOM: {
        this.list.set(action.id, action.name);
        this.emit('addNewRoom', action.id);
        break;
      }
      case ACTION_ROOM_SELECTED : {
        const roomId = action.roomId;
        this.emit('changeSelectedRoom', roomId);
        break;
      }
    }
  }
  getList() {
    return this.list;
  }
}

var roomStore = new RoomStore;
Dispatcher.register(roomStore.actionHandler.bind(roomStore));
export default roomStore;