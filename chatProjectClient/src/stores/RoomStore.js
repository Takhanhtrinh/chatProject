import {EventEmitter} from 'events'

import {JOIN_A_ROOM_RESPOND, USERS_IN_A_ROOM} from '../Constant';
import userStore from '../stores/UserStore'

class RoomStore extends EventEmitter {
  constructor() {
    super();
    this.list = new Map();
  }
  createSample() {
    var buffer = [];
    var offset = 0;
    buffer[offset++] = 1;
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
    buffer[offset++] = 75;

    return buffer;
  }
  createSample1() {
    var buffer = [];
    var offset = 0;
    buffer[offset++] = 1;
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
  getList() {
    return this.list;
  }
  deSerialize(data) {
    var uint8Array = new Uint8Array(data);
    var arrayBuffer = uint8Array.buffer;

    var view = new DataView(arrayBuffer);

    var offset = 0;
    const type = view.getUint8(offset++);
    switch (type) {
      case JOIN_A_ROOM_RESPOND: {
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
          this.list.set(roomId, msg);

          this.emit('addNewStore', roomId);
          break;
        } else {
          throw new ('ROOMStore : invalid value');
        }
      }
    }
  }
}
var roomStore = new RoomStore;
window.roomStore = roomStore;
export default roomStore;