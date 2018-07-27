import {EventEmitter} from 'events'
import userStore from './UserStore'
import { NEW_TEXT_MESSAGE, CURRENT_SELECT_ROOM_ID, ROOM_SELECTED } from '../Constant';
import Dispatcher from './Dispatcher'

class MessageStore extends EventEmitter{
    constructor() {
        super();
        // key is roomId
        this.list = new Map();
    }
    getList() {
        return this.list;
    }
    newRoom(id) {
        if (this.list.has(id)) {
            throw new ("this room already exist");
        }
        else {
            this.list.set(id,[]);
        }

    }
    deSerialize(data){
        const actionType = data.actionType;
        switch(actionType) {
            case NEW_TEXT_MESSAGE: {
                const userName = data.userName; 
                if (userName == undefined) {
                    throw new ("userName is null");
                }
                const roomId = data.roomId;
                const timeStamp = data.timeStamp;
                const userId = data.userId;
                const messageLength = data.messageLength;
                const message = data.message;
                console.log("roomid: " + roomId);
                console.log("timestamp: " , timeStamp);
                console.log("userId: ", userId);
                console.log("messageLength: " , messageLength);
                console.log("message: ", message);
                console.log("userName: " , userName);
                if (this.list.has(roomId)) {
                   var array = this.list.get(roomId);
                   
                   const e = {
                       messageType: "text",
                       timeStamp: timeStamp,
                       userId: userId,
                       userName: userName,
                       messageLength: messageLength,
                       message: message,
                   }
                   array.push(e);
                    
                }
                else {
                    var array = [];
                    const e = {
                        messageType: "text",
                       timeStamp: timeStamp,
                       userId: userId,
                       userName: userName,
                       messageLength: messageLength,
                       message: message
                    }
                    array.push(e);
                }
                this.emit("newTextMessage", roomId);
                break;
            }
        }
        
    }
    actionHandler(action) {
        const actionType = action.actionType;
        switch(actionType) {
            case ROOM_SELECTED: {
                const roomId = action.roomId;
                this.emit("changeSelectedRoom", roomId);
                break;
            }
        }

    }
}
var messageStore = new MessageStore;
Dispatcher.register(messageStore.actionHandler.bind(messageStore));
export default messageStore;