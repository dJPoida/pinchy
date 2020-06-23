const merge = require('webpack-merge');
const path = require('path');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const { CleanWebpackPlugin } = require('clean-webpack-plugin');
const WebpackShellPlugin = require('webpack-shell-plugin');

const packageJson = require('./package.json');
const baseConfig = require('./webpack.config.base');
const entryPoints = require('./webpack.entrypoints');
const appVersionSuffix = packageJson.version.replace(/\./g, '-');

module.exports = merge.smart(baseConfig, {
  mode: 'production',

  module: {
    rules: [
      // TS and TSX files
      {
        test: /\.ts(x?)$/,
        exclude: /node_modules/,
        use: [
          {
            loader: 'ts-loader',
            options: {
              configFile: path.resolve(__dirname, 'tsconfig.prod.json'),
            },
          }
        ]
      },

      // All output '.js' files will have any sourcemaps re-processed by 'source-map-loader'.
      {
        enforce: 'pre',
        test: /\.js$/,
        loader: 'source-map-loader'
      },

      // Compile SCSS to CSS using the basic css and sass loaders for production mode
      {
        test: /\.(scss|css)$/,
        use: [
          {
            loader: 'css-loader',
            options: {
              sourceMap: true,
            },
          },
          {
            loader: 'sass-loader',
            options: {
              sourceMap: true,
            },
          },
        ],
      },
    ]
  },

  plugins: [
    // Clean the dist directory before performing a production build
    new CleanWebpackPlugin(),

    // Use HTML Webpack Plugin to copy and populate our html templates
    ...entryPoints.map((entryPoint) => new HtmlWebpackPlugin({
      template: path.resolve(__dirname, 'src', `${entryPoint.fileName}.html`),
      filename: `${entryPoint.fileName}.html`,
      chunks: [entryPoint.name],
      hash: true,
      templateParameters: {
        appVersionSuffix,
        jsSuffix: 'production.min'
      },
    })),

    // Copy the webpack source files to the dist directory
    new CopyWebpackPlugin({
      patterns: [
        path.resolve(__dirname, 'node_modules/react/umd', 'react.production.min.js'),
        path.resolve(__dirname, 'node_modules/react-dom/umd', 'react-dom.production.min.js'),
        path.resolve(__dirname, 'pinchy.json'),
      ],
    }),

    // When deploying a production build, the dist directory is copied into the Arduino source `/data` directory
    // This is in preparation for the `ESP32 Sketch Data Upload` utility which requires the contents of the
    // data partition to be located in the sketch/data directory.
    // This has to be done post-build which is why we're using the WebpackShellPlugin instead of CopyWebpackPlugin
    new WebpackShellPlugin({
      onBuildEnd:[
        // Remove the existing contents
        `rm -rf ${path.resolve(__dirname, '../arduino/pinchy/data')}/*`,
        // Create the directory (if it does not yet already exist)
        `test -d ${path.resolve(__dirname, '../arduino/pinchy/data')} || mkdir ${path.resolve(__dirname, '../arduino/pinchy/data')}`,
        // Copy the dist output to the data directory
        `cp -r ${path.resolve(__dirname, './dist')}/* ${path.resolve(__dirname, '../arduino/pinchy/data')}`,
      ]
    })
  ],
});
