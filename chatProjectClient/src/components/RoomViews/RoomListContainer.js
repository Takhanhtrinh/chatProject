import React from 'react'
import './css/RoomView.css'
import 'bootstrap/dist/css/bootstrap.min.css'
import {INFO_HEIGHT} from '../../Constant'
import RoomStore from '../../stores/RoomStore'
import Room from './Room'
export default class RoomListContainer extends React.Component {
    constructor() {
        super();
        this.state = {list: []};
            const array = RoomStore.getAll();
            this.state = {list: array.map((d) => <Room key = {d.id}id={d.id} roomName={d.message}/>)};
    }
    componentWillMount() {
        RoomStore.on("change", ()=> {
            const array = RoomStore.getAll();
            this.state = {list: array.map((d) => <Room id={d.id} roomName={d.message}/>)};
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