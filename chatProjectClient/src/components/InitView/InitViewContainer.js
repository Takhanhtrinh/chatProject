import React from 'react';
import './style.css'
import InitPacket from '../../Packet'
import network from '../../Client'
import { sendInit } from '../../stores/Action';
import packetStore from '../../stores/PacketStore'
import { NETWORK_INIT_FAIL } from '../../Constant';
export default class InitViewContainer extends React.Component {
    constructor() {
        super();
        this.state = {
            centerX: 0,
            centerY: 0,
            errorMsg: "",
            network_connected: false,
            name: ""
            
        }
        this.textField = React.createRef();
    }
    componentDidMount() {
        packetStore.on("network_connected",()=>{
            this.setState({
                network_connected: true
            });

        });
        packetStore.on(NETWORK_INIT_FAIL, (msg)=> {
            this.setState( {
                errorMsg: msg
            })
        });
    }
    joinButtonClicked() {
        if (packetStore.getState() == WebSocket.CLOSED || packetStore.getState() == WebSocket.CLOSING){
            packetStore.run();
            return;
        }
        if (this.state.name != "") {
           sendInit(this.state.name); 
        }
        else {
            this.textField.current.focus();
        }
    }
    textFiedChange(e) {
        this.state.name = e.target.value;
    }
    render() {
        return (
            <div className="InitContainer">
                <div className="wrapper">
                    <div className="row">
                        <div className="offset-2 offset-md-5 col-10 col-md-3">
                            <p className="text">Enter Your UserName</p>
                            <input ref = {this.textField} autoFocus onChange={this.textFiedChange.bind(this)} type="text" className="form-control col-10" placeholder="Enter UserName..." maxLength = "31" />
                            <p className="smallText">{this.state.errorMsg}</p>
                            <button onClick= {this.joinButtonClicked.bind(this)} className="btn btn-secondary col-10">JOIN</button>
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}