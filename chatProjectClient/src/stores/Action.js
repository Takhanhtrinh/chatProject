import {SEND_INIT, SEND_SEARCH_ROOM, ACTION_ADD_ROOM, ACTION_ADD_USERS, ACTION_USER_LEFT_ROOM, ACTION_USER_ENTER_ROOM, ACTION_SEND_MESSAGE} from '../Constant';

import Dispatcher from './Dispatcher'

export function sendInit(name) {
  Dispatcher.dispatch({actionType: SEND_INIT, name: name});
}
export function sendSearchRoom(roomName) {
  Dispatcher.dispatch({actionType: SEND_SEARCH_ROOM, name: roomName})
}
export function addRoom(roomName, id) {
  Dispatcher.dispatch({
    actionType: ACTION_ADD_ROOM,
    name: roomName,
    id: id
  });
}
export function addUsersToRoom(roomId, data){
  Dispatcher.dispatch({
    actionType: ACTION_ADD_USERS,
    roomId: roomId,
    data: data
  })
}
  export function userLeftRoom(roomId, userId) {
    Dispatcher.dispatch( {
      actionType: ACTION_USER_LEFT_ROOM,
      roomId: roomId,
      userId: userId 
    });
  }
  export function userEnterRoom(roomId, userId, name) {
    Dispatcher.dispatch({
      actionType: ACTION_USER_ENTER_ROOM,
      roomId: roomId,
      userId: userId,
      name: name
    }) ;
  }
    export function userSendText(message, roomId, sendTo) {
      Dispatcher.dispatch ({
        actionType: ACTION_SEND_MESSAGE,
        roomId: roomId,
        message: message,
        sendTo: sendTo
      });
    }

