
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

// my includes
#include "itkMultiscaleVolumeToTopologicalDepthMapFilter.h"
#include "itkImageProjectionFromTopologicalMapFilter.h"



int main(int argc, char ** argv)
{
    
    clock_t tStart;
    tStart = clock();
    
    // variables
    std::string inputName, outputName, zmapName, projectionType;
    int shiftZ, deltaTop, deltaBot;
    
    
    // command line
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("input,in", boost::program_options::value<std::string>(&inputName), "Input file name")
    ("output,out", boost::program_options::value<std::string>(&outputName), "Output file name")
    ("zmap,zm", boost::program_options::value<std::string>(&zmapName), "topological map file name")
    ("type,p", boost::program_options::value<std::string>(&projectionType)->default_value("max"), "type of projection [max|mean]")
    ("shift,s", boost::program_options::value<int>(&shiftZ)->default_value(0), "z-shift")
    ("deltaTop,t", boost::program_options::value<int>(&deltaTop)->default_value(-1), "Delta top")
    ("deltaBot,b", boost::program_options::value<int>(&deltaBot)->default_value(1), "Delta bot");
    
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    
    if (vm.count("help"))
    {
        std::cout << "Compiled : " << __DATE__ << " at " << __TIME__ << std::endl;
        std::cout << desc << "\n";
        return EXIT_SUCCESS;
    }
    
    
    
    
    
    typedef unsigned short PixelType;
    typedef itk::Image< PixelType, 3 > VolumeType;
    typedef itk::Image< PixelType, 2 > ImageType;
    typedef itk::ImageFileReader< VolumeType > VolumeReaderType;
    typedef itk::ImageFileReader< ImageType > ImageReaderType;
    typedef itk::ImageFileWriter< ImageType > ImageWriterType;
    typedef itk::MedianImageFilter<VolumeType, VolumeType > VolumeMedianFilterType;
    typedef itk::ImageProjectionFromTopologicalMapFilter<VolumeType, ImageType, ImageType> ImageProjectionFromTopologicalMapFilterType;
    
    
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // INPUTS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //std::cout << "\tRead input volume ... ";
    VolumeReaderType::Pointer inputReader = VolumeReaderType::New();
    inputReader->SetFileName(inputName);
    try
    {
        inputReader->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
        std::cout << std::endl;
        std::cerr << "Error inputReader ... " << std::endl;
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }
    
    ImageReaderType::Pointer zmapReader = ImageReaderType::New();
    zmapReader->SetFileName(zmapName);
    try
    {
        zmapReader->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
        std::cout << std::endl;
        std::cerr << "Error zmapReader ... " << std::endl;
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }
    
    VolumeMedianFilterType::InputSizeType radius;
    radius.Fill(1);
    VolumeMedianFilterType::Pointer median = VolumeMedianFilterType::New();
    median->SetInput(inputReader->GetOutput());
    median->SetRadius(radius);
    
    ImageProjectionFromTopologicalMapFilterType::ArrayType delta;
    delta[0] = deltaTop;
    delta[1] = deltaBot;
    ImageProjectionFromTopologicalMapFilterType::Pointer projectionFilter = ImageProjectionFromTopologicalMapFilterType::New();
    projectionFilter->SetInputImage(median->GetOutput());
    projectionFilter->SetInputZmap(zmapReader->GetOutput());
    projectionFilter->SetProjectionType(projectionType);
    projectionFilter->SetDeltaZ(delta);
    projectionFilter->SetTranslationZ(shiftZ);
    try
    {
        projectionFilter->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
        std::cout << std::endl;
        std::cerr << "Error projectionFilter ... " << std::endl;
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // OUTPUTS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetFileName(outputName);
    writer->SetInput(projectionFilter->GetOutput());
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
    
    
    std::cout << "Processing time : " << (double)(clock() - tStart) / CLOCKS_PER_SEC << " sec" << std::endl;
    
    
    return EXIT_SUCCESS;
}

