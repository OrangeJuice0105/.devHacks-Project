Write-Host "Building frontend..."

Set-Location frontend
npm install
npm run build
Set-Location ..

Write-Host "Preparing backend/public..."

if (!(Test-Path "backend/public")) { 
    New-Item -ItemType Directory -Path "backend/public" | Out-Null 
}

Copy-Item -Recurse -Force "frontend/dist/*" "backend/public/"
Write-Host "Bundled frontend into backend/public"

Write-Host "Rebuilding backend..."
Set-Location backend

if (Test-Path "build") {
    Remove-Item -Recurse -Force "build"
}

Start-Sleep 5

cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j

Set-Location ..

Write-Output "Bundle complete! Move to backend to run the executable"
Write-Output ""
Write-Output "Run the server with:"
Write-Output "  ./build/os_arena.exe  (Windows)"
Write-Output "  ./build/os_arena      (Linux/macOS)"