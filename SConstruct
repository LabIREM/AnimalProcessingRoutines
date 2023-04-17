import os
import sys

def setCppFlags(env):
    env.AppendUnique(CPPFLAGS = ['/EHsc', '/MD', '/Ox'])

def setLibPaths(env):
    """
    here we define the all required:
      - ITK lib paths
      - boost lib paths
      - MATLAB paths, which used only for SegmentatedIlagesAnalysis (SIA)  code
    it's depend of local installation  
    """
    itkPath = 'D:/LabIREM/codes/c/ITK'
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
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/Smoothing/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/LabelMap/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/ImageStatistics/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Filtering/ImageFeature/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Numerics/Statistics/include'])
    env.AppendUnique(CPPPATH = [itkPath + '/Modules/Registration/Common/include'])

    boostPath = 'C:/Program Files/PackageManagement/NuGet/Packages/boost.1.81.0/lib/native/include'
    env.AppendUnique(CPPPATH = [boostPath])
    env.AppendUnique(LIBPATH = ['C:/Program Files/PackageManagement/NuGet/Packages/boost_program_options-vc143.1.81.0/lib/native'])
    env.AppendUnique(LIBPATH = ['C:/Program Files/PackageManagement/NuGet/Packages/boost_filesystem-vc143.1.81.0/lib/native'])
 
    env.AppendUnique(CPPPATH = ['C:/Program Files/MATLAB/R2018b/extern/include']) # if you don't use SIA comment this line 

def setLibs(env):
    """
    here we have to enumerate all used libraries provided by ITK,
    the lib label varies between OS 
    """
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

