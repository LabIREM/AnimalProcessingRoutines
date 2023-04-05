#ifndef __itkMultiscaleVolumeToTopologicalDepthMapFilter_hxx
#define __itkMultiscaleVolumeToTopologicalDepthMapFilter_hxx

#include "itkMultiscaleVolumeToTopologicalDepthMapFilter.h"


namespace itk
{

    template< class InputImageType, class OutputImageType>
    MultiscaleVolumeToTopologicalDepthMapFilter< InputImageType, OutputImageType>
    ::MultiscaleVolumeToTopologicalDepthMapFilter()
    {
            this->SetNumberOfRequiredInputs(1);
            this->SetNumberOfRequiredOutputs(1);

            m_MultiscalePyramideImageFilter = MultiResolutionPyramidImageFilterType::New();
            m_ExpandImageFilter = ExpandImageFilterType::New();
            m_MedianImageFilter = MedianImageFilterType::New();
            m_TopologicalDepthMapFilter = VolumeDataToTopologicalDepthMapFilterType::New();

            m_Detection = 0;
            m_Tolerance = 0;
            m_DeltaZ = 3;
            m_MedianRadius = 2;
            m_NumberOfLevels = 1;
            m_ScalingSchedule.SetSize(1, 3);
            m_ScalingSchedule.SetElement(0, 0, 1);
            m_ScalingSchedule.SetElement(0, 1, 1);
            m_ScalingSchedule.SetElement(0, 2, 1);

            m_ProjectionDimension = InputImageDimension - 1;
        }


    template< class InputImageType, class OutputImageType>
    void
    MultiscaleVolumeToTopologicalDepthMapFilter< InputImageType, OutputImageType >
    ::GenerateOutputInformation()
    {
            itkDebugMacro("GenerateOutputInformation Start");

            if (m_ProjectionDimension >= InputImageType::ImageDimension)
            {
                itkExceptionMacro(<< "Invalid ProjectionDimension. ProjectionDimension is "
                    << m_ProjectionDimension
                    << " but input ImageDimension is "
                    << InputImageType::ImageDimension);
            }

            typename OutputImageType::RegionType outputRegion;
            typename InputImageType::IndexType inputIndex;
            typename InputImageType::SizeType inputSize;
            typename OutputImageType::SizeType outputSize;
            typename OutputImageType::IndexType outputIndex;
            typename InputImageType::SpacingType inSpacing;
            typename InputImageType::PointType inOrigin;
            typename OutputImageType::SpacingType outSpacing;
            typename OutputImageType::PointType outOrigin;

            // Get pointers to the input and output
            typename Superclass::OutputImagePointer output = this->GetOutput();
            typename Superclass::InputImagePointer input = const_cast< InputImageType * >(this->GetInput());

            inputIndex = input->GetLargestPossibleRegion().GetIndex();
            inputSize = input->GetLargestPossibleRegion().GetSize();
            inSpacing = input->GetSpacing();
            inOrigin = input->GetOrigin();

            // Set the LargestPossibleRegion of the output.
            // Reduce the size of the accumulated dimension.

            if (static_cast< unsigned int >(InputImageDimension) == static_cast< unsigned int >(OutputImageDimension))
            {
                for (unsigned int i = 0; i < InputImageDimension; i++)
                {
                    if (i != m_ProjectionDimension)
                    {
                        outputSize[i] = inputSize[i];
                        outputIndex[i] = inputIndex[i];
                        outSpacing[i] = inSpacing[i];
                        outOrigin[i] = inOrigin[i];
                    }
                    else
                    {
                        outputSize[i] = 1;
                        outputIndex[i] = 0;
                        outSpacing[i] = inSpacing[i] * inputSize[i];
                        outOrigin[i] = inOrigin[i] + (i - 1) * inSpacing[i] / 2;
                    }
                }
            }
            else
            {
                // Then OutputImageDimension = InputImageDimension - 1
                for (unsigned int i = 0; i < OutputImageDimension; i++)
                {
                    if (i != m_ProjectionDimension)
                    {
                        outputSize[i] = inputSize[i];
                        outputIndex[i] = inputIndex[i];
                        outSpacing[i] = inSpacing[i];
                        outOrigin[i] = inOrigin[i];
                    }
                    else
                    {
                        outputSize[i] = inputSize[InputImageDimension - 1];
                        outputIndex[i] = inputIndex[InputImageDimension - 1];
                        outSpacing[i] = inSpacing[InputImageDimension - 1];
                        outOrigin[i] = inOrigin[InputImageDimension - 1];
                    }
                }
            }

            outputRegion.SetSize(outputSize);
            outputRegion.SetIndex(outputIndex);
            output->SetOrigin(outOrigin);
            output->SetSpacing(outSpacing);
            output->SetLargestPossibleRegion(outputRegion);

            itkDebugMacro("GenerateOutputInformation End");
        }

