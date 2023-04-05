# Epithelium Projection

Programs for topological map computation of cadherine depth localisation in an epithelium tissue and three dimensions intensity projection based on topological information \
Done by Stéphane U. Rigaud \
v1.0 27/04/2018 

# Usage
## Zmap Generation

Command line parameters: 
* --help : command usage help
* --input : Raw image path
* --output : Output folder path 
* --delta : Delta value (=1)
* --scale : Max downscaling factor (=16)
* --type : Data preprocessing (=raw)
* --peak : Peak type detection (=0)
* --tol : Peak tolerance value (=25)

## Zmap Projection

Command line parameters: 
* --help : command usage help
* --input : Raw image path
* --output : Output folder path 
* --zmap : Zmap image path
* --type : Projection type (=max)
* --shift : z-translation of the topological map (=0)
* --deltaTop : Upper delta value (=-1)
* --deltaBot : Lower delta value (=1)

# Libraries

Required C++ libraries:
* ITK - v4 or higher (https://itk.org)
* Boost 1.64 (https://www.boost.org)
