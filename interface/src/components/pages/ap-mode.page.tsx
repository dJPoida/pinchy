import * as React from 'react';
import { Visibility, VisibilityOff } from '@material-ui/icons';
import { global } from '../../constants/global.constant';
import { Button } from '@material-ui/core';

export interface APModePageProps { }

/**
 * When providing details to Pinchy in the Access Point mode
 * the form can be in several states.
 */
enum FormState {
  entry = 1,          // User is entering the data
  confirmation = 2,   // Data is about to be submitted and requires confirmation
  submitting = 3,     // Data has been posted to Pinchy
  confirmed = 4,      // Data was received by pinchy
  failed = 5,         // Data could not be posted to pinchy
};


/**
 * Render the Access Point Mode Page
 */
export const APModePage: React.FC = (props: APModePageProps) => {
  const [showPassword, setShowPassword] = React.useState(false);
  
  const [wifiSSID, setWifiSSID] = React.useState(''); // TODO: fetch this from the device?
  const [wifiPassword, setWifiPassword] = React.useState('');
  const [formState, setFormState] = React.useState(FormState.entry);

  /**
   * Fired when the user clicks the Toggle Password Visibility button
   */
  const handleShowPasswordClick = React.useCallback(function handleShowPasswordClick(event: React.MouseEvent<HTMLButtonElement, MouseEvent>) {
    event.preventDefault();
    setShowPassword(!showPassword);
  }, [showPassword, setShowPassword]);


  /**
   * Fired when the user submits their preferred wifi details
   */
  const handleConfirmWiFiDetails = React.useCallback(function handleConfirmWiFiDetails() {

  }, [wifiSSID, wifiPassword]);


  /**
   * Render the form that allows a user to specify their WiFi configuration
   */
  const renderEnterWiFiDetailsForm = React.useCallback(function renderEnterWiFiDetailsForm():JSX.Element {
    return (
      <>
        <div className="row">
          <p>
            Congratulations! You've connected your device directly to Pinchy!
          </p>
          <p>
            Now you need to tell Pinchy how to connect to your local network. Check in with your local IT guru and enter the WiFi details of your local network below.
          </p>
        </div>

        <div className="row">
          <h2>WiFi Configuration</h2>
          <form className="wifi-config" id="wifi_config">
            <div className="field-group">
              {/* SSID */}
              <label htmlFor="input_wifiSSID">SSID</label>
              <input
                className="wifi-ssid"
                type="text"
                id="input_wifiSSID"
                placeholder="SSID"
                onChange={(event: React.ChangeEvent<HTMLInputElement>) => {setWifiSSID(event.currentTarget.value)}}
                value={wifiSSID}
              />

              {/* Password */}
              <label htmlFor="input_wifiPassword">Password</label>
              <input
                className="wifi-password"
                type={showPassword ? 'text' : 'Password'}
                id="input_wifiPassword"
                placeholder="Password"
                onChange={(event: React.ChangeEvent<HTMLInputElement>) => {setWifiPassword(event.currentTarget.value)}}
                value={wifiPassword}
              />
              <Button 
                className="show-password"
                onClick={handleShowPasswordClick}
              >
                {showPassword && <Visibility />}
                {!showPassword && <VisibilityOff />}                
              </Button>
            </div>

            <div className="button-next">
              <Button
                id="button_next"
                className="next success inverted"
                onClick={() => setFormState(FormState.confirmation)}
                disabled={!!!wifiSSID}
              >
                Next
              </Button>
            </div>
          </form>
        </div>
      </>
    );
  }, [showPassword, handleShowPasswordClick, wifiSSID, wifiPassword]);


  /**
   * After the user submits the form we'll just double check they're happy with their
   * supplied configuration prior to sending it to Pinchy.
   */
  function renderConfirmationForm():JSX.Element {
    return (
      <>
        <div className="row">
          <p>
            OK, sweet! Just double check these details before Pinchy attempts to connect to your local network.
          </p>
          <div className="wifi-conf">
            <span>SSID</span>
            <span>{wifiSSID}</span>
            <Button
              className="show-password"
              onClick={handleShowPasswordClick}
            >
              {showPassword && <Visibility />}
              {!showPassword && <VisibilityOff />}                
            </Button>
            <span>Password</span>
            <span>{wifiPassword}</span>
          </div>
          <p>
            If this is all good, click Submit below!
          </p>
          <Button
            id="button_confirm"
            className="confirm success inverted"
            onClick={() => handleConfirmWiFiDetails()}
          >
            Next
          </Button>
        </div>
      </>
    );
  }


  /**
   * Once the user confirms their specified wifi details, we submit them to pinchy.
   * This is a quick step, as we're basically waiting for a response from the device.
   */
  function renderSubmittingDetails():JSX.Element {
    return (
      <>
        <div className="row">
          <p>
            Spokie Dokie. Letting Pinchy know what's up....
          </p>
        </div>
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

        {formState === FormState.entry && renderEnterWiFiDetailsForm()}
        {formState === FormState.confirmation && renderConfirmationForm()}
      </div>
    </div>
  )
}