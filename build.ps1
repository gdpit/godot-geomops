git submodule update --init --recursive


$PythonPath = "Python314"

if (-not (Test-Path $PythonPath)) {
    py -m venv $PythonPath
}

if (-not $Env:VIRTUAL_ENV) {
    Invoke-Expression $(Join-Path $PythonPath "Scripts" "activate")
}

pip install --disable-pip-version-check --quiet scons


$ExtensionApiJson = "extension_api.json"

if (-not (Test-Path $ExtensionApiJson)) {

    Write-Host "extension_api.json not found"

    $godotPath = Read-Host "Enter path to Godot executable (godot.exe)"
    $godotPath = $godotPath.Trim('"', "'")

    if (-not (Test-Path $godotPath)) {
        throw "Godot path does not exist"
    }

    Write-Host "Generating extension API..."

    & $godotPath --dump-extension-api $ExtensionApiJson
}

if (-not (Test-Path "godot-cpp\bin\*")) {

    Write-Host "Building godot-cpp..."

    Push-Location "godot-cpp"

    scons verbose=yes `
        platform=windows `
        custom_api_file="../$ExtensionApiJson"

    Pop-Location
}


scons verbose=yes platform=windows target=template_debug
scons verbose=yes platform=windows target=template_release

deactivate
