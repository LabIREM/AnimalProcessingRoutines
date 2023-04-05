# CellTracking

Cell tracking program for segmented images of epithelium \
Done by Raphael Marchand, Boris Guirao, Yûki Goya, and Stéphane U. Rigaud \
v3.1 27/04/2018 

# How to compile `CellTracking`

# 1. Install `double conversion`

ITK uses a double conversion module (cf. https://gitlab.com/inkscape/lib2geom/-/issues/32) :

- Windows : Use `CMake` (https://github.com/google/double-conversion)

- MacOS : Use homebrew to install double-conversion with: `brew install double-conversion`

- Ubuntu : 

# 2. Build and compile ITK

Let's assume the following repository structure : 

````
```bash
└── compiling cell tracking
    ├── ITK_source
    │── ITK_build
    ├── cellTracking_source
    └── cellTracking_build
```
````

## 1.1. Download ITK :

- Version 5.3.0 (20/12/22) : https://itk.org/download/
- Version 4.13.1 (09/08/18) : https://github.com/InsightSoftwareConsortium/ITK/releases/tag/v4.13.1 

Unpack the source code in a folder.

## 1.2. Run ITK CMake

Run the following command inside the `ITK-build` folder :

### Windows :

```bash
cmake -D Module_ITKReview=on -D ITK_USE_SYSTEM_DOUBLECONVERSION=on ../ITK 
```

(Remember that the file path must not go beyond 50 characters)


### MacOS/(Unix ?) :


```bash
cmake -D CMAKE_CXX_COMPILER="/usr/bin/g++" -D Module_ITKReview=on -D ITK_USE_SYSTEM_DOUBLECONVERSION=on ../ITK 
```

### Description of CMake Commands

- To ensure that the C++ compiler used is the one from the system and not from Conda, for example, add the argument `CMAKE_CXX_COMPILER="/usr/bin/g++"`. If you still get an error indicating that the Conda compiler is being used, you can delete the `CmakeCache.txt` file in the build directory. To retrieve the path of the C++ compiler, you can use the command `which g++`.

- The CellTracking project uses the `ITKReview` module. To install the module, add the argument: `-D Module_ITKReview=on`.

- It is necessary to specify that ITK should use the system's double conversion with the command `-D ITK_USE_SYSTEM_DOUBLECONVERSION=on`.

- The argument `../ITK` specifies that we are going to compile the sources located in the subdirectory `ITK` of the parent directory to the current directory.

## Compile ITK


In the same folder, run : 

### Windows 

```bash
MsBuild ITK.sln /t:rebuild 
```

### MacOS

```bash
make 
```

# Build and compile Cell Tracking

## Construction Cell Tracking

Run the following command inside the  `CellTracking-build` folder:


### Windows 

```bash
cmake -D ITK_DIR="`chemin/vers/ITK-build" ../CellTracking 
```

### MacOS/(Unix ?)


```bash
cmake -D CMAKE_CXX_COMPILER="/usr/bin/g++" -D ITK_DIR="`chemin/vers/ITK-build" ../CellTracking 
```



- ### Description of CMake Commands

  - Same as for building ITK, it is necessary to ensure that the C++ compiler used is the one from the system and not from Conda, for example, by adding the argument `CMAKE_CXX_COMPILER="/usr/bin/g++"`.

  - It is necessary to provide the CMake with the path to our ITK build with the command: `-D ITK_DIR="path/to/ITK-build"`, where `path/to/ITK-build` corresponds to the absolute path of ITK if ITK is not in the environment variables. Otherwise, use its name associated with the environment variables.

  - The argument `../CellTracking` specifies that we are going to compile the sources located in the subdirectory `CellTracking` of the parent directory to the current directory.

## Compilation CellTracking

In the same folder, run : 

### Windows 

```bash
MsBuild CellTracking.sln /t:rebuild 
```

### MacOS

```bash
make 
```

**Should be done now ! Enjoy !** 

