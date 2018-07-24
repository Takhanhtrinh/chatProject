import Store from './Store'

 class RoomStore extends Store {
    constructor() {
        super();
    }
    createSample() {
        var buffer =[];
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
        buffer[offset++] = 67;

        return buffer;
        
    }
    deSerialize(data) {
        var uint8Array = new Uint8Array(data);
        var arrayBuffer = uint8Array.buffer;

        var view = new DataView(arrayBuffer);
        
        console.log(view.byteLength);
        var offset = 0;
        const type = view.getUint8(offset++);
        const status = view.getUint8(offset++);
        const ROOM_NOT_EXIST = -1;
        const ROOM_EXIST = 1;
        if (status == ROOM_NOT_EXIST ) {
            throw new ("ROOM_NOT_EXIST");
        }
        else if (status == ROOM_EXIST) {
            // type status roomId roomLen roomname totalUser user
            var msg = {
                roomId: 0,
                nameLen: 0,
                name: " ",
                totalUser: 0,
                users: []
             };
            const roomId = view.getUint32(offset);
            msg.roomId = roomId;
            console.log("roomid: " + roomId);
            offset+= 4;
            const nameLen = view.getUint8(offset++);
            msg.nameLen = nameLen;
            var name = new String();
            for (var i = 0; i < nameLen; i++) {
                name += String.fromCharCode(view.getUint8(offset++));
            }
            msg.name = name;
            const totalUser = view.getUint32(offset);
            offset+=4;
            msg.totalUser = totalUser;
            for ( var i = 0; i < totalUser; i++) {
                const userId = view.getUint32(offset);
                offset+= 4;
                const nameLen = view.getUint8(offset++);
                var userName = new String();
                for (var j = 0; j < nameLen ;j++) {
                    userName += String.fromCharCode(view.getUint8(offset++));
                }
                msg.users.push({
                    id: userId,
                    name: userName
                })

            }
            this.list.push(msg);
            this.newMessage();
        }
        else {
            throw new ("ROOMStore : invalid value");
        }

    }
    newMessage() {
        this.emit("change");
    }
}
var roomStore = new RoomStore;
window.roomStore = roomStore;
export default roomStore;