    template< class InputImageType, class OutputImageType>
    void
    MultiscaleVolumeToTopologicalDepthMapFilter< InputImageType, OutputImageType>
    ::GenerateInputRequestedRegion()
    {
            itkDebugMacro("GenerateInputRequestedRegion Start");

            if (m_ProjectionDimension >= InputImageType::ImageDimension)
            {
                itkExceptionMacro(<< "Invalid ProjectionDimension "
                    << m_ProjectionDimension
                    << " but ImageDimension is "
                    << InputImageType::ImageDimension);
            }

            Superclass::GenerateInputRequestedRegion();

            if (this->GetInput())
            {
                typename InputImageType::RegionType RequestedRegion;
                typename InputImageType::SizeType inputSize;
                typename InputImageType::IndexType inputIndex;
                typename InputImageType::SizeType inputLargSize;
                typename InputImageType::IndexType inputLargIndex;
                typename OutputImageType::SizeType outputSize;
                typename OutputImageType::IndexType outputIndex;

                outputIndex = this->GetOutput()->GetRequestedRegion().GetIndex();
                outputSize = this->GetOutput()->GetRequestedRegion().GetSize();
                inputLargSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
                inputLargIndex = this->GetInput()->GetLargestPossibleRegion().GetIndex();

                if (static_cast< unsigned int >(InputImageDimension) == static_cast< unsigned int >(OutputImageDimension))
                {
                    for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
                    {
                        if (i != m_ProjectionDimension)
                        {
                            inputSize[i] = outputSize[i];
                            inputIndex[i] = outputIndex[i];
                        }
                        else
                        {
                            inputSize[i] = inputLargSize[i];
                            inputIndex[i] = inputLargIndex[i];
                        }
                    }
                }
                else
                {
                    for (unsigned int i = 0; i < OutputImageDimension; i++)
                    {
                        if (i != m_ProjectionDimension)
                        {
                            inputSize[i] = outputSize[i];
                            inputIndex[i] = outputIndex[i];
                        }
                        else
                        {
                            // the size of the output image on this dimension is the size
                            // of the input image on the removed dimension
                            inputSize[InputImageDimension - 1] = outputSize[i];
                            inputIndex[InputImageDimension - 1] = outputIndex[i];
                        }
                    }
                    inputSize[m_ProjectionDimension] = inputLargSize[m_ProjectionDimension];
                    inputIndex[m_ProjectionDimension] = inputLargIndex[m_ProjectionDimension];
                }

                RequestedRegion.SetSize(inputSize);
                RequestedRegion.SetIndex(inputIndex);
                InputImagePointer input = const_cast< InputImageType * >(this->GetInput());
                input->SetRequestedRegion(RequestedRegion);
            }

            itkDebugMacro("GenerateInputRequestedRegion End");
        }


