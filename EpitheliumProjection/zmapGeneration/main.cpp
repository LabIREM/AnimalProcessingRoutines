
// std
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>

// boost
#include <boost/program_options.hpp>

// itk
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDerivativeImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkInvertIntensityImageFilter.h"

// external
#include "itkVarianceImageFilter.h"

// my includes
#include "itkMultiscaleVolumeToTopologicalDepthMapFilter.h"
#include "itkImageProjectionFromTopologicalMapFilter.h"




int main ( int argc, char ** argv )
{

    /** Benchmarking **/
    clock_t tStart;
    tStart = clock();

    /** Program variables delcaration **/
    std::string inputName, outputName, type;
    int delta, scale, peak;
    double tolerance;
    
    /** Commande line definition and default values **/
    boost::program_options::options_description desc ("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("input,in", boost::program_options::value<std::string>(&inputName), "Input file name")
    ("output,out", boost::program_options::value<std::string>(&outputName), "Output file name")
    ("delta,d", boost::program_options::value<int>(&delta)->default_value(1), "Delta (default=1)")
    ("scale,k", boost::program_options::value<int>(&scale)->default_value(16), "Scaling factor (default=16)")
    ("type,t", boost::program_options::value<std::string>(&type)->default_value("raw"), "Data type - raw (default), variance (var), derivative (der)")
    ("peak,p", boost::program_options::value<int>(&peak)->default_value(0), "Peak detection - max (default), first (1), second (2)")
    ("tol,l", boost::program_options::value<double>(&tolerance)->default_value(50), "Peak tolerance (default=25)");
    
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << "Compiled : " << __DATE__ << " at " << __TIME__ << std::endl;
        std::cout << desc << "\n";
        return EXIT_SUCCESS;
    }

    /** Typedef and typeame declarations **/
    typedef unsigned short PixelType;
    typedef itk::Image< PixelType, 3 > VolumeType;
    typedef itk::Image< PixelType, 2 > ImageType;
    typedef itk::Image< float, 3 > FloatVolumeType;
    typedef itk::ImageFileReader< VolumeType > VolumeReaderType;
    typedef itk::ImageFileWriter< ImageType > ImageWriterType;
    typedef itk::MedianImageFilter< VolumeType, VolumeType > VolumeMedianFilterType;
    typedef itk::MultiscaleVolumeToTopologicalDepthMapFilter< VolumeType, ImageType > VolumeDataToTopologicalMapFilterType;    
    typedef itk::VarianceImageFilter< VolumeType, VolumeType > VarianceFilterType;
    typedef itk::DerivativeImageFilter< VolumeType, FloatVolumeType > DerivativeFilterType;
    typedef itk::InvertIntensityImageFilter< FloatVolumeType, FloatVolumeType > InvertIntensityFilterType;
    typedef itk::RescaleIntensityImageFilter< FloatVolumeType, VolumeType > IntensityRescaleFilterType;







    /** Load input data 
     *  inputName - string variable provided by the user
    **/
    VolumeReaderType::Pointer inputReader = VolumeReaderType::New();
    inputReader->SetFileName(inputName);
    try
    {
        inputReader->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
        std::cout << std::endl;
        std::cerr << "Error during input reading ... " << std::endl;
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }








    /** Data preprocessing 
     *  Modify the input data in order to facilitate the detection of the topological plan.
     *  It will have a direct impact on the speed and quality of the detection.
     *  Three options available:
     *  raw,r - we do not modify the data (default)
     *  var,v - we use the local 2d intensity variation of the signal
     *  der,d - we use the invert of first derivative of the signal along the z-axis
    **/
    enum { RAW = 'r', VAR = 'v', DER = 'd' };
    VolumeType::Pointer data;
    switch (type[0])
    {
    case RAW:
    {
                data = inputReader->GetOutput();
                break;
    }
    case VAR:
    {
                VarianceFilterType::Pointer varianceFilter = VarianceFilterType::New();
                //FilterWatcher filterWatch(varianceFilter);
                varianceFilter->SetInput(inputReader->GetOutput()); // set the input image
                VolumeType::SizeType radius;
                radius.Fill(10);
                radius[2] = 0;
                varianceFilter->SetRadius(radius);
                try
                {
                    varianceFilter->Update();
                }
                catch (itk::ExceptionObject & excp)
                {
                    std::cout << std::endl;
                    std::cerr << "Error during local variance calculation ... " << std::endl;
                    std::cerr << excp << std::endl;
                    return EXIT_FAILURE;
                }
                data = varianceFilter->GetOutput();
                break;
    }
    case DER:
    {
                DerivativeFilterType::Pointer derivativeFilter = DerivativeFilterType::New();
                derivativeFilter->SetInput(inputReader->GetOutput());
                derivativeFilter->SetDirection(2);
                InvertIntensityFilterType::Pointer invert = InvertIntensityFilterType::New();
                invert->SetInput(derivativeFilter->GetOutput());
                IntensityRescaleFilterType::Pointer caster = IntensityRescaleFilterType::New();
                caster->SetInput(invert->GetOutput());
                try
                {
                    caster->Update();
                }
                catch (itk::ExceptionObject & excp)
                {
                    std::cout << std::endl;
                    std::cerr << "Error during local derivative calculation ... " << std::endl;
                    std::cerr << excp << std::endl;
                    return EXIT_FAILURE;
                }
                data = caster->GetOutput();
                break;
    }
    default:
    {
               std::cerr << "Error: Wrong data type specified. We stop the program." << std::endl;
               return EXIT_FAILURE;
               break;
    }
    }

    
    
    /** Topological plan detection
     *  Filter that use a multiscale resolution approach to detect the topological plan of the signal
     *  The scale level determine how much the data will be downscale, the filter will then build a 
     *  scale schedule of the multiscale pyramid using a regular factor 2 upscale.
     *  A median filter is used to smooth the output data.
     *  Finally, the topological map is determine using different method:
     *  max - maximum intensity peak
     *  first - first relevant intensity peak
     *  second - second relevant intensity peak
    **/



    VolumeMedianFilterType::InputSizeType radius;
    radius.Fill(1);
    VolumeMedianFilterType::Pointer median = VolumeMedianFilterType::New();
    median->SetInput(data);
    median->SetRadius(radius);
    
    unsigned int level = 3;
    unsigned int searchRange = delta;
    unsigned int medianRadius = 1;
    
    itk::Array2D<unsigned int> schedule(level, 3);
    for (unsigned int i = 0; i < level; i++)
    {
        schedule.SetElement(i, 0, scale);
        schedule.SetElement(i, 1, scale);
        schedule.SetElement(i, 2, 1);
        scale = std::max<int>(scale / 2, 0);
    }
    
    VolumeDataToTopologicalMapFilterType::Pointer topologicalMapFilter = VolumeDataToTopologicalMapFilterType::New();
    topologicalMapFilter->SetInput(median->GetOutput());
    topologicalMapFilter->SetDeltaZ(searchRange);
    topologicalMapFilter->SetMedianRadius(medianRadius);
    topologicalMapFilter->SetNumberOfLevels(level);
    topologicalMapFilter->SetScalingSchedule(schedule);
    topologicalMapFilter->SetTolerance(tolerance);
    topologicalMapFilter->SetDetection(peak);



    /** Output writing 
     *  outputName - string variable provided by the user
    **/
    ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetFileName(outputName);
    writer->SetInput(topologicalMapFilter->GetOutput());
    try
    {
        writer->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
        std::cout << std::endl;
        std::cerr << "Error writer ... " << std::endl;
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }
    



    /** Benchmarking **/
    std::cout << "Processing time : " << (double)(clock() - tStart) / CLOCKS_PER_SEC << " sec" << std::endl;
    
    /** That's all folks! **/
    return EXIT_SUCCESS;
}

