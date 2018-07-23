import React from 'react';
import './css/RoomView.css'
export default class Room extends React.Component {
    constructor() {
        super();
    }
    render() {
        return(
            <div className ="roomContainer">
                {this.props.roomName}
            </div>
        );
    }


}