import {EventEmitter} from 'events'

import {ACTION_ADD_ROOM, ACTION_ADD_USERS, ACTION_USER_ENTER_ROOM, ACTION_USER_LEFT_ROOM, ACTION_ROOM_SELECTED} from '../Constant';

import Dispatcher from './Dispatcher'

class UserStore extends EventEmitter {
  constructor() {
    super();
    this.list = new Map();
    // room id users
  }
  getList() {
    return this.list;
  }
  getUserName(roomId, userId) {
    var array = this.list.get(roomId);
    if (array != undefined) {
      for (var i = 0; i < array.length; i++) {
        if (array[i].id == userId) return array[i].name;
      }
    }
    return undefined;
  }
  actionHandler(action) {
    let actionType = action.actionType;
    switch (actionType) {
      case ACTION_ADD_ROOM: {
        let id = action.id;
        
        this.list.set(id, []);
        this.emit('selectedRoom', id);
        break;
      }
      case ACTION_ADD_USERS: {
        let data = action.data;
        var array = this.list.get(action.roomId);
        if (array == undefined) array = [];
        for (var i = 0; i < data.length; i++) {
          let d = data[i];
          let id = d.id;
          let name = d.name;
          array.push({id: id, name: name});
          this.list.set(id, array);
        }
        this.emit('change', action.roomId);
        break;
      }
      case ACTION_USER_LEFT_ROOM: {
        let roomId = action.roomId;
        let userId = action.userId;
        var array = this.list.get(roomId);
        var index = -1;
        for (var i = 0; i < array.length; i++) {
          if (array[i].id == userId) {
            index = i;
            break;
          }
        }
        if (index == -1) {
          throw new String('this user is not in this room');
        } else {
          array.splice(index, 1);
          this.emit('change', roomId);
        }
        break;
      }
      case ACTION_USER_ENTER_ROOM: {
        let roomId = action.roomId;
        let userId = action.userId;
        let name = action.name
        var array = this.list.get(roomId);
        if (array == undefined) throw new String('array is null');
        array.push({id: userId, name: name});
        this.emit('change', roomId);
        break;
      }
      case ACTION_ROOM_SELECTED: {
        let id = action.roomId;
        this.emit('selectedRoom', id);
        break;
      }
    }
  }
}
var userStore = new UserStore;
window.userStore = userStore;
Dispatcher.register(userStore.actionHandler.bind(userStore));
export default userStore;