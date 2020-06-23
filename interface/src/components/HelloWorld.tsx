import * as React from 'react';

export interface HelloWorldProps { compiler: string; framework: string; }

export const HelloWorld = (props: HelloWorldProps) => <h1>Hello from {props.compiler} and {props.framework}</h1>