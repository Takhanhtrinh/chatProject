import { CLIENT_INIT_PACKET, CLIENT_SEARCH_ROOM_PACKET, CLIENT_SEND_TEXT_MESSAGE_PACKET } from "./Constant";

class Packet {
    constructor(size, type){
        this.type = type;
        this.size = size;
        this.buffer = new Uint8Array(size);
        this.view = new DataView(this.buffer.buffer);
    }
}
 class InitPacket extends Packet {
    constructor(name) {
        super(name.length + 1, CLIENT_INIT_PACKET);
        this.name = name;
        this.serialize();
    }
    serialize() {
        var offset = 0;
        this.view.setUint8(offset++,this.type);
        for (var i = 0 ; i < this.name.length; i++) {
            this.view.setUint8(offset++,this.name.charCodeAt(i));
        }
    }
}
export default InitPacket;
export class SearchRoomPacket extends Packet {
    constructor(name) {
       super(name.length + 1 , CLIENT_SEARCH_ROOM_PACKET); 
       this.name = name;
       this.serialize();
    }
    serialize() {
        var offset = 0;
        this.view.setUint8(offset++, this.type);
        for (var i = 0 ; i < this.name.length; i++) {
            this.view.setUint8(offset++,this.name.charCodeAt(i));
        }
    }
}
export class SendMessagePacket extends Packet {
    constructor(roomId, message, sendTo) {
        super(1 + 4 + 4 + 4 + message.length + sendTo.length * 4, CLIENT_SEND_TEXT_MESSAGE_PACKET);
        this.roomId = roomId;
        this.message = message;
        this.sendTo = sendTo;
        this.serialize();
    }
    serialize() {
        var offset = 0;
        this.view.setUint8(offset++, this.type);
        this.view.setUint32(offset, this.roomId,true);
        offset += 4;
        this.view.setUint32(offset, this.message.length, true);
        offset += 4;
        for (var i = 0 ; i < this.message.length ; i++) {
            this.view.setUint8(offset++, this.message.charCodeAt(i));
        }
        this.view.setUint32(offset, this.sendTo.length, true);
        offset += 4
        for ( var i = 0 ; i < this.sendTo.length; i++) {
            this.view.setUint32(offset, this.sendTo[i], true);
            offset+= 4;
        }
    }
}