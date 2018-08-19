import React from 'react'
import './css/PostView.css'
import MessagePost from './MessagePost'
import ImagePost from './ImagePost'
export default class PostViewContainer extends React.Component {
    constructor() {
        super();
    }

    render() {
        var com;
        if (this.props.messageType == "text") {
            com = <MessagePost key={this.props.timeStamp} messageLength={this.props.messageLength} message={this.props.message} userId={this.props.userId} userName={this.props.userName} timeStamp={this.props.timeStamp} />

        }
        else {
            com = <ImagePost />
        }
        return (
            <div className="PostViewContainer">

                {com};
            </div>
        );
    }
}