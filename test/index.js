const spawn = require("child_process").spawn;
const assert = require("assert");
const lib = require("../");

// Ensure we can run tests without mocha (for debugging native code)
global.describe = (name, fn) => fn();
global.it = (name, fn) => fn();

describe("IsProcessRunning", function () {
  it("Current process should be running", function () {
    const pid = process.pid;
    const processName = "node";
    const result = lib.isProcessRunning(pid, processName);
    console.log(`Self test ${pid} ${processName}: ${result}`);
    assert.strictEqual(result, true);
  });

  it("Fake process should not be running", function () {
    const fakePid = 999999;
    const processName = "node";
    const result = lib.isProcessRunning(fakePid, processName);
    console.log(`Fake PID test ${fakePid} ${processName}: ${result}`);
    assert.strictEqual(result, false);
  });

  it("External process should be running", function () {
    const child = spawn("sleep", ["10"]);
    // Replace with an actual PID and process name of a running process on your system
    const pid = child.pid;
    const processName = "sleep";
    const result = lib.isProcessRunning(pid, processName);
    console.log(`External test ${pid} ${processName}: ${result}`);
    assert.strictEqual(result, true);
  });
});
