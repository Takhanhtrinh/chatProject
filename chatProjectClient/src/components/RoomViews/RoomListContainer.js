import React from 'react'
import './css/RoomView.css'
import 'bootstrap/dist/css/bootstrap.min.css'
import { INFO_HEIGHT, USERS_IN_A_ROOM, ACTION_ROOM_SELECTED, CURRENT_SELECT_ROOM_ID } from '../../Constant'
import roomStore from '../../stores/RoomStore'
import Room from './Room'
import Dispatcher from '../../stores/Dispatcher'
import userStore from '../../stores/UserStore'
import Search from '@material-ui/icons/Search';
import Add from '@material-ui/icons/Add'
export default class RoomListContainer extends React.Component {
    constructor() {
        super();
        this.state = {
            list: [],
            selectedId: 0,
            open: false
        };
    }
    componentDidMount() {
        roomStore.on("addNewRoom", (id) => {
            const array = roomStore.getList();
            const room = array.get(id);
            if (room != undefined) {
                this.setState({ list: array, selectedId: id });
            console.log("addnewRoomId: " ,id);
            }
            else {
                throw new ("RoomListContainer cant find this roomId");
            }
        });
    }
    setSelectedIndex(index) {
        if (index == this.state.selectedId) return;
        this.setState({ selectedId: index });
        var data = {};
        data.actionType = ACTION_ROOM_SELECTED;
        data.roomId = index;
        Dispatcher.dispatch(data);
        console.log("change id : ", index);
    }
    addButtonClicked() {
        this.props.popup();
    }

    render() {
        var components = [];
        const array = this.state.list;
        for (const [key, value] of array) {
            const isSelected = this.state.selectedId == key;
            var e = <Room setSelectedIndex={this.setSelectedIndex.bind(this)} isSelected={isSelected} name={value} id={key} key={key} />;
            components.push(e);
        }
        return (
            <div className="RoomListContainer" style={{ height: window.innerHeight - INFO_HEIGHT }}>
                <div className="input-container">
                    <input type="text" className="form-control" placeholder="Find a room..." />
                </div>
                <div className="roomList" style={{ height: window.innerHeight - 53 * 2 - INFO_HEIGHT }}>
                    {components}
                </div>
                <div className="row bottomContainer">
                    <div className="myName col-8">
                        <div className="padding">
                            {this.props.userName}
                        </div>
                    </div>
                    <div onClick={this.addButtonClicked.bind(this)} className="createRoom col-4">Add</div>
                </div>
            </div>
        );
    }

}