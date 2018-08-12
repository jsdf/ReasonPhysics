const path = require('path');
const publicDir = path.join(__dirname, 'public');

const isProd = process.env.NODE_ENV === 'production';

const buildPath = path.join(publicDir, 'build/');

module.exports = {
  entry: './src/Index.bs.js',
  mode: isProd ? 'production' : 'development',
  devServer: {
    contentBase: publicDir,
  },
  output: {
    path: buildPath,
    filename: 'bundle.js',
  },
};
