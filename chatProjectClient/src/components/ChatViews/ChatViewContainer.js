import React from 'react'
import './css/ChatView.css'
import InputView from '../InputViews/InputViewContainer'
import { INFO_HEIGHT, MINIMUM_INPUT_HEIGHT, INPUT_PADDING } from '../../Constant';
import RoomInfo from './RoomInfo'
import PostView from '../PostView/PostViewContainer'

export default class ChatViewContainer extends React.Component {
    constructor() {
        super();
        this.state = { textFieldHeight: MINIMUM_INPUT_HEIGHT + INPUT_PADDING};
    }
    textFieldSizeChange(value) {
        this.setState({textFieldHeight: value});
    }
    render() {
        return(
            <div>
                <div className ="roomInfo" style= {{height: 30}}> 
                    <RoomInfo />
                </div>
                <div className ="ChatViewContainer" style ={{height: window.innerHeight - INFO_HEIGHT - this.state.textFieldHeight - 30}}>
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                    <PostView / > 
                 </div>
                <InputView style={{height: MINIMUM_INPUT_HEIGHT + INPUT_PADDING}} sizeChanged= {this.textFieldSizeChange.bind(this)} />
            </div>
        ) ;
    }
}