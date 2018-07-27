import {EventEmitter} from 'events'

import { ADD_NEW_USER_IN_A_ROOM,USERS_IN_A_ROOM, JOIN_A_ROOM_RESPOND_PACKET, ADD_NEW_USER_PACKET, REMOVE_USER_PACKET, REMOVE_USER, NEW_TEXT_MESSAGE_PACKET, NEW_TEXT_MESSAGE} from '../Constant';
import userStore from '../stores/UserStore'
import messageStore from '../stores/MessageStore'

class RoomStore extends EventEmitter {
  constructor() {
    super();
    this.list = new Map();
  }
  createSample() {
    var buffer = [];
    var offset = 0;
    buffer[offset++] = 97;
    buffer[offset++] = 1;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 1;
    buffer[offset++] = 3;
    buffer[offset++] = 65;
    buffer[offset++] = 66;
    buffer[offset++] = 67;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    // buffer[offset++] = 0;
    // buffer[offset++] = 0;
    // buffer[offset++] = 0;
    // buffer[offset++] = 1;
    // buffer[offset++] = 3;
    // buffer[offset++] = 65;
    // buffer[offset++] = 66;
    // buffer[offset++] = 67;
    // buffer[offset++] = 0;
    // buffer[offset++] = 0;
    // buffer[offset++] = 0;
    // buffer[offset++] = 22;
    // buffer[offset++] = 3;
    // buffer[offset++] = 56;
    // buffer[offset++] = 66;
    // buffer[offset++] = 75;

    return buffer;
  }
  createSample1() {
    var buffer = [];
    var offset = 0;
    buffer[offset++] = 97;
    buffer[offset++] = 1;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 2;
    buffer[offset++] = 3;
    buffer[offset++] = 65;
    buffer[offset++] = 66;
    buffer[offset++] = 67;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 2;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 1;
    buffer[offset++] = 3;
    buffer[offset++] = 65;
    buffer[offset++] = 66;
    buffer[offset++] = 67;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 0;
    buffer[offset++] = 22;
    buffer[offset++] = 3;
    buffer[offset++] = 56;
    buffer[offset++] = 66;
    buffer[offset++] = 76;

    return buffer;
  }
  createMessagePacket(id,message) {
      var buffer = [];
      var offset = 0;
      buffer[offset++] = 'p'.charCodeAt(0);
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 2;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 1;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = id;
      for (var i = 0 ; i < message.length; i++) {
          buffer[offset++] = message.charCodeAt(i);
      }
      return buffer;

  }
  createAddUserPacket(roomId, userId, name) {
      var buffer = [];
      var offset = 0;
      buffer[offset++] = 'o'.charCodeAt(0);
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = roomId;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = 0;
      buffer[offset++] = userId;
      for (var i = 0 ; i < name.length; i++) {
          buffer[offset++] = name.charCodeAt(i);
      }
      return buffer;
  }
  getList() {
    return this.list;
  }
  deSerialize(data) {
    var uint8Array = new Uint8Array(data);
    var arrayBuffer = uint8Array.buffer;

    var view = new DataView(arrayBuffer);

    var offset = 0;
    const type = view.getUint8(offset++);
    console.log('type ', type);
    switch (type) {
      case JOIN_A_ROOM_RESPOND_PACKET: {
        const status = view.getUint8(offset++);
        const ROOM_NOT_EXIST = -1;
        const ROOM_EXIST = 1;
        if (status == ROOM_NOT_EXIST) {
          throw new ('ROOM_NOT_EXIST');
        } else if (status == ROOM_EXIST) {
          // type status roomId roomLen roomname totalUser user
          var msg = {roomId: 0, nameLen: 0, name: ' ', totalUser: 0, users: []};
          const roomId = view.getUint32(offset);
          msg.roomId = roomId;
          offset += 4;
          const nameLen = view.getUint8(offset++);
          console.log('nameLen ', nameLen);
          msg.nameLen = nameLen;
          var name = new String();
          for (var i = 0; i < nameLen; i++) {
            name += String.fromCharCode(view.getUint8(offset++));
          }
          msg.name = name;
          const totalUser = view.getUint32(offset);
          offset += 4;
          msg.totalUser = totalUser;
          for (var i = 0; i < totalUser; i++) {
            const userId = view.getUint32(offset);
            offset += 4;
            const nameLen = view.getUint8(offset++);
            var userName = new String();
            for (var j = 0; j < nameLen; j++) {
              userName += String.fromCharCode(view.getUint8(offset++));
            }
            msg.users.push({id: userId, name: userName})
          }
          var data = {};
          data.actionType = USERS_IN_A_ROOM;
          data.users = msg.users;
          data.roomId = roomId;
          userStore.deSerialize(data);
          messageStore.newRoom(roomId);
          this.list.set(roomId, msg);
          this.emit('addNewRoom', roomId);
        } else {
          throw new ('ROOMStore : invalid value');
        }
        break;
      }
      case ADD_NEW_USER_PACKET: {
        const roomId = view.getUint32(offset);
        offset += 4;
        const userId = view.getUint32(offset);
        offset += 4;
        var userName = '';
        for (; offset < data.length; offset++) {
          userName += String.fromCharCode(view.getUint8(offset));
        }
        var msg = {
          actionType: ADD_NEW_USER_IN_A_ROOM,
          roomId: roomId,
          userId: userId,
          userName: userName
        };
        userStore.deSerialize(msg);
        break;
      }
      case REMOVE_USER_PACKET: {
          const roomId = view.getUint32(offset);
          offset += 4;
          const userId = view.getUint32(offset);
          offset += 4;
          var msg = {
              actionType: REMOVE_USER,
              roomId: roomId,
              userId: userId
          }
          userStore.deSerialize(msg);
          break;
      }
      case NEW_TEXT_MESSAGE_PACKET: {
          const roomId = view.getUint32(offset);
          if (this.list.get(roomId) == undefined) {
              throw new ("this roomId: %d doesnt exist", roomId);
          }
          offset +=4;
          const timeStamp = view.getFloat64(offset);
          offset += 8;
          const fromId = view.getUint32(offset);
          offset += 4;
          var msg = "";
          for (; offset < data.length; offset++) {
            msg += String.fromCharCode(view.getUint8(offset));
          }
          var m = {
              actionType: NEW_TEXT_MESSAGE,
              userName: userStore.getUserName(roomId, fromId),
              roomId: roomId,
              timeStamp: timeStamp,
              userId : fromId,
              messageLength: msg.length,
              message: msg
          }
          messageStore.deSerialize(m);
          
      }
    }
  }
}
var roomStore = new RoomStore;
window.roomStore = roomStore;
export default roomStore;