import { EventEmitter } from 'events'
class RoomStore extends EventEmitter {
    constructor() {
        super();
        this.list = [];
        this.sample();
    }
    sample() {
        for (var i = 0 ; i < 300; i++) {
            var message= "#thisIsARoomHowToDoItHAHAHAHAHlkadj flkdsajf lkdsajf ldaksjf ldsajf lksdajf lksadjf lskadjfAHA";
            this.list.push({id: i, message: message});
        }
    }
    getAll() {
        return this.list;
    }
    newMessage(message) {
        const id = Date.now();
        this.list.push({id: id , message: message});
        this.emit("change");
    }
}
var roomStore = new RoomStore;
window.roomStore = roomStore;
export default roomStore;