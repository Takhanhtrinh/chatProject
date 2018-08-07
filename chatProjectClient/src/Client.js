import InitPacket from './Packet'
export default class Client {
  constructor(host) {
    var network = new WebSocket(host);
    this.client = network;
    network = new WebSocket(host);
    network.binaryType = 'arraybuffer';
    network.onopen = function(event) {
        console.log("connected");
    };
    network.onmessage = function(event) {
        console.log(event.data);
    }
    network.onerror = function(event) {
      console.log('error');
    };
    network.onclose = function(event) {
        console.log("closed");
    }
  }
  isReady() {
    return this.client.readyState;
  }
  send(data) {
    this.client.send(data);
  }

}