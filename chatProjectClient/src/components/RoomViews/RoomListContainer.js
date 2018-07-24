import React from 'react'
import './css/RoomView.css'
import 'bootstrap/dist/css/bootstrap.min.css'
import {INFO_HEIGHT} from '../../Constant'
import roomStore from '../../stores/RoomStore'
import Room from './Room'
import userStore from '../../stores/UserStore'
export default class RoomListContainer extends React.Component {
    constructor() {
        super();
        this.state = {list: []};
            const array = roomStore.getList();
            this.state = {list: array.map((d) => <Room key = {d.id}id={d.id} roomName={d.roomName}/>)};
    }
    componentWillMount() {
        roomStore.on("change", ()=> {
            const array = roomStore.getList();
            console.log(array);
            var userData= {roomId: array[0].roomId, users:array[0].users};
            userStore.deSerialize(userData);

            // this.state = {list: array.map((d) => <Room id={d.id} roomName={d.message}/>)};
        });
    }
    render() {
        return (
            <div className="RoomListContainer" style = {{height: window.innerHeight - INFO_HEIGHT}}>
                <div className="input-container">
                    <input type ="text" className="form-control" placeholder="Find a room..."/> 
               </div>
               <div className ="roomList" style={{height: window.innerHeight - 80 - INFO_HEIGHT}}>
                    {this.state.list}
               </div>
                <div className="userInfo">

                </div>
            </div>
        );
    }
    
}