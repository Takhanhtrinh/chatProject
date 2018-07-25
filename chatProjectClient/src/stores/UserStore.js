import {EventEmitter} from 'events'
import Dispatcher from './Dispatcher'
import { USERS_IN_A_ROOM, ADD_NEW_USER_IN_A_ROOM, ROOM_SELECTED, REMOVE_USER } from '../Constant';
class UserStore extends EventEmitter {
    constructor() {
        super();
        this.list = new Map();
        // room id users 
    }
    getList() {
        return this.list;
    }
    deSerialize(data) {
        const actionType = data.actionType;
        console.log(data);
        switch(actionType) {
            case USERS_IN_A_ROOM: {
                const roomId = data.roomId;
                const users= data.users;
                var j =  [];
                for (var i = 0; i < users.length; i++) {
                    const user = users[i];
                    const userId = user.id;
                    const userName = user.name;
                    j.push({
                        id: userId,
                        name: userName
                    })

                }
                this.list.set(roomId, j);
                this.emit("roomSelected", roomId);
                break;
            }
            case ADD_NEW_USER_IN_A_ROOM: {
                const roomId = data.roomId;
                const userId = data.userId;
                const userName = data.userName;
                if (this.list.get(roomId) != undefined) {
                    const e = {id: userId, name:userName};
                    this.list.get(roomId).push(e);
                    this.emit("addNewUser", roomId);
                } else {
                    throw new ("this room doest exist");
                }
                break;
            }
            case ROOM_SELECTED : {
                const roomId = data.roomId;
                this.emit("roomSelected", roomId);
                break;
            }
            case REMOVE_USER: {
                const roomId = data.roomId;
                const userId = data.userId;
                if (this.list.get(roomId) != undefined) {
                    var array  = this.list.get(roomId);
                    var index = -1;
                    for (var i = 0 ; i < array.length; i++) {
                       if (array[i].id == userId) {
                           index = i;
                           break;
                       } 
                    }
                    if (index != -1){
                           array.splice(index,1);
                        this.list.set(roomId, array);
                        this.emit("roomSelected", roomId);
                    }
                    else {
                    }
                    break;
                } else {
                    throw new("CANT FIND THIS ROOMID");
                }
                    
            };

        }

    }
}
var userStore = new UserStore;
window.userStore = userStore;
Dispatcher.register(userStore.deSerialize.bind(userStore));
export default userStore;