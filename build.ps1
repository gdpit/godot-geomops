git submodule update --init --recursive

$PythonPath = "Python313"
if (-not (Test-Path $PythonPath)) {
    py -m venv $PythonPath
}

if (-not $Env:VIRTUAL_ENV) {
    Invoke-Expression $(Join-Path $PythonPath "Scripts" "activate")
}

pip install --disable-pip-version-check --quiet --requirement requirements.txt

scons platform=windows

deactivate
