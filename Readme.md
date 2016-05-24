# Node bindings for hfst-ospell

This is a simple work-in-progress library which aims to offer spell checking using [Hfst-ospell](https://github.com/hfst/hfst-ospell) in node.js.

## Install

This library is currently only known to work on my machine. If you are _really_ lucky – and you happen to have `tinyxml2` and `libarchive` installed – you might be able to get it working on your machine, too.

## Development

After you cloned this repository, make sure to also fetch the hfst-ospell sources using `git submodule update --init --recursive`.

You can build the library using `node-gyp configure build`. (Feel free to ignore any warnings on `lib/*` files.)

Use `npm test` to verify the library works on the node side.

## TODO

- [x] Make it compile!
- [ ] Everything with TODO and FIXME in code!
- [ ] Compile tinyxml2 ourselves (it's just one file)
- [ ] Compile libarchive ourselves (it's a truckload of stuff)
