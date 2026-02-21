Set-Location frontend
npm run build
Set-Location ..
if (!(Test-Path "backend/public")) { 
    New-Item -ItemType Directory -Path "backend/public" | Out-Null 
}

Remove-Item -Recurse -Force "backend/public/*" -ErrorAction SilentlyContinue
Copy-Item -Recurse -Force "frontend/dist/*" "backend/public/"
Write-Host "Bundled frontend into backend/public"