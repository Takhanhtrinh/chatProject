import React from 'react';
import './style.css'
import ResultView from './ResultView'
import searchRoomStore from '../../stores/SearchRoomStore'
import { sendSearchRoom } from '../../stores/Action';
export default class SearchRoomContainer extends React.Component {
    constructor() {
        super();
        this.state = {
            searchField: "",
        };
        this.textField = React.createRef();
    }
    componentDidMount() {
    }
    componentWillUnmount() {
    }
    searchClicked() {
        if (this.state.searchField.length > 0) {
            sendSearchRoom(this.state.searchField);
            this.textField.current.value = '';
            this.props.closePopup();
        }
        else {
            this.textField.current.focus();
        }
    }
    textfieldChanged(e) {
        this.state.searchField = e.target.value;
        searchRoomStore.roomName = e.target.value;
    }

    render() {
        return (
            <div style={{ display: this.props.display }}>
                <div className="searchRoomContainer row">
                    <input autoFocus="true" ref={this.textField} onInput={this.textfieldChanged.bind(this)} className="form-control search-input col-8 col-md-9" type="text" placeholder="Enter a room name to search..." />
                    <button onClick={this.searchClicked.bind(this)} className="btn btn-default search-btn offset-sm-1 offset-md-1 col-3 col-md-2">Search</button>
                    <div className="underline col-12"> </div>
                </div>
            </div >
        )
    }
}