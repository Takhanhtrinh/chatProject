import React from 'react'

export default class User extends React.Component {

    constructor(probs) {
        super(probs);
        // calculate parent container width
        // this.state = {containerWidth: 0};
        // this.calculateWidth();
    }
    calculateWidth() {
        // const windowWidth = window.innerWidth;
        // // xs
        // var value = 0;
        // if (windowWidth < 576) {

        // value = windowWidth / 4;
        // }
        // // sm
        // else if (windowWidth >= 576 && windowWidth < 768) {
        //      value = windowWidth / 4;

        // }
        // //md
        // else if(windowWidth >= 768 && windowWidth < 992) {
        //    value = windowWidth / 6;

        // }
        // //lg 
        // else if (windowWidth >= 992 && windowWidth < 1140) {
        //      value= windowWidth / 6;

        // }
        // // xl
        // else {
        //     value = windowWidth / 6;

        // }

        // value = Math.round(value);
        // this.state.containerWidth = value;
        // this.setState({containerWidth: value});

    }
    componentDidMount() {
       window.addEventListener("resize", this.windowResize.bind(this));
        
    }
    windowResize() {
        this.calculateWidth();
    }
    render() {
        return (
                    <div className ="nameContainer">
                        <div className="name">@{this.props.userName}</div>
                    </div>
        );
    }
}