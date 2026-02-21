# OS Arena

This is a simulation website can be run on both front and back ends. 

## Running the simulation directly (frontend route)

Use the provided script (`frontend.sh` for Linux/MacOS and `frontend.ps1` for Windows Powershell). For Linux/MacOS script, make sure that you run `chmod +x frontend.sh` to allow execution first before running this script.

## Building the server (backend route)

### Provided scripts

Use the provided script (`backend.sh` for Linux/MacOS and `backend.ps1` for Windows Powershell). For Linux/MacOS script, make sure that you run `chmod +x backend.sh` to allow execution first before running this script.

After running the scripts, move to `backend` and run the executable by one of the two following commands from there:

```
# Windows:
.\build\os_arena.exe

# MacOS/Linux:
./build/os_arena
```

### Build from source

First, move into `frontend` folder and run the following commands:

```
npm install
npm run build
```

After the webpage is created in the `dist` folder, copy the contents of this folder and transfer to `backend/public` folder. 

After moving the webpages, run the following commands in the `backend` folder to build the server (note that `cmake -S` command depends on the Operating System you are running on):

```
# Must enable MinGW on Windows first:
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug

# Or run normally if running on MacOS/Linux:
cmake -S . -B build

cmake --build build -j
```

Once completed, run the same executable as in the [Provided scripts route](#provided-scripts).
