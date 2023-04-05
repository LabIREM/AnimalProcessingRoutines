#ifndef __itkVolumeDataToTopologicalDepthMapFilter_h
#define __itkVolumeDataToTopologicalDepthMapFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkConceptChecking.h"

#include <algorithm>    // std::min/max
#include "LocalPeakDetection1D.h"

namespace itk
{
    template< class InputImageType, class OutputImageType>
    class VolumeDataToTopologicalDepthMapFilter:public ImageToImageFilter< InputImageType, OutputImageType >
    {
        
    public:
        
        /** Standard class typedefs. */
        typedef VolumeDataToTopologicalDepthMapFilter  Self;
        typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
        typedef SmartPointer< Self > Pointer;
        
        /** Method for creation through the object factory. */
        itkNewMacro(Self);
        
        /** Run-time type information (and related methods). */
        itkTypeMacro(VolumeDataToTopologicalDepthMapFilter, ImageToImageFilter);
        
        /** Method for getting I/O dimensions **/
        itkStaticConstMacro(InputImageDimension, unsigned int, InputImageType::ImageDimension);
        itkStaticConstMacro(OutputImageDimension, unsigned int, OutputImageType::ImageDimension);
        
        #ifdef ITK_USE_CONCEPT_CHECKING
        // Begin concept checking
        itkConceptMacro( ImageDimensionCheck, ( Concept::SameDimensionOrMinusOne<
                                               itkGetStaticConstMacro(InputImageDimension),
                                               itkGetStaticConstMacro(OutputImageDimension) > ) );
        // End concept checking
        #endif
        
        typedef typename InputImageType::Pointer     InputImagePointer;
        typedef typename InputImageType::RegionType  InputImageRegionType;
        typedef typename InputImageType::PixelType   InputImagePixelType;
        typedef typename InputImageType::SizeType    InputImageSizeType;
        typedef typename InputImageType::IndexType   InputImageIndexType;
        typedef typename OutputImageType::Pointer    OutputImagePointer;
        typedef typename OutputImageType::RegionType OutputImageRegionType;
        typedef typename OutputImageType::PixelType  OutputImagePixelType;
        typedef typename OutputImageType::SizeType   OutputImageSizeType;
        typedef typename OutputImageType::IndexType  OutputImageIndexType;
        
        itkSetMacro(InitialisationMap, typename OutputImageType::Pointer);
        itkSetMacro(Delta, unsigned int);
        itkSetMacro(Tolerance, double);
        itkSetMacro(Detection, unsigned int);
        itkGetConstReferenceMacro(ProjectionDimension, unsigned int);
        
    protected:
        
        VolumeDataToTopologicalDepthMapFilter();
        virtual ~VolumeDataToTopologicalDepthMapFilter(){}
        
        /** Manage the loss of dimension between input and output **/
        virtual void GenerateOutputInformation() ITK_OVERRIDE;
        virtual void GenerateInputRequestedRegion() ITK_OVERRIDE;
        
        /** Does the real work. */
        //virtual void GenerateData() ITK_OVERRIDE;
        virtual void ThreadedGenerateData(const OutputImageRegionType &, ThreadIdType ) ITK_OVERRIDE;
        
    private:
        
        ITK_DISALLOW_COPY_AND_ASSIGN(VolumeDataToTopologicalDepthMapFilter);
        
        typename OutputImageType::Pointer m_InitialisationMap;
        unsigned int m_Delta;
        double m_Tolerance;
        unsigned int m_ProjectionDimension;
        unsigned int m_Detection;

        
    };
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVolumeDataToTopologicalDepthMapFilter.hxx"
#endif


#endif // __itkVolumeDataToTopologicalDepthMapFilter_h
