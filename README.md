# node-darwin-process

Provides low level POSIX API functions concerning process management on macOS.

## Requirements

- macOS (darwin)
- Node.js >= 8.4.0
- node-gyp

## Installation

Install dependencies and build the native addon:

```sh
npm install
npm run build
```

## Usage

```js
const process = require('./module');
```

## Scripts

| Command | Description |
|---|---|
| `npm test` | Run the test suite with Mocha |
| `npm run configure` | Configure the native build |
| `npm run build` | Build the native addon |

## Debugging

The project includes VS Code launch configurations in `.vscode/launch.json`:

- **Run and debug C++ native module with tests** — launches Node with LLDB so you can set breakpoints in the C++ addon source. Requires the [CodeLLDB](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb) extension.
- **Run and debug JS with tests** — runs `npm test` in a Node.js terminal, letting you set breakpoints in JavaScript test code.

Both configurations run the full test suite on launch.
