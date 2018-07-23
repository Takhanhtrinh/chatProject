import React from 'react';
import ReactDOM from 'react-dom';
import ChatApp from './components/ChatApp'

class Index extends React.Component {
  constructor() {
    super();
    this.state = { connected: true, userName: '', status: '' }
  }
  userNameFromRegisterView(e) {
    if (e == '') {
      this.setState({status: 'Plese enter user name'});
    } else {
      const ws = new WebSocket('ws://localhost:5653');
      ws.onopen = function(event) {
          ws.send("q" + e);
      };
      ws.onmessage = function(event) {
        console.log(e.data);
      };
      ws.onclose = function(event) {
        console.log(e);
      };
      ws.onerror = function(event) {
        console.log(e);
      };
    }
  }
  render() {
    // if (this.state.connected == false) {
    //                return <RegisterView data= {this.userNameFromRegisterView.bind(this)} status = {this.state.status} />;
    //     }
    //     else {
            return (<ChatApp/>);
        // }
    }
}
ReactDOM.render(<Index />, document.getElementById('app'));