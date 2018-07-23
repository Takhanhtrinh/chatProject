import React from 'react'
import './css/PostView.css'
import MessagePost from './MessagePost'
import ImagePost from './ImagePost'
export default class PostViewContainer extends React.Component {
    constructor(){
        super();
    }

    render() {
        return (
            <div className="PostViewContainer"> 
                <div className = "profileImage"></div>
                    <div className="inline">
                        <div className = "name">Trinh Ta</div>
                         <div className="time"> 15:23
                    </div>
                </div>
                <ImagePost />
        </div>
        );
    }
}