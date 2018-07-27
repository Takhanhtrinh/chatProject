import React from 'react'

export default class MessagePost extends React.Component {
    constructor() {
        super();
    }
    render() {
        return (
            <div>
                <div className="profileImage"></div>
                <div className="inline">
                    <div className="name">{this.props.userName}</div>
                    <div className="time"> {this.props.timeStamp}
                    </div>
                </div>
                <div className="message">
                    {this.props.message}
                </div>
            </div>

        );
    }

}