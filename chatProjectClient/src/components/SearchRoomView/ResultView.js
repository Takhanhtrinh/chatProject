import React from 'react'
import 'bootstrap/dist/css/bootstrap.min.css'
export default class ResultView extends React.Component {
    constructor() {
        super();
    }
    render() {
        var btnInfo = "Create";
        if (this.props.roomStatus == "Join")
            btnInfo = "Join";
        return (
            <div className="result row">
                    <div className="resultList col-8 col-md-9">#{this.props.roomName}</div>
                    <button className="result-btn btn btn-default col-3 offset-sm-1 col-md-2" >{btnInfo} </button>
                </div>
        )
    }
}