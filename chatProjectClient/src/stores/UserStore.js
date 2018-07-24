import Store from './Store'
class UserStore extends Store {
    constructor() {
        super();
    }
    deSerialize(data) {
        console.log(data.roomId);
        console.log(data.users);

    }
    newMessage() {

    }
}
var userStore = new UserStore;
export default userStore;