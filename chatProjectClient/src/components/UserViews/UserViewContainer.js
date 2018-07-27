import './css/UserView.css'

import React from 'react'

import {INFO_HEIGHT} from '../../Constant';
import userStore from '../../stores/UserStore'

import User from './User'

export default class UserViewContainer extends React.Component {
  constructor(props) {
    super(props);
    this.state = { list: [], roomSelected: 0 }
  }
  componentDidMount() {
    userStore.on('roomSelected', (id) => {
      const array = userStore.getList().get(id);
      if (array != undefined) {
        this.setState({list: array, roomSelected: id});
        console.log('from user view container selected roomId: ', id);
      } else {
        throw new ('array is undefinied');
      }
    });
    userStore.on('modifiedList', (id) => {
      if (id == this.state.roomSelected) {
        const array = userStore.getList().get(id);
        this.setState({list: array, roomSelected: id});
      }
    });
  }
  componentDidUpdate() {}
  updateCanvas() {}


  render() {
    var components = [];
    for (var i = 0; i < this.state.list.length; i++) {
      const user = this.state.list[i];
            var e = <User userName = {user.name} id = {user.id} key = {user.id} />
            components.push(e);
        }
        return (
            <div className ="UserViewContainer" style ={{height: window.innerHeight - INFO_HEIGHT}}>
                <div className="inputContainer">
                    <input type = "text" className ="form-control" placeholder="Find a user in this room..." />
                 </div>
                 <div className = "userList" style = {{width: "100%", height: window.innerHeight - INFO_HEIGHT - 53}}>
                     {components}
                 </div>
            </div>
   

        )
    }
  }