# LosOj — C++17 Online Judge System

## Architecture

Two independent servers, both C++17 HTTP servers using [cpp-httplib](https://github.com/yhirose/cpp-httplib):

- **`ojServer/`** — Frontend: port 8080, MVC pattern (OjController/OjModel/OjView).
  Routes: `GET /all_questions`, `GET /question/{id}`, `POST /judge/{id}`.
  Load-balances across compile servers via `conf/serviceHost.conf`.
  Question storage: either MySQL (`MYSQL_MODEL`) or files (`FILE_MODEL`), toggled by `#define`.

- **`compileServer/`** — Backend: ports 8081-8083, accepts code at `POST /compile`.
  Pipeline: write source to temp file → `fork()` + `execlp("g++", ...)` → `fork()` + `setrlimit()` + `exec`.
  Error signals mapped to Chinese descriptions (SIGSEGV → 段错误, SIGXCPU → CPU超时 etc.).

Shared library in `common/`: `LosFile`, `LosPath`, `LosTime`, `LosLog`, `LosStr`.

## Build

Uses **vcpkg** at `/home/losangelous/LosAngelous/dev/vcpkg/` for all dependencies.

```sh
# ojServer
make build       # incremental build (cmake --build build + cp)
make rebuild     # cmake configure + build
make clean       # rm -rf build
make run         # ./OjServer

# compileServer — same targets, binary is CompileServer
```

VCPKG_ROOT is set in per-directory `makefile` files, not in env.

## Formatting

`.clang-format`: LLVM style, Allman braces, 4-space indent, 100 cols, left-aligned `*`.

## Dependencies

| Server | Dependencies |
|--------|-------------|
| ojServer | Boost, inja, nlohmann_json, jsoncpp, libmysql |
| compileServer | jsoncpp, httplib |

## Frontend

Ace editor in browser (`static/one.js`), split-pane layout, per-question code persistence in localStorage.
Templates rendered server-side with inja (`template/all_questions.html`, `template/one_question.html`).

## Tests

No test framework. Manual test executables in `test/` dir (g++ compiled by hand).

## Caveats

- `DEBUG_TEST` is `#define`d in `losLog.h` — debug logging always on.
- `makefile` at repo root is empty (listed in `.gitignore`).
- Question data path: `questions/questions.list` (file mode) or MySQL table `oj_questions` (MySQL mode).
