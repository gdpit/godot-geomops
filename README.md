## Build Instructions

### 1. Clone repository with submodules

```Bash
git clone --recurse-submodules https://github.com/aterray/godot-distutils.git
cd godot-distutils
```

If you already cloned:

```Bash
git submodule update --init --recursive
```

### 2. Generate `extension_api.json`

Run this in a terminal using the same Godot version you target:

```Bash
godot.exe --dump-extension-api
```

This will generate `extension_api.json` in the current working directory.

Move `extension_api.json` into the root of the repository folder `godot-distutils/`.

### 3. Create and activate a Python virtual environment (recommended)

```Bash
python -m venv venv
```

Activate it:

```Bash
venv\Scripts\activate
```

### 4. Install build dependency

```Bash
pip install scons
```

### 5. Build `godot-cpp`

```Bash
cd godot-cpp
scons platform=<platform> custom_api_file=../extension_api.json
cd ..
```

### 6. Build the addon

```Bash
scons platform=<platform>
```