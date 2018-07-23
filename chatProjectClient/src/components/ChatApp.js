
import React from 'react'
import InfoBar from './InfoViews/InfoContainer'
import RoomList from './RoomViews/RoomListContainer'
import ChatView from './ChatViews/ChatViewContainer'
import UserView from './UserViews/UserViewContainer'
import 'bootstrap/dist/css/bootstrap.min.css'
import './ChatAppStyle.css'
import {MINIMUM_INPUT_HEIGHT, MAXIMUM_INPUT_HEIGHT, INPUT_PADDING, INFO_HEIGHT} from '../../src/Constant'

export default class ChatApp extends React.Component {
    constructor() {
        super();
        this.state = {
            height: 0,
        };
    }
    windowResize() {
        this.setState({height: window.innerHeight});
    }
    componentWillMount() {
        window.addEventListener("resize",this.windowResize.bind(this));
    }
  render() {
        return(
           <div className = "AppContainer"> 
                <div className="row no-gutters">
                    <div className ="col-12">
                        <InfoBar />
                    </div>
                     <div className ="col-3 col-md-2 col-lg-2 col-xl-2">
                       <RoomList />
                    </div>
                     <div className ="col-6 col-md-8 col-lg-8 col-xl-8">
                        <ChatView />
                    </div>
                     <div className ="col-3 col-md-2 col-lg-2 col-xl-2">
                       <UserView  />
                    </div>
                </div>
            </div>
        );
    }
}