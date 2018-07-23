import React from 'react'

export default class User extends React.Component {

    constructor() {
        super();
        // calculate parent container width
        this.state = {containerWidth: 0};
        this.calculateWidth();
    }
    calculateWidth() {
        const windowWidth = window.innerWidth;
        // xs
        var value = 0;
        if (windowWidth < 576) {

        value = windowWidth / 4;
        }
        // sm
        else if (windowWidth >= 576 && windowWidth < 768) {
             value = windowWidth / 4;

        }
        //md
        else if(windowWidth >= 768 && windowWidth < 992) {
           value = windowWidth / 6;

        }
        //lg 
        else if (windowWidth >= 992 && windowWidth < 1140) {
             value= windowWidth / 6;

        }
        // xl
        else {
            value = windowWidth / 6;

        }

        value = Math.round(value);
        this.state.containerWidth = value;
        this.setState({containerWidth: value});
        console.log("containerWidth: " + this.state.containerWidth );

    }
    componentDidMount() {
       this.drawCanvas(); 
       window.addEventListener("resize", this.windowResize.bind(this));
        
    }
    windowResize() {
        this.calculateWidth();
        this.drawCanvas();
    }
    drawCanvas() {
        const ctx = this.refs.canvas.getContext('2d');
        var color = ["#9D50BB", "#061161", "#12fff7" , "#FFFFFF"];
        for (var i =0; i < 32 ; i++) {
            for (var j = 0; j < 32 ; j++) {
                ctx.fillStyle = color[Math.floor(Math.random() * 4)];
                ctx.fillRect(j * this.state.containerWidth / 32   ,i *this.state.containerWidth / 32,(this.state.containerWidth - 30) / 32, (this.state.containerWidth - 30) / 32);
            }
        }
    }
    render() {
        return (
            <div className="userCanvasContainer">
                <canvas ref="canvas" width={this.state.containerWidth - 30 } height={this.state.containerWidth - 30}> </canvas>
                    <div className ="nameContainer">
                        <div className="name">@TrinhTaasjldaksdfladkjflaksdjflksdjf</div>
                    </div>
            </div>
        );
    }
}