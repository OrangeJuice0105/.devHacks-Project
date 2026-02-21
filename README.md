# OS Arena (Template)

## Backend (C++)
1) Put `httplib.h` and `json.hpp` into `backend/external/`.
2) Build & run:
```bash
cd backend

# On Windows
cmake -S . -B build -G "MinGW Makefiles"

# On Linux
cmake -S . -B build

cmake --build build -j
./build/os_arena
```