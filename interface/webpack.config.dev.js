const merge = require('webpack-merge');
const path = require('path');
const fs = require('fs');

const CopyWebpackPlugin = require('copy-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');

const packageJson = require('./package.json');
const baseConfig = require('./webpack.config.base');
const entryPoints = require('./webpack.entrypoints');
const appVersionSuffix = packageJson.version.replace(/\./g, '-');

// The dev config is dependent on emulating Pinchy's onboard config
// Throw an exception if the developer hasn't copied and populated
// `pinchy.example.json` to `pinchy.json` yet.
if (!fs.existsSync('./pinchy.json')) {
  throw new Exception('Development Mode requires access to a `pinchy.json` file. Please copy and populate the `pinchy.example.json` and try again.');
}
const pinchyJson = require('./pinchy.json');

module.exports = merge.smart(baseConfig, {
  mode: 'development',
  
  // Enable sourcemaps for debugging webpack output.
  devtool: 'source-map',
  
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
              configFile: path.resolve(__dirname, 'tsconfig.dev.json'),
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

      // Compile SCSS to CSS using the MiniCSS Extract Plugin and Hot Loader when in development mode
      {
        test: /\.(scss|css)$/,
        use: [
          'css-hot-loader',
          {
            loader: MiniCssExtractPlugin.loader,
          },
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
    // Copy the webpack source files to the dist directory
    new CopyWebpackPlugin({
      patterns: [
        {
          from: path.resolve(__dirname, 'node_modules/react/umd', 'react.development.js'),
          to: 'js',
          toType: 'dir',
        },
        {
          from: path.resolve(__dirname, 'node_modules/react-dom/umd', 'react-dom.development.js'),
          to: 'js',
          toType: 'dir',
        },
        path.resolve(__dirname, 'pinchy.json'),
      ],
    }),

    // Use HTML Webpack Plugin to copy and populate our html templates
    ...entryPoints.map((entryPoint) => new HtmlWebpackPlugin({
      template: path.resolve(__dirname, 'src', `${entryPoint.fileName}.html`),
      filename: `${entryPoint.fileName}.html`,
      chunks: [entryPoint.name],
      hash: true,
      templateParameters: {
        appTitle: 'Pinchy (Dev)',
        appVersionSuffix,
        jsSuffix: 'development'
      },
    })),

    // Extract the compiled CSS for each entry point into an external file
    // This makes de-bugging and development easier
    new MiniCssExtractPlugin({
      filename: 'css/[name].css',
    }),
  ],

  // When running the application in development mode using "yarn dev",
  // the application will be served on localhost using the following config
  devServer: {
    contentBase: path.join(__dirname, 'dist'),
    compress: true,
    writeToDisk: true,
    port: 80
  }
});
