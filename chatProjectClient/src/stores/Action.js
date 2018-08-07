import Dispatcher from './Dispatcher'
import { SEND_INIT } from '../Constant';
export function sendInit(name) {
    Dispatcher.dispatch({
        actionType: SEND_INIT,
        name: name
    });
}