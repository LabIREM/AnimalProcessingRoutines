# AnimalProcessingRoutines

## Overview

To simplify the compilation of c++ routines used in the Matlab Animal Processing Tools we propose the python based approach using SCons project. Here we describe step by step the compilation processes. Note what we use ITK **static** library build, not dynamic.

## Requirements

* Package Scons version > 4.4, see <https://scons.org/pages/download.html>
  * the simplest way to obtain Scons is using **conda** (see <https://www.anaconda.com/>)

    ```bash
    conda install -c conda-forge scons
    ```

* Visual Studio 2022, Visual Studio 2019 should do the job as well, see <https://visualstudio.microsoft.com/downloads/>
* you wil nedd c++ **boost**, because few routines has abusive implementation using boost
  * open Windows Power Shell (run as Administrator)

    ```bash
    Register-PackageSource -provider NuGet -name nugetRepository -location https://www.nuget.org/api/v2
    ```

    ```bash
    Install-Package boost_filesystem-vc143
    ```

    ```bash
    Install-Package boost_program_options-vc143
    ```

* download ITK <https://itk.org/download/>
* note ITK do not support scons compilation, so install **CMake** GUI <https://cmake.org/download/>
* if you need compile SegmentedImageAnalysis, a priori, Matlab packages should be installed first.
  * System Properties - Advanced - Environment Variables - Edit Environment Variables : add 'C:\Program Files\MATLAB\R2018b\bin\win64'
  
## Project Compilation

### ITK build

we will use STATIC library build

* open **CMake** GUI

  ```bash
  cmake-gui
  ```

  in CMake do :
  * indicate where is downloaded ITK derectorie in **Where is the source code** tab
  * indicate where will be your ***build*** in **Where to build the binaries** tab (usual practice is to create ***build*** derectorie inside the source code folder)
  * if ***build*** derectory does not exist, CMake will propose to create it
  * Now select **Configure**
    * specify the generator for the this project, when CMake ask you : Visual Studio 2019 or Visual Studio 2022
    * verify the options:
      * [ ] BUILD_EXAMPLES uncheck
      * [x] BUILD_STATIC_LIBS option should be check
      * [ ] BUILD_TESTING uncheck
      * CMAKE_CONFIGURATION_TYPES = Release, leaves only ***Release***
      * [x] ITK_BUILD_DEFAULT_MODULES should be check
  * after configuration finished select **Generate**
  * when **Open project**, if **CMake** do not propose the rigth Porject Generator (Visual Studio > 2019):
    * find in ***build*** the file 'ITK.sln'
    * select 'Open with' and choose the rigth version of Visual Studio to build the project  
* in **Visual Studio**, from ALL_BUILD select **build** and wait untill ITK installation finished

### SConstruct file adoptation

in project directory **AnimalProcessingRoutines** edit the file ***SConstruct*** :

* give the path of itk library by setting the variable "itkPath"
* give the path of boost installation
* give the path of matlab installation
* check the libraries labels

### Compile project with Scons

now we can compile the project, from command line in Windows PowerShell or Terminal

````bash
cls; scons
````

wait until Scons compile the project. The executable binaries will be stored in **release** directory following the tree :

```bash
└── release
    ├── ct
    |    └── cellTracking.exe
    ├── jt
    |    └── jonctionTracking.exe
    ├── sa
    |    └── segmentedImageAnalysis.exe
    ├── zg
    |    └── zmapGeneration.exe
    └── zp
         └── zmapProjection.exe  
```  
