#ifndef __itkVolumeDataToTopologicalDepthMapFilter_hxx
#define __itkVolumeDataToTopologicalDepthMapFilter_hxx

#include "itkVolumeDataToTopologicalDepthMapFilter.h"
#include <vector>
#include <cmath>

namespace itk
{

    template< class InputImageType, class OutputImageType>
    VolumeDataToTopologicalDepthMapFilter< InputImageType, OutputImageType>
    ::VolumeDataToTopologicalDepthMapFilter()
    {
            this->SetNumberOfRequiredInputs(1);
            m_ProjectionDimension = InputImageDimension - 1;
            m_Delta = 1;
            m_Tolerance = 0;
            m_Detection = 0;
        }

    template< class InputImageType, class OutputImageType>
    void
    VolumeDataToTopologicalDepthMapFilter< InputImageType, OutputImageType>
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
    VolumeDataToTopologicalDepthMapFilter< InputImageType, OutputImageType>
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
    VolumeDataToTopologicalDepthMapFilter< InputImageType, OutputImageType>
    ::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId)
    {
            if (m_ProjectionDimension >= InputImageType::ImageDimension)
            {
                itkExceptionMacro(<< "Invalid ProjectionDimension "
                    << m_ProjectionDimension
                    << " but ImageDimension is "
                    << InputImageType::ImageDimension);
            }

            // use the output image to report the progress: there is no need to
            // call CompletedPixel() for all input pixel
            ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

            // get some values, just to be easier to manipulate
            typename Superclass::InputImageConstPointer input = this->GetInput();
            typename InputImageType::RegionType inputRegion = input->GetLargestPossibleRegion();
            typename InputImageType::SizeType inputSize = inputRegion.GetSize();
            typename InputImageType::IndexType inputIndex = inputRegion.GetIndex();
            typename OutputImageType::Pointer output = this->GetOutput();
            typename OutputImageType::RegionType outputRegion = output->GetLargestPossibleRegion();
            typename OutputImageType::SizeType outputSizeForThread = outputRegionForThread.GetSize();
            typename OutputImageType::IndexType outputIndexForThread = outputRegionForThread.GetIndex();

            // compute the input region for this thread
            typename InputImageType::RegionType inputRegionForThread = inputRegion;
            typename InputImageType::SizeType inputSizeForThread = inputSize;
            typename InputImageType::IndexType inputIndexForThread = inputIndex;
            if (static_cast< unsigned int >(InputImageDimension) == static_cast< unsigned int >(OutputImageDimension))
            {
                for (unsigned int i = 0; i < InputImageDimension; i++)
                {
                    if (i != m_ProjectionDimension)
                    {
                        inputSizeForThread[i] = outputSizeForThread[i];
                        inputIndexForThread[i] = outputIndexForThread[i];
                    }
                }
            }
            else
            {
                for (unsigned int i = 0; i < OutputImageDimension; i++)
                {
                    if (i != m_ProjectionDimension)
                    {
                        inputSizeForThread[i] = outputSizeForThread[i];
                        inputIndexForThread[i] = outputIndexForThread[i];
                    }
                    else
                    {
                        // the size of the output image on this dimension is the size
                        // of the input image on the removed dimension
                        inputSizeForThread[InputImageDimension - 1] = outputSizeForThread[i];
                        inputIndexForThread[InputImageDimension - 1] = outputIndexForThread[i];
                    }
                }
                inputSizeForThread[m_ProjectionDimension] = inputSize[m_ProjectionDimension];
                inputIndexForThread[m_ProjectionDimension] = inputIndex[m_ProjectionDimension];
            }
            inputRegionForThread.SetSize(inputSizeForThread);
            inputRegionForThread.SetIndex(inputIndexForThread);

            SizeValueType projectionSize = inputSize[m_ProjectionDimension];

            // we define an iterator
            typedef ImageLinearConstIteratorWithIndex< InputImageType > InputIteratorType;
            InputIteratorType iIt(input, inputRegionForThread);
            iIt.SetDirection(m_ProjectionDimension);
            iIt.GoToBegin();

            // for each (x,y) couple
            while (!iIt.IsAtEnd())
            {
                // move the ouput iterator and set the output value
                typename OutputImageType::IndexType oIdx;
                typename InputImageType::IndexType iIdx = iIt.GetIndex();
                for (unsigned int i = 0; i < OutputImageDimension; i++)
                {
                    if (i != m_ProjectionDimension)
                    {
                        oIdx[i] = iIdx[i];
                    }
                }

                // define the z-range
                unsigned int topZ = 0;
                unsigned int botZ = projectionSize - 1;
                if (m_InitialisationMap.IsNotNull())
                {
                    // get the given inital z and search around it for a more optimal value
                    int previousZ = m_InitialisationMap->GetPixel(oIdx);
                    topZ = (int)previousZ - m_Delta;
                    topZ = std::max<int>(topZ, 0);
                    topZ = std::min<int>(topZ, projectionSize - 1);
                    botZ = (int)previousZ + m_Delta;
                    botZ = std::max<int>(botZ, 0);
                    botZ = std::min<int>(botZ, projectionSize - 1);
                }


                // accumulate the values in a vector
                typename std::vector< typename InputImageType::PixelType>  valueList;
                typename std::vector< typename OutputImageType::PixelType> indexList;
                while (!iIt.IsAtEndOfLine())
                {
                    if (iIt.GetIndex()[m_ProjectionDimension] > topZ && iIt.GetIndex()[m_ProjectionDimension] < botZ)
                    {
                        valueList.push_back(iIt.Get());
                        indexList.push_back(iIt.GetIndex()[m_ProjectionDimension]);
                    }
                    ++iIt;
                }

                // get peak position
                LocalPeakDetection1D< typename InputImageType::PixelType, typename OutputImageType::PixelType> detector(valueList, indexList, m_Tolerance);
                typename OutputImageType::PixelType optimalZ;
                enum { MAX = 0, FIRST = 1, SECOND = 2 };
                switch (m_Detection)
                {
                case MAX: optimalZ = detector.GetMaxPeak(); break;
                case FIRST: optimalZ = detector.GetFirstPeak(); break;
                case SECOND: optimalZ = detector.GetSecondPeak(); break;
                default: optimalZ = detector.GetMaxPeak(); break;
                }

                output->SetPixel(oIdx, optimalZ);

                // one more line done !
                progress.CompletedPixel();

                // continue with the next one
                iIt.NextLine();
            }
        }

    
    //template< class InputImageType, class OutputImageType>
    //void
    //    VolumeDataToTopologicalDepthMapFilter< InputImageType, OutputImageType>
    //    ::GenerateData()
    //{
    //
    //        typename InputImageType::ConstPointer input = this->GetInput();
    //        typename OutputImageType::Pointer output = this->GetOutput();
    //
    //        this->AllocateOutputs();
    //
    //        // Get input dimension size
    //        InputImageSizeType inputSize = input->GetLargestPossibleRegion().GetSize();
    //
    //        // TODO:
    //        // replace for loops by iterators on the output image
    //        // (optional) retry the parallel computing ...
    //
    //        for (unsigned int x = 0; x < inputSize[0]; x++)
    //        {
    //            for (unsigned int y = 0; y < inputSize[1]; y++)
    //            {
    //                InputImagePixelType maximumIntensity(0), optimalZ(0), temporaryValue(0);
    //                InputImageIndexType temporaryIndex;
    //                OutputImageIndexType  outputIndex;
    //
    //                outputIndex[0] = x;
    //                outputIndex[1] = y;
    //
    //                unsigned int topZ, botZ;
    //                if (m_InitialisationMap.IsNotNull())
    //                {
    //                    // get the given inital z and search around it for a more optimal value
    //                    int previousZ = m_InitialisationMap->GetPixel(outputIndex);
    //                    topZ = std::max<int>((int)previousZ - m_Delta, 0);
    //                    botZ = std::min<int>((int)previousZ + m_Delta, inputSize[2]-1);
    //                }
    //                else
    //                {
    //                    // no initial z, we search the entire depth range of the volume
    //                    topZ = 0;
    //                    botZ = inputSize[2]-1;
    //                }
    //
    //                // Find the z associated with the highest intensity
    //                for (unsigned int z = topZ; z <= botZ; z++)
    //                {
    //                    temporaryIndex[0] = x;
    //                    temporaryIndex[1] = y;
    //                    temporaryIndex[2] = z;
    //                    temporaryValue = input->GetPixel(temporaryIndex);
    //                    if (temporaryValue > maximumIntensity)
    //                    {
    //                        maximumIntensity = temporaryValue;
    //                        optimalZ = z;
    //                    }
    //                }
    //
    //                // save the optimal z found
    //                output->SetPixel(outputIndex, optimalZ);
    //            }
    //        }
    //    }
        

}// end namespace


#endif
