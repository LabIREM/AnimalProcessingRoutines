#ifndef __itkImageProjectionFromTopologicalMapFilter_hxx
#define __itkImageProjectionFromTopologicalMapFilter_hxx

#include "itkImageProjectionFromTopologicalMapFilter.h"


namespace itk
{
    
    template< class InputImageType, class ZmapImageType, class OutputImageType >
    ImageProjectionFromTopologicalMapFilter<InputImageType, ZmapImageType, OutputImageType>
    ::ImageProjectionFromTopologicalMapFilter()
    {
        this->SetNumberOfRequiredInputs(2);
        m_ProjectionDimension = InputImageDimension - 1;
        m_DeltaZ[0] = 0;
        m_DeltaZ[1] = 0;
        m_TranslationZ = 0;
        m_ProjectionType = "max";
    }
    
    template< class InputImageType, class ZmapImageType, class OutputImageType >
    void
    ImageProjectionFromTopologicalMapFilter<InputImageType, ZmapImageType, OutputImageType>
    ::SetInputImage(const InputImageType * image)
    {
        this->SetNthInput(0, const_cast<InputImageType *>(image));
    }
    
    template< class InputImageType, class ZmapImageType, class OutputImageType >
    void
    ImageProjectionFromTopologicalMapFilter<InputImageType, ZmapImageType, OutputImageType>
    ::SetInputZmap(const ZmapImageType * zmap)
    {
        this->SetNthInput(1, const_cast<ZmapImageType *>(zmap));
    }
    
    template< class InputImageType, class ZmapImageType, class OutputImageType >
    void
    ImageProjectionFromTopologicalMapFilter<InputImageType, ZmapImageType, OutputImageType>
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
        typename Superclass::InputImagePointer input = const_cast<InputImageType *>(this->GetInputImage());
        
        inputIndex = input->GetLargestPossibleRegion().GetIndex();
        inputSize = input->GetLargestPossibleRegion().GetSize();
        inSpacing = input->GetSpacing();
        inOrigin = input->GetOrigin();
        
        // Set the LargestPossibleRegion of the output.
        // Reduce the size of the accumulated dimension.
        
        if (static_cast<unsigned int>(InputImageDimension) == static_cast<unsigned int>(OutputImageDimension))
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
    
    template< class InputImageType, class ZmapImageType, class OutputImageType >
    void
    ImageProjectionFromTopologicalMapFilter<InputImageType, ZmapImageType, OutputImageType>
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
        
        if (this->GetInputImage())
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
            
