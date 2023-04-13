import os
import sys

def setCppFlags(env):
    env.AppendUnique(CPPFLAGS = ['/EHsc', '/MD'])
    # env.AppendUnique(CPPFLAGS = ['/FS', '/Zi'])

def setLibPaths(env):
    itkPath = 'D:/RomaBio/temp/itk'
    env.AppendUnique(LIBPATH = [itkPath + '/build/lib/Release'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Core/Common/include'])
    # env.AppendUnique(CPPPATH = [itkPath + '/Modules/Core/Common'])
    env.AppendUnique(CPPPATH = [itkPath + '/build/Modules/Core/Common'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/ThirdParty/VNL/src/vxl/core'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/ThirdParty/VNL/src/vxl/vcl'])
    env.AppendUnique(CPPPATH = [itkPath + '/build/Modules/ThirdParty/VNL/src/vxl/core'])
    env.AppendUnique(CPPPATH = [itkPath + '/build/Modules/ThirdParty/VNL/src/vxl/vcl'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/IO/ImageBase/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/build/Modules/IO/ImageBase'])
    env.AppendUnique(CPPPATH = [itkPath + '/build/Modules/ThirdParty/Eigen3/src'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/ThirdParty/Eigen3/src'])
    env.AppendUnique(CPPPATH = [itkPath + '/build/Modules/ThirdParty/KWSys/src'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Segmentation/ConnectedComponents/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/ImageLabel/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/ImageIntensity/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/ImageFilterBase/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Nonunit/Review/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Core/Transform/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/ImageGrid/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Core/ImageFunction/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/MathematicalMorphology/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/BinaryMathematicalMorphology/include'])

def setLibs(env):
    libs = [
    'hdf5_hl_cpp-static.lib',
    'ITKBiasCorrection-5.3.lib',
    'ITKColormap-5.3.lib',
    'ITKCommon-5.3.lib',
    'ITKConvolution-5.3.lib',
    'ITKDeformableMesh-5.3.lib',
    'ITKDenoising-5.3.lib',
    'ITKDICOMParser-5.3.lib',
    'ITKDiffusionTensorImage-5.3.lib',
    'itkdouble-conversion-5.3.lib',
    'ITKEXPAT-5.3.lib',
    'ITKFastMarching-5.3.lib',
    'ITKFFT-5.3.lib',
    'itkgdcmcharls-5.3.lib',
    'itkgdcmCommon-5.3.lib',
    'itkgdcmDICT-5.3.lib',
    'itkgdcmDSED-5.3.lib',
    'itkgdcmIOD-5.3.lib',
    'itkgdcmjpeg12-5.3.lib',
    'itkgdcmjpeg16-5.3.lib',
    'itkgdcmjpeg8-5.3.lib',
    'itkgdcmMEXD-5.3.lib',
    'itkgdcmMSFF-5.3.lib',
    'itkgdcmopenjp2-5.3.lib',
    'itkgdcmsocketxx-5.3.lib',
    'ITKgiftiio-5.3.lib',
    'itkgtest-5.3.lib',
    'itkgtest_main-5.3.lib',
    'itkhdf5_cpp-static-5.3.lib',
    'itkhdf5_hl-static-5.3.lib',
    'itkhdf5-static-5.3.lib',
    'ITKImageFeature-5.3.lib',
    'ITKImageIntensity-5.3.lib',
    'ITKIOBioRad-5.3.lib',
    'ITKIOBMP-5.3.lib',
    'ITKIOBruker-5.3.lib',
    'ITKIOCSV-5.3.lib',
    'ITKIOGDCM-5.3.lib',
    'ITKIOGE-5.3.lib',
    'ITKIOGIPL-5.3.lib',
    'ITKIOHDF5-5.3.lib',
    'ITKIOImageBase-5.3.lib',
    'ITKIOIPL-5.3.lib',
    'ITKIOJPEG2000-5.3.lib',
    'ITKIOJPEG-5.3.lib',
    'ITKIOLSM-5.3.lib',
    'ITKIOMeshBase-5.3.lib',
    'ITKIOMeshBYU-5.3.lib',
    'ITKIOMeshFreeSurfer-5.3.lib',
    'ITKIOMeshGifti-5.3.lib',
    'ITKIOMeshOBJ-5.3.lib',
    'ITKIOMeshOFF-5.3.lib',
    'ITKIOMeshVTK-5.3.lib',
    'ITKIOMeta-5.3.lib',
    'ITKIOMINC-5.3.lib',
    'ITKIOMRC-5.3.lib',
    'ITKIONIFTI-5.3.lib',
    'ITKIONRRD-5.3.lib',
    'ITKIOPNG-5.3.lib',
    'ITKIOSiemens-5.3.lib',
    'ITKIOSpatialObjects-5.3.lib',
    'ITKIOStimulate-5.3.lib',
    'ITKIOTIFF-5.3.lib',
    'ITKIOTransformBase-5.3.lib',
    'ITKIOTransformHDF5-5.3.lib',
    'ITKIOTransformInsightLegacy-5.3.lib',
    'ITKIOTransformMatlab-5.3.lib',
    'ITKIOVTK-5.3.lib',
    'ITKIOXML-5.3.lib',
    'itkjpeg-5.3.lib',
    'ITKKLMRegionGrowing-5.3.lib',
    'ITKLabelMap-5.3.lib',
    'itklbfgs-5.3.lib',
    'ITKMarkovRandomFieldsClassifiers-5.3.lib',
    'ITKMathematicalMorphology-5.3.lib',
    'ITKMesh-5.3.lib',
    'ITKMetaIO-5.3.lib',
    'itkminc2-5.3.lib',
    'itkNetlibSlatec-5.3.lib',
    'ITKniftiio-5.3.lib',
    'ITKNrrdIO-5.3.lib',
    'itkopenjpeg-5.3.lib',
    'ITKOptimizers-5.3.lib',
    'ITKOptimizersv4-5.3.lib',
    'ITKPath-5.3.lib',
    'ITKPDEDeformableRegistration-5.3.lib',
    'itkpng-5.3.lib',
    'ITKPolynomials-5.3.lib',
    'ITKQuadEdgeMesh-5.3.lib',
    'ITKQuadEdgeMeshFiltering-5.3.lib',
    'ITKRegionGrowing-5.3.lib',
    'ITKRegistrationMethodsv4-5.3.lib',
    'ITKSmoothing-5.3.lib',
    'ITKSpatialObjects-5.3.lib',
    'ITKStatistics-5.3.lib',
    'itksys-5.3.lib',
    'ITKTestKernel-5.3.lib',
    'itktestlib-5.3.lib',
    'itktiff-5.3.lib',
    'ITKTransform-5.3.lib',
    'ITKTransformFactory-5.3.lib',
    'itkv3p_netlib-5.3.lib',
    'itkvcl-5.3.lib',
    'ITKVideoCore-5.3.lib',
    'ITKVideoIO-5.3.lib',
    'itkvnl-5.3.lib',
    'itkvnl_algo-5.3.lib',
    'ITKVNLInstantiation-5.3.lib',
    'ITKVTK-5.3.lib',
    'ITKWatersheds-5.3.lib',
    'itkzlib-5.3.lib',
    'ITKznz-5.3.lib',
    'Advapi32.lib',
    'Shell32.lib'
    ]

    env.AppendUnique(LIBS = libs)

env = Environment()
# env.sourceDir = 'D:/RomaBio/temp/CellTracking'
env.sourceDir = os.getcwd()

setCppFlags(env)
setLibPaths(env)
setLibs(env)
env.AppendUnique(CPPDEFINES=['ITK_STATIC'])
env['CCCOMSTR'] = ''

Export('env')

debug = ARGUMENTS.get('debug', False)
buildDir = 'debug' if debug else 'release'

# SConscript('CellTracking/source/SConscript', variant_dir=buildDir, duplicate = 0)
# SConscript('JunctionTracking/source/SConscript', variant_dir=buildDir, duplicate = 0)
SConscript('SegmentedImageAnalysis/source/SConscript', variant_dir=buildDir, duplicate = 0)