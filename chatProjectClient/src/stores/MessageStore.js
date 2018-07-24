import Store from './Store' 
import { TEXT_MESSAGE_TYPE, PIXEL_MESSAGE_TYPE, MAXIMUM_PIXEL_ART_SIZE } from '../Constant';

class MessageStore extends Store{
    constructor() {
        super();
    }
    deSerialize(data) {
        var arrayBuffer = new ArrayBuffer(data.length);
        var view = new DataView(arrayBuffer);
        var offset = 0;
        const type = view.getUint8(offset++);
        const messageType = view.getUint8(offset++);
        if (messageType == TEXT_MESSAGE_TYPE) {
            const length = view.getUint16(offset);
            offset+= 2;
            var roomName = " ";
            roomName.repeat(length);
            for (var index = 0; offset < data.length ; offset++) {
                roomName[index++] = view.getUint8(offset);
            }
            var msg = {
                messageType: messageType,
                nameLength: length,
                roomName: roomName
            }
            this.list.push(msg);
        }
        else if(messageType ==  PIXE_MESSAGE_TYPE) {
            var pixel = {"data": []};
            for (var i =0 ; i < MAXIMUM_PIXEL_ART_SIZE; i++) {
                const r = view.getUint8(offset++);
                const g = view.getUint8(offset++);
                const b = view.getUint8(offset++);
                const a = view.getUint8(offset++);
                pixel.data.push({
                   r: r,
                   g: g,
                   b: b, 
                   a: a
                })

            }
            this.list.push({
                messageType: messageType,
                data: pixel.data
            })
            
        }
        else {
            throw new("MESSAGE STORE: cant find this message type");
        }
        this.newMessage();

    }
    newMessage() {
        this.emit("change");
    }

}
var messageStore = new MessageStore;
export default messageStore;