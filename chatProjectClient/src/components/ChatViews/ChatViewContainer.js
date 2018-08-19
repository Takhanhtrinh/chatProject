import React from 'react'
import './css/ChatView.css'
import InputView from '../InputViews/InputViewContainer'
import { INFO_HEIGHT, MINIMUM_INPUT_HEIGHT, INPUT_PADDING } from '../../Constant';
import RoomInfo from './RoomInfo'
import PostView from '../PostView/PostViewContainer'
import messageStore from '../../stores/MessageStore'
import roomStore from '../../stores/RoomStore'

export default class ChatViewContainer extends React.Component {
    constructor() {
        super();
        this.state = { 
            textFieldHeight: MINIMUM_INPUT_HEIGHT + INPUT_PADDING,
            currentRoomId: -1,
            list: []
        };
    }
    getCurrentRoomId() {
        return this.state.currentRoomId;
    }
    componentDidMount() {
        roomStore.on("addNewRoom", (id) =>{
            console.log("chatview roomid ", id);
            messageStore.newRoom(id);
            this.setState({currentRoomId: id, list: []});

        });
        messageStore.on("newTextMessage", (id)=> {
            if (this.state.currentRoomId != id) return;
            const array = messageStore.getList().get(id);
            console.log("array ne" ,array);
            if (array != undefined) {
                var newList = [];
                for (var i = 0; i < array.length; i++) {
                    const e = <PostView messageType = {array[i].messageType} timeStamp = {array[i].timeStamp} key = {array[i].msgId} userId = {array[i].userId} userName ={array[i].userName} messageLength = {array[i].messageLenth} message = {array[i].message} />
                    newList.push(e);
                }
                this.setState({
                    currentRoomId: id,
                    list: newList
                })
                console.log("new text message ",id);

            }
            else {
                throw new ("ChatViewContainer array is null");
            }

        });

        roomStore.on("changeSelectedRoom", (id)=> {
            console.log("from chat view container current roomId:   ", id );
            const array = messageStore.getList().get(id);
            if (array != undefined) {
                var newList = [];
                for (var i = 0; i < array.length; i++) {
                    const e = <PostView messageType = {array[i].messageType} timeStamp = {array[i].timeStamp} key = {array[i].timeStamp} userId = {array[i].userId} userName ={array[i].userName} messageLenth = {array[i].messageLenth} message = {array[i].message} />
                    newList.push(e);
                }
                this.setState({
                    currentRoomId: id,
                    list: newList
                })

            }
            else {
                throw new ("ChatViewContainer array is null");
            }

        });
    }
    textFieldSizeChange(value) {
        this.setState({textFieldHeight: value});
    }
    render() {
        return(
            <div>
                <div className ="roomInfo" style= {{height: 53}}> 
                    <RoomInfo />
                </div>
                <div className ="ChatViewContainer" style ={{height: window.innerHeight - INFO_HEIGHT - this.state.textFieldHeight - 53}}>
                    {this.state.list}
                 </div>
                <InputView currentRoomId = {this.getCurrentRoomId.bind(this)} style={{height: MINIMUM_INPUT_HEIGHT + INPUT_PADDING}} sizeChanged= {this.textFieldSizeChange.bind(this)} />
            </div>
        ) ;
    }
}