import * as React from 'react';
import * as ReactDOM from 'react-dom';

import './scss/ap-mode.scss';

import { HelloWorld } from './components/HelloWorld';

ReactDOM.render(
  <HelloWorld compiler="TypeScript" framework="React" />,
  document.getElementById("example")
);