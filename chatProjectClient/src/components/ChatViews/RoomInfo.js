import React from 'react'
import roomStore from '../../stores/RoomStore'
import './css/ChatView.css'
export default class RoomInfo extends React.Component {
    constructor() {
        super();
        this.state = {roomName: ""};
    }
    componentDidMount() {
        roomStore.on('addNewRoom', (id) => {
            const array = roomStore.getList();
            const room = array.get(id);
            var roomName = "#" + room;
            if (room != undefined) {
                this.setState({roomName: roomName});
            }
        });
        roomStore.on('changeSelectedRoom', (id)=> {
            const array = roomStore.getList();
            const room = array.get(id);
            var roomName = "#" + room;
            if (room != undefined) {
                this.setState({roomName: roomName});
            }
        });


    }
    render() {
        return (
            <div className="roomInfoContainer">
                <div id="roomNameOffset" className ="roomName">
                {this.state.roomName}
                </div>
                <div className="removeRoom">


                </div>
            </div>
        );
    }
}