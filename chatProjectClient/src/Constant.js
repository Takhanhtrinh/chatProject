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
export const SERVER_INIT_PACKET_SUCCESS = 1;
export const SERVER_INIT_PACKET_FAIL = 2;

// userStore
export const USERS_IN_A_ROOM = 1;
export const ADD_NEW_USER_IN_A_ROOM = 2;
export const ROOM_SELECTED = 3;
export const REMOVE_USER = 4;


// MessageStore
export const NEW_TEXT_MESSAGE = 5;
// for action
export const CURRENT_SELECT_ROOM_ID = "SELECT_ROOM";
export const SEND_INIT = "SEND_INIT";
export const SEND_SEARCH_ROOM = "SEND_SEARCH_ROOM";

export const NETWORK_INIT_FAIL = "INIT_FAIL";
export const NETWORK_INIT_SUCCESS = "INIT_SUCCESS";