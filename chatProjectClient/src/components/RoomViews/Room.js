import React from 'react';
import './css/RoomView.css'
export default class Room extends React.Component {
    constructor() {
        super();
        this.state = {
            hover: false
        };
    }
    onClickHandler() {
        this.props.setSelectedIndex(this.props.id);
    }
    onMouseEnter() {
        this.setState(
            {
                hover: true
            });
    }
    onMouseLeave() {
        this.setState({
            hover: false
        });
    }
    componentDidMount() {

    }
    
    render() {
        var style = "roomContainer";
        if (this.props.isSelected) {
            style = "roomContainerActive"
        }
        else {
            if (this.state.hover == true) {
                style = "roomContainerHover";
            }
        }
        return(
            <div onMouseLeave = {this.onMouseLeave.bind(this)} onMouseEnter = {this.onMouseEnter.bind(this)} onClick ={this.onClickHandler.bind(this)} className = {style}>
                #{this.props.name}
            </div>
        );
    }


}