def setMatlab(env):
    """
    used only for SIA compilation, depens of your local installation
    """
    env.AppendUnique(LIBPATH = ['C:/Program Files/MATLAB/R2018b/extern/lib/win64/microsoft'])

    libs = [
        'SimulinkBlock.lib',
        'asynciocoder.lib',
        'gpu.lib',
        'gpumexbinder.lib',
        'libMatlabCppSharedLib.lib',
        'libMatlabDataArray.lib',
        'libMatlabEngine.lib',
        'libcovrt.lib',
        'libemlrt.lib',
        'libeng.lib',
        'libfixedpoint.lib',
        'libmat.lib',
        'libmex.lib',
        'libmwComputeMetric.lib',
        'libmwblas.lib',
        'libmwblascompat32.lib',
        'libmwboxfilter.lib',
        'libmwboxfilter3.lib',
        'libmwbwdistEDT.lib',
        'libmwbwdistEDTFT.lib',
        'libmwbwdistEDTFT_tbb.lib',
        'libmwbwdistEDT_tbb.lib',
        'libmwbwlookup.lib',
        'libmwbwlookup_tbb.lib',
        'libmwbwpackc.lib',
        'libmwbwpackctbb.lib',
        'libmwbwunpackc.lib',
        'libmwbwunpackctbb.lib',
        'libmwcannythresholding_tbb.lib',
        'libmwcgir_construct.lib',
        'libmwcgxert.lib',
        'libmwcoder_CodeInstrHostAppSvc.lib',
        'libmwcoder_CodeInstrTgtAppSvc.lib',
        'libmwcoder_CoderAssumpHostAppSvc.lib',
        'libmwcoder_CoderAssumpTgtAppSvc.lib',
        'libmwcoder_ParamTuningTgtAppSvc.lib',
        'libmwcoder_RTIOStreamHostAppSvc.lib',
        'libmwcoder_RTIOStreamTgtAppSvc.lib',
        'libmwcoder_ToAsyncQueueTgtAppSvc.lib',
        'libmwcoder_XILHostAppSvc.lib',
        'libmwcoder_XILTgtAppSvc.lib',
        'libmwcoder_connectivity_memunit.lib',
        'libmwcoder_target_services.lib',
        'libmwcoderprofile.lib',
        'libmwddist.lib',
        'libmwedgesobelprewitt_tbb.lib',
        'libmwedgethinning_tbb.lib',
        'libmwforegroundDetector.lib',
        'libmwgetnumcores.lib',
        'libmwgrayto16.lib',
        'libmwgrayto8.lib',
        'libmwgrayxform.lib',
        'libmwgrayxform_tbb.lib',
        'libmwimfilter.lib',
        'libmwimlincomb.lib',
        'libmwimlincomb_tbb.lib',
        'libmwimreconstruct.lib',
        'libmwimregionalmax.lib',
        'libmwimterp2d.lib',
        'libmwint16touint16.lib',
        'libmwint32touint32.lib',
        'libmwint8touint8.lib',
        'libmwintlut.lib',
        'libmwintluttbb.lib',
        'libmwipp.lib',
        'libmwippfilter.lib',
        'libmwippgeotrans.lib',
        'libmwippmedianfilter.lib',
        'libmwippreconstruct.lib',
        'libmwjpegreader.lib',
        'libmwlapack.lib',
        'libmwmathutil.lib',
        'libmwmorphop_binary.lib',
        'libmwmorphop_binary_tbb.lib',
        'libmwmorphop_flat.lib',
        'libmwmorphop_flat_tbb.lib',
        'libmwmorphop_nonflat.lib',
        'libmwmorphop_nonflat_tbb.lib',
        'libmwmorphop_ocv.lib',
        'libmwmorphop_packed.lib',
        'libmwocvgpumex.lib',
        'libmwocvmex.lib',
        'libmwordfilt2.lib',
        'libmwpolyfun.lib',
        'libmwremap.lib',
        'libmwremaptbb.lib',
        'libmwrgb2gray_tbb.lib',
        'libmwrgb2hsv_tbb.lib',
        'libmwrtiostreamutils.lib',
        'libmwsdi_raccel.lib',
        'libmwservices.lib',
        'libmwsigstream.lib',
        'libmwsimulink.lib',
        'libmwsl_AsyncioQueue.lib',
        'libmwsl_fileio.lib',
        'libmwsl_log_load_blocks.lib',
        'libmwsl_services.lib',
        'libmwsl_sfcn_cov_bridge.lib',
        'libmwsl_sfcn_loader.lib',
        'libmwsl_simtarget_core.lib',
        'libmwsl_simtarget_instrumentation.lib',
        'libmwsl_solver_rtw.lib',
        'libmwslexec_parallel.lib',
        'libmwslexec_simbridge.lib',
        'libmwslexec_simlog.lib',
        'libmwslio_clients.lib',
        'libmwslio_core.lib',
        'libmwsltest_raccel.lib',
        'libmwstringutil.lib',
        'libmwtbbhist.lib',
        'libmwuint16toint16.lib',
        'libmwxilcomms.lib',
        'libmwxilcomms_rtiostream.lib',
        'libmwxilutils.lib',
        'libmwxilutils_sl.lib',
        'libmx.lib',
        'libut.lib',
        'mclbase.lib',
        'mclcommain.lib',
        'mclmcr.lib',
        'mclmcrrt.lib',
        'mclxlmain.lib',
        'ne_mli.lib',
        'ocrutils.lib',
        'physmod_common_external_library_kernel.lib',
        'physmod_common_foundation_core_performance.lib',
        'physmod_common_foundation_core_util.lib',
        'physmod_common_foundation_sli_util.lib',
        'rtwcg.lib',
        'sf_runtime.lib'
    ]

    env.AppendUnique(LIBS = libs)

def setDefines(env):
    env.AppendUnique(CPPDEFINES=['ITK_STATIC'])

env = Environment()
# env = Environment(MSVC_SCRIPT = 'custom.bat', MSVC_VERSION = '14.3')
# env.sourceDir = 'D:/RomaBio/temp/CellTracking'
env.sourceDir = os.getcwd()

setCppFlags(env)
setLibPaths(env)
setLibs(env)
setMatlab(env)#comment this line if you don't want compile SIA code
setDefines(env)
env['CCCOMSTR'] = ''

Export('env')

debug = ARGUMENTS.get('debug', False)
buildDir = 'debug' if debug else 'release'

SConscript('CellTracking/source/SConscript', variant_dir=buildDir + '/ct', duplicate = 0)
SConscript('JunctionTracking/source/SConscript', variant_dir=buildDir + '/jt', duplicate = 0)
SConscript('SegmentedImageAnalysis/source/SConscript', variant_dir=buildDir + '/sa', duplicate = 0)
SConscript('EpitheliumProjection/zmapGeneration/SConscript', variant_dir=buildDir + '/zg', duplicate = 0)
SConscript('EpitheliumProjection/zmapProjection/SConscript', variant_dir=buildDir + '/zp', duplicate = 0)