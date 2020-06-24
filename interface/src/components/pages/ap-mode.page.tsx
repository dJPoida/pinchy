import * as React from 'react';
import { global } from '../../constants/global.constant';
import { Visibility, VisibilityOff } from '@material-ui/icons';

export interface APModePageProps { }

/**
 * Render the Access Point Mode Page
 */
export const APModePage: React.FC = (props: APModePageProps) => {
  const [showPassword, setShowPassword] = React.useState(false);


  /**
   * Fired when the user clicks the Toggle Password Visibility button
   */
  const handleShowPasswordClick = React.useCallback(function toggleShowPassword(event: React.MouseEvent<HTMLButtonElement, MouseEvent>) {
    event.preventDefault();
    setShowPassword(!showPassword);
  }, [showPassword, setShowPassword]);


  /**
   * Render the form that allows a user to specify their WiFi configuration
   */
  const renderWiFiForm = React.useCallback(function renderWiFiForm():JSX.Element {
    return (
      <>
        <div className="row">
          <p>
            Congratulations! You've connected your device directly to Pinchy!
          </p>
          <p>
            Now you need to tell Pinchy how to connect to your local network. 
          </p>
        </div>

        <div className="row">
          <h2>WiFi Configuration</h2>
          <form className="wifi-config" id="wifi_config">
            <div className="field-group">
              {/* SSID */}
              <label htmlFor="input_wifiSSID">SSID</label>
              <input type="text" id="input_wifiSSID" placeholder="SSID" className="wifi-ssid"/>

              {/* Password */}
              <label htmlFor="input_wifiPassword">Password</label>
              <input type={showPassword ? 'text' : 'Password'} id="input_wifiPassword" placeholder="Password" className="wifi-password"/>
              <button className="show-password" onClick={handleShowPasswordClick}>
                {showPassword && <Visibility />}
                {!showPassword && <VisibilityOff />}                
              </button>
            </div>

            <div className="button-next">
              <button id="button_next" className="next success inverted">Next</button>
            </div>
          </form>
        </div>
      </>
    );
  }, [showPassword, handleShowPasswordClick]);


  /**
   * After the user submits the form we'll just double check they're happy with their
   * supplied configuration prior to sending it to Pinchy.
   */
  function renderAboutToConnect():JSX.Element {
    return (
      <>
      </>
    );
  }  


  return (
    <div className="page ap-mode">
      <div className="container">
        
        <div className="row">
          <img className="logo" src="img/app_icon.svg"></img>
          <h1 className="title">Pinchy</h1>
          <span>{`v${global.VERSION}`}</span>
        </div>

        {renderWiFiForm()}
      </div>
    </div>
  )
}