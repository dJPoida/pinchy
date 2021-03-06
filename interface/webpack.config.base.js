const path = require('path');
const webpack = require('webpack');
const CopyWebpackPlugin = require('copy-webpack-plugin');

const packageJson = require('./package.json');
const entryPoints = require('./webpack.entrypoints');


// Flatten the entry points to an entry object which we can pass to the webpack config
const entry = (() => {
  const result = {};

  entryPoints.forEach((entryPoint) => {
    result[entryPoint.name] = [
      path.resolve(__dirname, 'src', `${entryPoint.fileName}.tsx`),
    ];
  });
  return result;
})();


module.exports = {
  mode: 'none',
  
  entry,

  resolve: {
    // Add '.ts' and '.tsx' as resolvable extensions.
    extensions: ['.ts', '.tsx', '.js']
  },
  
  plugins: [
    // Provide some global variables to the client
    new webpack.DefinePlugin({
      // Let's give our front end application the ability to render the version number
      __VERSION__: JSON.stringify(packageJson.version),
    }),

    // Copy public assets to our dist folder
    new CopyWebpackPlugin({
      patterns:[
        {
          from: path.join(__dirname, './src', 'public'),
          to: path.join(__dirname, './dist'),
          toType: 'dir',
        }
      ]
    }),
  ],

  // When importing a module whose path matches one of the following, just
  // assume a corresponding global variable exists and use that instead.
  // This is important because it allows us to avoid bundling all of our
  // dependencies, which allows browsers to cache those libraries between builds.
  // Given the small amount of bandwidth on the ESP32, we want the user's browser to
  // cache this as much as possible
  externals: {
    'react': 'React',
    'react-dom': 'ReactDOM'
  }
};