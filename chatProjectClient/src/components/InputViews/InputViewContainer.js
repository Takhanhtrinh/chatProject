import React from 'react'
import './css/InputView.css'
import { INPUT_PADDING, MAXIMUM_INPUT_HEIGHT } from '../../Constant';
import { userSendText } from '../../stores/Action';
import roomStore from '../../stores/RoomStore'
export default class InputViewContainer extends React.Component {

    constructor() {
        super();
        this.textField = React.createRef();
        const windowSize = window.innerWidth;
        // xs
        var value = 0;
        if (windowSize < 576) {
            value = windowSize / 4;
            value *= 2;
        }
        // sm
        else if (windowSize >= 576 && windowSize < 768) {
            value = windowSize / 4;
            value *= 2;
        }
        //md
        else if (windowSize >= 768 && windowSize < 992) {
            value = windowSize / 6;
            value *= 4;
        }
        //lg 
        else if (windowSize >= 992 && windowSize < 1140) {
            value = windowSize / 6;
            value *= 4;
        }
        // xl
        else {
            value = windowSize / 6;
            value *= 4;
        }

        const numberOfButton = 3;
        const paddingToParentDiv = 15;
        const paddingBetweenButton = 5;
        const buttonWidth = 32;
        const v = value - paddingToParentDiv * 2 - buttonWidth * numberOfButton - (paddingBetweenButton * 2) * numberOfButton;
        this.state = { textFieldWidth: v };
    }
    calculateTextFieldWidth() {
        const windowSize = window.innerWidth;
        // xs
        var value = 0;
        if (windowSize < 576) {
            value = windowSize / 4;
            value *= 2;
        }
        // sm
        else if (windowSize >= 576 && windowSize < 768) {
            value = windowSize / 4;
            value *= 2;
        }
        //md
        else if (windowSize >= 768 && windowSize < 992) {
            value = windowSize / 6;
            value *= 4;
        }
        //lg 
        else if (windowSize >= 992 && windowSize < 1140) {
            value = windowSize / 6;
            value *= 4;
        }
        // xl
        else {
            value = windowSize / 6;
            value *= 4;
        }

        const numberOfButton = 3;
        const paddingToParentDiv = 15;
        const paddingBetweenButton = 5;
        const buttonWidth = 32;
        const v = value - paddingToParentDiv * 2 - buttonWidth * numberOfButton - (paddingBetweenButton * 2) * numberOfButton;
        this.setState({ textFieldWidth: v });

    }

    windowReisze() {
        this.calculateTextFieldWidth();
    }
    componentDidMount() {
        this.calculateTextFieldWidth();
        this.textField.current.value = "";
        this.resizeTextField();
        window.addEventListener("resize", this.windowReisze.bind(this));
        // done searching focus on the text field, after click search on search room
        roomStore.on("doneSearching",() => {
            this.textField.current.focus();


        });

    }

    handleOnInput() {
        let isNewLine = this.textField.current.value[this.textField.current.value.length - 1] == '\n';
        var value = this.textField.current.value.trim();
        if (value.length == 0) {
            this.textField.current.value = '';
            return;
        }
            if (isNewLine) {
                let message = value;
                 userSendText(message, this.props.currentRoomId(), []);
                 console.log("roomid: " , this.props.currentRoomId());
                this.textField.current.value = '';
                value = this.textField.current.value;
            }
            this.resizeTextField();
    }
    resizeTextField() {
        if (document.getElementsByClassName("tf")[0].clientHeight < MAXIMUM_INPUT_HEIGHT) {
            document.getElementsByClassName("tf")[0].setAttribute("style", "height: auto");
            document.getElementsByClassName("tf")[0].setAttribute("style", "height: " + this.textField.current.scrollHeight + "px;");
            document.getElementsByClassName("InputContainer")[0].setAttribute("style", "height: " + this.textField.current.scrollHeight + INPUT_PADDING + "px;");
            this.props.sizeChanged(this.textField.current.scrollHeight + INPUT_PADDING);
        }

    }
    render() {
        return (
            <div className="InputContainer">
                <div style = {{width: this.state.textFieldWidth + "px", display: "inline-block"}} >
                    <textarea autoFocus id="textInput" placeholder="Enter a message..." rows="1" ref={this.textField} className="tf form-control" onInput={this.handleOnInput.bind(this)}> </textarea>
                </div>
                <div className="buttonInline emotionBtn"> </div>
                <div className="buttonInline pixelBtn"> </div>
                <div className="buttonInline sendToBtn"> </div>
            </div>
        );
    }

}