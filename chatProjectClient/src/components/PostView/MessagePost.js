import React from 'react'

export default class MessagePost extends React.Component {
    constructor() {
        super();
    }
    render() {
        let date = new Date(this.props.timeStamp);
        return (
            <div>
                <div className="profileImage"></div>
                <div className="inline">
                    <div className="name">{this.props.userName}</div>
                    <div className="time"> {date.getMonth()}/{date.getDate()}/{date.getFullYear()}
                    </div>
                </div>
                <div className="message">
                    {this.props.message}
                </div>
            </div>

        );
    }
}