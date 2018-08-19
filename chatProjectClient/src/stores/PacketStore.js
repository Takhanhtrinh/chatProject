import {EventEmitter} from 'events';

import SearchRoomContainer from '../components/SearchRoomView/SearchRoomContainer';
import {ACTION_ADD_ROOM, ACTION_ADD_USERS, ADD_ROOM_FAIL, ADD_ROOM_SUCCESS, NETWORK_INIT_FAIL, NETWORK_INIT_SUCCESS, SEND_INIT, SEND_SEARCH_ROOM, SERVER_INIT_PACKET, SERVER_INIT_PACKET_FAIL, SERVER_INIT_PACKET_SUCCESS, SERVER_SEARCH_ROOM_PACKET, SERVER_USER_LEFT_ROOM_PACKET, SERVER_USERS_I_A_ROOM_PACKET, SERVER_USER_ENTER_ROOM_PACKET, ACTION_SEND_MESSAGE, SERVER_SEND_NEW_MESSAGE_PACKET, NEW_TEXT_MESSAGE} from '../Constant';
import InitPacket, { SendMessagePacket } from '../Packet';
import {SearchRoomPacket} from '../Packet';
import searchRoomStore from '../stores/SearchRoomStore'

import {addRoom, addUsersToRoom, userLeftRoom, userEnterRoom} from './Action';
import Dispatcher from './Dispatcher';
import userStore from './UserStore';
import messageStore from './MessageStore';
import roomStore from './RoomStore';

class PacketStore extends EventEmitter {
  constructor(host) {
    super();
    this.host = host;
    this.name = '';
    this.network = null;
  }
  run() {
    this.network = new WebSocket(this.host);
    this.network.binaryType = 'arraybuffer';
    this.network.onopen = this.onopen.bind(this);
    this.network.onerror = this.onerror.bind(this);
    this.network.onclose = this.onclose.bind(this);
    this.network.onmessage = this.onmessage.bind(this);
  }
  onopen(event) {
    console.log('connected');
    this.emit('network_connected');
  }
  onerror(event) {
    console.log('error');
    this.emit('network_error');
  }
  onclose(event) {
    console.log('disconnect');
    this.emit('network_disconnect');
  }
  getState() {
    return this.network.readyState;
  }
  onmessage(event) {
    console.log('new message: ', event.data);
    console.log('message len: ', event.data.length);
    var buffer = new Uint8Array(event.data);
    var view = new DataView(buffer.buffer);
    var offset = 0;
    let type = view.getUint8(offset++);
    switch (type) {
      case SERVER_INIT_PACKET: {
        let status = view.getUint8(offset++);
        if (status == SERVER_INIT_PACKET_SUCCESS) {
          this.emit(NETWORK_INIT_SUCCESS, this.name);
        } else if (status == SERVER_INIT_PACKET_FAIL) {
          var str = '';
          for (var i = 0; i < buffer.length - 2; i++) {
            str += String.fromCharCode(view.getUint8(offset++));
          }
          this.emit(NETWORK_INIT_FAIL, str);
        }
      } break;
      case SERVER_SEARCH_ROOM_PACKET: {
        let status = view.getUint8(offset++);
        if (status == ADD_ROOM_FAIL) {
        } else if (status == ADD_ROOM_SUCCESS) {
          let roomId = view.getUint32(offset, true);
          offset += 4;
          addRoom(searchRoomStore.roomName, roomId);
        }
        break;
      }
      case SERVER_USERS_I_A_ROOM_PACKET: {
        let roomId = view.getUint32(offset, true);
        var data = [];
        offset += 4;
        let totalUsers = view.getUint32(offset, true);
        offset += 4;
        for (var i = 0; i < totalUsers; i++) {
          let nameLength = view.getUint8(offset++);
          var name = '';
          for (var j = 0; j < nameLength; j++) {
            name += String.fromCharCode(view.getUint8(offset++));
          }
          let userId = view.getUint32(offset, true);
          data.push({name: name, id: userId});
          offset += 4;
        }
        addUsersToRoom(roomId, data);
        break;
      }
      case SERVER_USER_LEFT_ROOM_PACKET: {
        let roomId = view.getUint32(offset, true);
        offset += 4;
        let userId = view.getUint32(offset, true);
        offset += 4;
        userLeftRoom(roomId, userId);
        break;
      }
      case SERVER_USER_ENTER_ROOM_PACKET: {
          let roomId = view.getUint32(offset, true);
          offset+= 4;
          let userId = view.getUint32(offset, true);
          offset+= 4;
          var name = "";
          let nameLength = view.buffer.byteLength - offset;
          for (var i =0; i < nameLength; i++) {
            name += String.fromCharCode(view.getUint8(offset++));
          }
          userEnterRoom(roomId, userId, name);
          break;
      }
      case SERVER_SEND_NEW_MESSAGE_PACKET: {
        let roomId = view.getUint32(offset, true);
        offset += 4;
        let fromId = view.getUint32(offset, true);
        offset += 4;
        var bytes = [];
        for (var i = 0; i < 8; i++) {
          bytes.push(view.getUint8(offset++));
        }
        let userName = userStore.getUserName(roomId, fromId);
        console.log("userName: " , userName);
        var timeStamp = this.Int64ToString(bytes.reverse()); 
        var number = new Number(timeStamp);
        let msgId = view.getUint32(offset, true);
        offset += 4;
        let length = view.buffer.byteLength;
        var msg = "";
        for (offset ; offset < length; offset++) {
          msg += String.fromCharCode(view.getUint8(offset));
        }
        console.log("message id : ", msgId);
        let data = {
          msgId: msgId,
          actionType: NEW_TEXT_MESSAGE,
          timeStamp: number.valueOf(),
          userId: fromId,
          roomId: roomId,
          userName: userName,
          messageLength: length,
          message: msg
        }
        Dispatcher.dispatch(data);
        break;
      }
    }
  }
  Int64ToString(bytes) {
    const digits = [];
    bytes.forEach((byte, j) => {
      for(let i = 0; byte > 0 || i < digits.length; i++) {
        byte += (digits[i] || 0) * 0x100;
        digits[i] = byte % 10;
        byte = (byte - digits[i]) / 10;
      }
    });
    return '' + digits.reverse().join('');
  }
  actionHandler(action) {
    switch (action.actionType) {
      case SEND_INIT: {
        let name = action.name;
        var packet = new InitPacket(name);
        this.name = name;
        this.network.send(packet.view.buffer);
        break;
      }
      case SEND_SEARCH_ROOM: {
        let name = action.name;
        var packet = new SearchRoomPacket(name);
        this.network.send(packet.view.buffer);
        roomStore.emit("doneSearching");
        break;
      }
      case ACTION_SEND_MESSAGE: {
        let roomId = action.roomId;
        let message = action.message;
        let sendTo = action.sendTo;
        let packet = new SendMessagePacket(roomId, message, sendTo);
        this.network.send(packet.view.buffer);
        break;

      }
    }
  }
}
var packetStore = new PacketStore('ws://localhost:5653');
Dispatcher.register(packetStore.actionHandler.bind(packetStore));
export default packetStore;