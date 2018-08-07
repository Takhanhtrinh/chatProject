import React from 'react';
import './style.css'
export default class SearchRoomContainer extends React.Component {
    constructor() {
        super();
    }

    render() {
        return (
            <div className="searchRoomContainer">
                <div className="input-group">
                    <input  className="form-control" type='text' placeholder="Enter a room name to add or join..." />
                    <span className="input-group-btn">
                        <button className="btn btn-default" type="button">Search </button>
                    </span>
                </div>
                <div className="result" style={{ width: window.innerWidth / 2 - 30, height: window.innerHeight / 2 - 60 }}>result </div>
            </div>
        )
    }
}