    template< class InputImageType, class OutputImageType>
    void
    MultiscaleVolumeToTopologicalDepthMapFilter< InputImageType, OutputImageType>
    ::GenerateData()
    {
            // I/O
            typename InputImageType::ConstPointer input = this->GetInput();
            typename OutputImageType::Pointer output = this->GetOutput();
            this->AllocateOutputs();

            // Setup the multiscale image filter
            m_MultiscalePyramideImageFilter->SetInput(input);
            m_MultiscalePyramideImageFilter->SetNumberOfLevels(m_NumberOfLevels);
            m_MultiscalePyramideImageFilter->SetSchedule(m_ScalingSchedule);

            // Initialise loop variables
            typename MedianImageFilterType::InputSizeType radius;
            radius.Fill(m_MedianRadius);
            typename OutputImageType::Pointer previousTopologicalMap;
            int ExpandImageFactor = 1;

            // For each scaling iteration
            for (unsigned int iterationLevel = 0; iterationLevel < m_MultiscalePyramideImageFilter->GetNumberOfLevels(); iterationLevel++)
            {

                // Compute the topological map at a given scale from previous topological map
                m_TopologicalDepthMapFilter->SetInput(m_MultiscalePyramideImageFilter->GetOutput(iterationLevel));
                m_TopologicalDepthMapFilter->SetDelta(m_DeltaZ);
                m_TopologicalDepthMapFilter->SetTolerance(m_Tolerance);
                m_TopologicalDepthMapFilter->SetDetection(m_Detection);
                if (previousTopologicalMap.IsNotNull())
                {
                    // if needed, expand previous topological map according to the scaling schedule
                    ExpandImageFactor = m_ScalingSchedule.GetElement(iterationLevel - 1, 0) / m_ScalingSchedule.GetElement(iterationLevel, 0);
                    m_ExpandImageFilter->SetExpandFactors(ExpandImageFactor);
                    m_ExpandImageFilter->SetInput(previousTopologicalMap);
                    try
                    {
                        m_ExpandImageFilter->UpdateLargestPossibleRegion();
                    }
                    catch (itk::ExceptionObject & excp)
                    {
                        std::cerr << "error caught in m_expandimagefilter loop iteration => " << iterationLevel << std::endl;
                        std::cerr << excp << std::endl;
                    }
                    previousTopologicalMap = m_ExpandImageFilter->GetOutput();
                    m_TopologicalDepthMapFilter->SetInitialisationMap(previousTopologicalMap);
                }

                // Smooth results using a median filter
                m_MedianImageFilter->SetRadius(radius);
                m_MedianImageFilter->SetInput(m_TopologicalDepthMapFilter->GetOutput());
                try
                {
                    m_MedianImageFilter->UpdateLargestPossibleRegion();
                }
                catch (itk::ExceptionObject & excp)
                {
                    std::cerr << "Error caught in m_MedianImageFilter loop iteration => " << iterationLevel << std::endl;
                    std::cerr << excp << std::endl;
                }

                // Disconnect iteration output
                previousTopologicalMap = m_MedianImageFilter->GetOutput();
                previousTopologicalMap->DisconnectPipeline();
            }

            // Expand and smooth final topological map
            ExpandImageFactor = (int)input->GetLargestPossibleRegion().GetSize()[0] / (int)previousTopologicalMap->GetLargestPossibleRegion().GetSize()[0];
            m_ExpandImageFilter->SetExpandFactors(ExpandImageFactor);
            m_ExpandImageFilter->SetInput(previousTopologicalMap);
            m_MedianImageFilter->SetRadius(radius);
            m_MedianImageFilter->SetInput(m_ExpandImageFilter->GetOutput());
            try
            {
                m_MedianImageFilter->UpdateLargestPossibleRegion();
            }
            catch (itk::ExceptionObject & excp)
            {
                std::cerr << "Error caught in m_MedianImageFilter ... " << std::endl;
                std::cerr << excp << std::endl;
            }

            // Graft it to the pipeline output
            this->GetOutput()->Graft(m_MedianImageFilter->GetOutput());
        }



}// end namespace


#endif
