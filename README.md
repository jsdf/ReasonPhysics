# reason-physics

A simple 2d physics engine written in ReasonML. Heavily inspired by [Coffee Physics](https://github.com/soulwire/Coffee-Physics).

See the [in-browser demo](https://jsdf.github.io/ReasonPhysics/) and the [demo source](https://github.com/jsdf/ReasonPhysics/blob/master/reason-physics/src/DemoGravity.re).

## to build the react demo

```bash
cd react-demo
yarn start # bsb watch mode
yarn server # local devserver

# or
yarn prod # build to docs/ dir
```

## to build the reprocessing demo

```
cd reprocessing
yarn sync # hard copies the reason-physics dep to node_modules. yarn link doesn't work because bsb-native uses absolute paths

# web
yarn watch:web
yarn server

# native
yarn build:native && yarn start:native
```
