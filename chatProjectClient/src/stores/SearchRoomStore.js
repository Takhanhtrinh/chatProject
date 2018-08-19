import {EventEmitter} from 'events'

class SearchRoomStore extends EventEmitter {
    constructor() {
        super();
        // roomName current search
        this.roomName = "";
    }
}
var searchRoomStore = new SearchRoomStore();
export default searchRoomStore;