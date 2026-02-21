#!/usr/bin/env bash

set -e 

echo "Building frontend..."
cd frontend
npm install
npm run build
cd ..

echo "Preparing backend/public..."
mkdir -p backend/public
rm -rf backend/public/*

echo "Copying built frontend into backend/public..."
cp -r frontend/dist/* backend/public/

echo "⚙️ Rebuilding backend..."
cd backend

rm -rf build

cmake -S . -B build
cmake --build build

cd ..

echo "Bundle complete!"
echo ""
echo "Run the server with:"
echo "  ./backend/build/os_arena.exe  (Windows)"
echo "  ./backend/build/os_arena      (Linux/macOS)"