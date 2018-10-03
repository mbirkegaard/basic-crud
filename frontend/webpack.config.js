const path = require("path");

module.exports = {
  entry: "./lib/js/src/Index.bs.js",
  mode: process.env.NODE_ENV === "production" ? "production" : "development",
  output: {
    path: path.join(__dirname, "public"),
    filename: "bundle.js"
  },
  devServer: {
    historyApiFallback: {
      index: "index.html"
    },
    contentBase: "public/",
    inline: true,
    port: 3000
  }
};
