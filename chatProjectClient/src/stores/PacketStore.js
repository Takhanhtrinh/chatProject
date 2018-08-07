import {EventEmitter} from 'events';
import { SEND_INIT, SERVER_INIT_PACKET, SERVER_INIT_PACKET_SUCCESS, NETWORK_INIT_SUCCESS, SERVER_INIT_PACKET_FAIL, NETWORK_INIT_FAIL, SEND_SEARCH_ROOM } from '../Constant';
import InitPacket from '../Packet';
import Dispatcher from './Dispatcher';

class PacketStore extends EventEmitter {
  constructor(host) {
    super();
    this.host = host;
    this.name = "";
    this.network = null;
  }
  run() {
    this.network = new WebSocket(this.host);
    this.network.binaryType = 'arraybuffer';
    this.network.binaryType = 'arraybuffer';
    this.network.onopen = this.onopen.bind(this);
    this.network.onerror = this.onerror.bind(this);
    this.network.onclose = this.onclose.bind(this);
    this.network.onmessage = this.onmessage.bind(this);
  }
  onopen(event) {
    console.log("connected");
    this.emit("network_connected");
  }
  onerror(event) {
    console.log("error");
    this.emit("network_error");
  }
  onclose(event) {
      console.log("disconnect");
      this.emit("network_disconnect");

  }
  getState() {
      return this.network.readyState;
  }
  onmessage(event) {
    console.log("new message: ", event.data );
    console.log("message len: ", event.data.length);
    var buffer = new Uint8Array(event.data);
    var view = new DataView(buffer.buffer);
    var offset = 0;
    let type = view.getUint8(offset++);
    switch(type) {
        case SERVER_INIT_PACKET: {
            let status = view.getUint8(offset++);
            if (status == SERVER_INIT_PACKET_SUCCESS) {
                this.emit(NETWORK_INIT_SUCCESS, this.name);
            }
            else if (status == SERVER_INIT_PACKET_FAIL) {
                var str = "";
                for (var i = 0; i < buffer.length - 2; i++) {
                    str += String.fromCharCode(view.getUint8(offset++));
                }
                this.emit(NETWORK_INIT_FAIL, str);
            }
        }
        break;
    }
  }
  actionHandler(action) {
      switch(action.actionType) {
          case SEND_INIT: {
              let name = action.name;
              var packet = new InitPacket(name);
              this.name = name;
              this.network.send(packet.view.buffer);
              break;
          }
      }

  }
}
var packetStore = new PacketStore('ws://localhost:5653');
Dispatcher.register(packetStore.actionHandler.bind(packetStore));
export default packetStore;