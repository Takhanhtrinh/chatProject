import React from 'react' 
import './css/InputView.css'
import { INPUT_PADDING, MAXIMUM_INPUT_HEIGHT } from '../../Constant';
export default class InputViewContainer extends React.Component {

    constructor() {
        super();
        this.textField = React.createRef();
    }

    handleOnInput() {
        var value = this.textField.current.value;
        if (value.length > 0 ) {
            if (value[value.length - 1 ] == '\n') {
                this.textField.current.value = '';
                value = this.textField.current.value;
            }
        }
        if (document.getElementsByClassName("tf")[0].clientHeight < MAXIMUM_INPUT_HEIGHT || value.length =='') {
            document.getElementsByClassName("tf")[0].setAttribute("style", "height: auto");
              document.getElementsByClassName("tf")[0].setAttribute("style", "height: " + this.textField.current.scrollHeight + "px;") ;
            document.getElementsByClassName("InputContainer")[0].setAttribute("style", "height: " + this.textField.current.scrollHeight + INPUT_PADDING + "px;");
        this.props.sizeChanged(this.textField.current.scrollHeight + INPUT_PADDING);
        }

    }
    componentDidMount() {
        this.handleOnInput();
    }
    render() {
        return (
            <div className="InputContainer">
                <textarea placeholder="Enter a message..." rows="1"  ref={this.textField}className = "form-control tf" onInput={this.handleOnInput.bind(this)} > </textarea> 
            </div>
        );
    }

}