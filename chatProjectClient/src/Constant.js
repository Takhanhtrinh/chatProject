export const INFO_HEIGHT = 40;
export const MINIMUM_INPUT_HEIGHT = 20;
export const INPUT_PADDING = 20;
export const MAXIMUM_INPUT_HEIGHT = 150;
export const MAXIMUM_PIXEL_ART_SIZE = 128;
export const TEXT_MESSAGE_TYPE = 1;
export const PIXEL_MESSAGE_TYPE = 2;

//packet type
export const JOIN_A_ROOM_RESPOND_PACKET = "a".charCodeAt(0);
export const ADD_NEW_USER_PACKET = 'o'.charCodeAt(0);
export const REMOVE_USER_PACKET = 'i'.charCodeAt(0);
export const NEW_TEXT_MESSAGE_PACKET = 'p'.charCodeAt(0);
export const CLIENT_INIT_PACKET = 'q'.charCodeAt(0);
export const SERVER_INIT_PACKET = 'Q'.charCodeAt(0);
export const CLIENT_SEARCH_ROOM_PACKET = 'e'.charCodeAt(0);
export const SERVER_SEARCH_ROOM_PACKET = 'E'.charCodeAt(0);
export const SERVER_USERS_I_A_ROOM_PACKET = 'A'.charCodeAt(0);
export const SERVER_USER_LEFT_ROOM_PACKET = 'O'.charCodeAt(0);
export const SERVER_USER_ENTER_ROOM_PACKET = 'o'.charCodeAt(0);
export const CLIENT_SEND_TEXT_MESSAGE_PACKET = 't'.charCodeAt(0);
export const SERVER_SEND_NEW_MESSAGE_PACKET = 'u'.charCodeAt(0);
export const SERVER_INIT_PACKET_SUCCESS = 1;
export const SERVER_INIT_PACKET_FAIL = 2;
export const ADD_ROOM_SUCCESS = 1;
export const ADD_ROOM_FAIL= 2;



// MessageStore
export const NEW_TEXT_MESSAGE = 5;
// for action
export const CURRENT_SELECT_ROOM_ID = "SELECT_ROOM";
export const SEND_INIT = "SEND_INIT";
export const SEND_SEARCH_ROOM = "SEND_SEARCH_ROOM";
export const ACTION_SEND_MESSAGE = "ACTION_SEND_MESSAGE";
export const ACTION_ADD_ROOM = "ADD_ROOM";
export const ACTION_ADD_ROOM_FAIL = "ADD_ROOM_FAIL";
export const ACTION_ADD_USERS = "ACTION_ADD_USERS";
export const ACTION_USER_LEFT_ROOM = "ACTION_USER_LEFT_ROOM";
export const ACTION_USER_ENTER_ROOM = "ACTION_USER_ENTER_ROOM";
export const ACTION_ROOM_SELECTED = "ACTION_ROOM_SELECTED";
export const NETWORK_INIT_FAIL = "INIT_FAIL";
export const NETWORK_INIT_SUCCESS = "INIT_SUCCESS";
//https://stackoverflow.com/questions/45455979/byte-array-to-uint64-as-a-string
