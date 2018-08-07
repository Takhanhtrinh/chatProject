import 'bootstrap/dist/css/bootstrap.min.css'
import './ChatAppStyle.css'

import React from 'react'

import { INFO_HEIGHT, INPUT_PADDING, MAXIMUM_INPUT_HEIGHT, MINIMUM_INPUT_HEIGHT, NETWORK_INIT_SUCCESS, NETWORK_INIT_FAIL } from '../../src/Constant'

import ChatView from './ChatViews/ChatViewContainer'
import InfoBar from './InfoViews/InfoContainer'
import InitViewContainer from './InitView/InitViewContainer'
import RoomList from './RoomViews/RoomListContainer'
import UserView from './UserViews/UserViewContainer'
import packetStore from '../stores/PacketStore'
import SearchRoomContainer from '../components/SearchRoomView/SearchRoomContainer'

export default class ChatApp extends React.Component {
    constructor() {
        super();
        this.state = {
            height: window.innerHeight,
            width: window.innerWidth,
            init: false,
            name: "",
            popup: false
        };
        packetStore.run();
    }
    componentDidMount() {
        packetStore.on(NETWORK_INIT_SUCCESS, (name) => {
            this.setState({
                init: true,
                name: name
            });
        });
    }
    windowResize() {
        this.setState({
            height: window.innerHeight,
            width: window.innerWidth
        });
    }
    componentWillMount() {
        window.addEventListener('resize', this.windowResize.bind(this));
    }

    popupClickedOutside() {
        console.log("outside");
        this.setState({
            popup:false
        })
    }
    addRoomButtonClicked() {
        this.setState({
            popup:true
        })
        console.log("vj");
    }
    render() {
        if (this.state.init == false) {
            return (
                <InitViewContainer />
            );
        }
        else {
            var popup = "none";
            if (this.state.popup == true) {
                popup = "block";
            }
            return (
                <div className="AppContainer">
                    <div className="popup" onClick={this.popupClickedOutside.bind(this)} style={{ display: popup }} >
                    </div>
                    <div style={{
                        display: popup
                    }}>
                        <SearchRoomContainer />
                    </div>
                    <div className="row no-gutters">
                        <div className="col-12">
                            <InfoBar />
                        </div>
                        <div className="col-3 col-md-2 col-lg-2 col-xl-2">
                            <RoomList popup={this.addRoomButtonClicked.bind(this)} userName={this.state.name} />
                        </div>
                        <div className="col-6 col-md-8 col-lg-8 col-xl-8">
                            <ChatView />
                        </div>
                        <div className="col-3 col-md-2 col-lg-2 col-xl-2">
                            <UserView />
                        </div>
                    </div>
                </div>
            );
        }
    }
}