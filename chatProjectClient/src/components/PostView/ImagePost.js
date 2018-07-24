import React from 'react'
import { MAXIMUM_PIXEL_ART_SIZE } from '../../Constant';
export default class ImagePost extends React.Component {
    constructor() {
        super();
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
        const PADDING = 15;
        const imageWidth = 50;
        value = Math.round(windowWidth - value * 2 - PADDING * 3 - imageWidth);
        this.state.containerWidth = value;
        this.setState({containerWidth: value});

    }
    componentDidMount() {
        this.drawCanvas();
       window.addEventListener("resize", this.windowResize.bind(this));
    }
    newImage(canvas) {
        var image = new Image();
        image.src = canvas.toDataURL();
        return image;
    }
    drawCanvas() {
        const ctx = this.refs.canvas.getContext('2d');
        var color1 = {r: 157, g: 80, b: 187};
        var color2 = {r: 6, g: 17, b: 97 };
        var color3 = {r:18, g: 255, b: 247 };
        var color4= {r: 255, g: 255, b: 255};
        var color = [color1, color2 ,color3, color4];
        const size = MAXIMUM_PIXEL_ART_SIZE;
        var imageData = ctx.createImageData(size, size);
        for (var i =0; i < imageData.data.length ; i+= 4) {
               const temp = color[Math.floor(Math.random() * 4)];
                imageData.data[i] = temp.r;
                imageData.data[i + 1] = temp.g;
                imageData.data[i + 2] = temp.b;
                imageData.data[i + 3] = 255;
        }
        var temp = this.scaleImageData(imageData, Math.ceil(this.state.containerWidth / size));
        ctx.putImageData(temp, 0,0, 0,0,this.state.containerWidth, this.state.containerWidth);
        

    }
     scaleImageData(imageData, scale) {
        const ctx = this.refs.canvas.getContext('2d');
        var scaled = ctx.createImageData(imageData.width * scale, imageData.height * scale);
        var subLine = ctx.createImageData(scale, 1).data
        for (var row = 0; row < imageData.height; row++) {
            for (var col = 0; col < imageData.width; col++) {
                var sourcePixel = imageData.data.subarray(
                    (row * imageData.width + col) * 4,
                    (row * imageData.width + col) * 4 + 4
                );
                for (var x = 0; x < scale; x++) subLine.set(sourcePixel, x*4)
                for (var y = 0; y < scale; y++) {
                    var destRow = row * scale + y;
                    var destCol = col * scale;
                    scaled.data.set(subLine, (destRow * scaled.width + destCol) * 4)
                }
            }
        }
    
        return scaled;
    }
    windowResize() {
        this.calculateWidth();
        this.drawCanvas();
    }
    render() {
        return (
            <div className="imagePost">
                <canvas ref="canvas" width={this.state.containerWidth} height={this.state.containerWidth}> </canvas>
            </div>
        );
    }
}