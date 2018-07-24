import {EventEmitter} from 'events'

export default class Store extends EventEmitter {
    constructor() {
        super();
        this.list = [];
    }
    getList() {
        return this.list;
    }

    deSerialize(data){}
    newMessage(){}

}