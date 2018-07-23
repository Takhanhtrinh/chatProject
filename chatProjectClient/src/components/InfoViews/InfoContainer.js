import React from 'react'
import './css/InfoView.css'
import { INFO_HEIGHT } from '../../Constant';

export default class InfoContainer extends React.Component {
    render() {
        return(
            <div className ="InfoContainer" style={{height: INFO_HEIGHT}}>
            </div>
        );
    }
}