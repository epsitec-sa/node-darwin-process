#include <node_api.h>
#include <libproc.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int is_process_running(int pid, const char* expectedName) {
    int res = 0;
    char procName[1024] = {0};

    if (!pid)
        return 0;

    // If errno == ESRCH, process with pid is not running
    // if errno is EPERM == 1: Operation not permitted, we know that
    //   the process exists, then we continue
    res = kill(pid, 0);
    if (res && errno != EPERM)
        return 0;

    // No process with the pid has been found (ret == 0), or an error occurred if
    // it's not 1
    res = proc_name(pid, procName, sizeof(procName));
    if (res <= 0)
        return 0;

    // Process with pid has been found, but what about its name?
    return strstr(procName, expectedName) ? 1 : 0;
}

napi_value IsProcessRunning(napi_env env, napi_callback_info info) {
    napi_status status;

    size_t argc = 2;
    napi_value argv[2];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (status != napi_ok || argc < 2) {
        napi_throw_error(env, NULL, "Expected arguments: pid (number), name (string)");
        return nullptr;
    }

    int32_t pid;
    status = napi_get_value_int32(env, argv[0], &pid);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid pid");
        return nullptr;
    }

    char expectedName[PROC_PIDPATHINFO_MAXSIZE];
    size_t nameLen;

    status = napi_get_value_string_utf8(env, argv[1], expectedName, sizeof(expectedName), &nameLen);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid process name");
        return nullptr;
    }

    int result = is_process_running(pid, expectedName);

    napi_value napiResult;
    status = napi_create_int32(env, result, &napiResult);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to create result");
        return nullptr;
    }

    return napiResult;
}

napi_value Init(napi_env env, napi_value exports) {
    napi_value fn;
    napi_status status;

    status = napi_create_function(env, NULL, 0, IsProcessRunning, NULL, &fn);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to create function");
        return nullptr;
    }

    status = napi_set_named_property(env, exports, "isProcessRunning", fn);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to export function");
        return nullptr;
    }

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)