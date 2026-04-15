## Usage

```gdscript
var params_a := GeomOpsParams3D.new()
params_a.shape     = ...
params_a.transform = ...

var params_b := GeomOpsParams3D.new()
params_b.shape     = ...
params_b.transform = ...

var result := GeomOpsResult3D.new()

if GeomOps3D.closest_to_shape(params_a, params_b, result):
    result.point_a
    result.point_b

if GeomOps3D.closest_to_point(params, point, result):
    ...
```

## Build Instructions

### 1. Clone repository with submodules

```Bash
git clone --recurse-submodules https://github.com/gdpit/godot-geomops.git
cd godot-geomops
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

Move `extension_api.json` into the root of the repository folder `godot-geomops/`.

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