            if (static_cast<unsigned int>(InputImageDimension) == static_cast<unsigned int>(OutputImageDimension))
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
            InputImagePointer input = const_cast<InputImageType *>(this->GetInputImage());
            input->SetRequestedRegion(RequestedRegion);
        }
        
        itkDebugMacro("GenerateInputRequestedRegion End");
    }
    
    
    
    
    template< class InputImageType, class ZmapImageType, class OutputImageType >
    void
    ImageProjectionFromTopologicalMapFilter<InputImageType, ZmapImageType, OutputImageType>
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
        typename Superclass::InputImageConstPointer input = this->GetInputImage();
        typename InputImageType::RegionType inputRegion = input->GetLargestPossibleRegion();
        typename InputImageType::SizeType inputSize = inputRegion.GetSize();
        typename InputImageType::IndexType inputIndex = inputRegion.GetIndex();
        typename OutputImageType::Pointer output = this->GetOutput();
        typename OutputImageType::RegionType outputRegion = output->GetLargestPossibleRegion();
        typename OutputImageType::SizeType outputSizeForThread = outputRegionForThread.GetSize();
        typename OutputImageType::IndexType outputIndexForThread = outputRegionForThread.GetIndex();
        
        // get some values for the zmap
        typename ZmapImageType::ConstPointer zmap = this->GetInputZmap();
        
        // compute the input region for this thread
        typename InputImageType::RegionType inputRegionForThread = inputRegion;
        typename InputImageType::SizeType inputSizeForThread = inputSize;
        typename InputImageType::IndexType inputIndexForThread = inputIndex;
        if (static_cast<unsigned int>(InputImageDimension) == static_cast<unsigned int>(OutputImageDimension))
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
            typename ZmapImageType::IndexType zIdx;
            typename InputImageType::IndexType iIdx = iIt.GetIndex();
            for (unsigned int i = 0; i < OutputImageDimension; i++)
            {
                if (i != m_ProjectionDimension)
                {
                    oIdx[i] = iIdx[i];
                    zIdx[i] = iIdx[i];
                }
            }
            
            
            int projZ = zmap->GetPixel(zIdx) + m_TranslationZ;
            // get z range and manage border effect
            int topZ = projZ + m_DeltaZ[0];
            topZ = std::max<int>(topZ, 0);
            topZ = std::min<int>(topZ, projectionSize - 1);
            int botZ = projZ + m_DeltaZ[1];
            botZ = std::max<int>(botZ, 0);
            botZ = std::min<int>(botZ, projectionSize - 1);
            
            
            // accumulate along the dimention
            std::vector<InputImagePixelType> accumulatedData;
            while (!iIt.IsAtEndOfLine())
            {
                if (iIt.GetIndex()[m_ProjectionDimension] >= topZ && iIt.GetIndex()[m_ProjectionDimension] <= botZ)
                {
                    accumulatedData.push_back(iIt.Get());
                }
                ++iIt;
            }
            
            // project according to methode
            OutputImagePixelType result = 0;
            if (!accumulatedData.empty())
            {
                if (m_ProjectionType.compare("max") == 0)
                {
                    typename std::vector<InputImagePixelType>::iterator rIt;
                    rIt = std::max_element(accumulatedData.begin(), accumulatedData.end());
                    result = *rIt;
                }
                else if (m_ProjectionType.compare("mean") == 0)
                {
                    int sum = std::accumulate(accumulatedData.begin(), accumulatedData.end(), 0);
                    result = sum / accumulatedData.size();
                }
            }
            output->SetPixel(oIdx, static_cast<OutputImagePixelType>(result));
            
            // one more line done !
            progress.CompletedPixel();
            
            // continue with the next one
            iIt.NextLine();
        }
        
    }
    
    
    
    
    //template< class InputImageType, class OutputImageType >
    //void
    //    ImageProjectionFromTopologicalMapFilter< InputImageType, OutputImageType >
    //    ::GenerateData()
    //{
    //
    //        typename InputImageType::ConstPointer input = this->GetInput();
    //        typename OutputImageType::Pointer output = this->GetOutput();
    //
    //        this->AllocateOutputs();
    //
    //        InputImageSizeType dataSize = input->GetLargestPossibleRegion().GetSize();
    //        unsigned int zTop = 0;
    //        unsigned int zBot = dataSize[2];
    //        for (unsigned int x = 0; x < dataSize[0]; x++)
    //        {
    //            for (unsigned int y = 0; y < dataSize[1]; y++)
    //            {
    //                // get the z for the coord (x,y)
    //                OutputImageIndexType topologicalMapIndex;
    //                topologicalMapIndex[0] = x;
    //                topologicalMapIndex[1] = y;
    //
    //                if (m_TopologicalMap.IsNotNull())
    //                {
    //                    int projZ = m_TopologicalMap->GetPixel(topologicalMapIndex) + m_TranslationZ;
    //                    // get z range and manage border effect
    //                    zTop = std::max<int>(projZ + m_DeltaZ[0], 0);
    //                    zBot = std::min<int>(projZ + m_DeltaZ[1], dataSize[2]-1);
    //                }
    //
    //                std::vector<InputImagePixelType> accumulatedData;
    //                InputImageIndexType inputIndex;
    //                for (unsigned int z = zTop; z < zBot; z++)
    //                {
    //                    inputIndex[0] = x;
    //                    inputIndex[1] = y;
    //                    inputIndex[2] = z;
    //                    accumulatedData.push_back(input->GetPixel(inputIndex));
    //                }
    //
    //                // project according to methode
    //                if (m_ProjectionType.compare("max") == 0)
    //                {
    //                    typename std::vector<InputImagePixelType>::iterator result;
    //                    result = std::max_element(accumulatedData.begin(), accumulatedData.end());
    //                    output->SetPixel(topologicalMapIndex, *result);
    //                }
    //                else if (m_ProjectionType.compare("mean") == 0)
    //                {
    //                    int sum = std::accumulate(accumulatedData.begin(), accumulatedData.end(), 0);
    //                    InputImagePixelType result = sum / accumulatedData.size();
    //                    output->SetPixel(topologicalMapIndex, result);
    //                }
    //            }
    //        }
    //    }
    
    
}// end namespace


#endif
