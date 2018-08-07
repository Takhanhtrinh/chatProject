import { CLIENT_INIT_PACKET } from "./Constant";

class Packet {
    constructor(size, type){
        this.type = type;
        this.size = size;
        this.buffer = new Uint8Array(size);
        this.view = new DataView(this.buffer.buffer);
    }
}
export default class InitPacket extends Packet {
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