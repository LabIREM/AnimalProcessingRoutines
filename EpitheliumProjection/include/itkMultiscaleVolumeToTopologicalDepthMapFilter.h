#ifndef __itkMultiscaleVolumeToTopologicalDepthMapFilter_h
#define __itkMultiscaleVolumeToTopologicalDepthMapFilter_h

#include "itkImageToImageFilter.h"

#include <algorithm>    // std::min/max
#include "itkMedianImageFilter.h"
#include "itkMultiResolutionPyramidImageFilter.h"
#include "itkExpandImageFilter.h"
#include "itkVolumeDataToTopologicalDepthMapFilter.h"

namespace itk
{
    template< class InputImageType, class OutputImageType>
    class MultiscaleVolumeToTopologicalDepthMapFilter : public ImageToImageFilter< InputImageType, OutputImageType >
    {

    public:

        /** Standard class typedefs. */
        typedef MultiscaleVolumeToTopologicalDepthMapFilter  Self;
        typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
        typedef SmartPointer< Self > Pointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(MultiscaleVolumeToTopologicalDepthMapFilter, ImageToImageFilter);

        /** Method for getting I/O dimensions **/
        itkStaticConstMacro(InputImageDimension, unsigned int, InputImageType::ImageDimension);
        itkStaticConstMacro(OutputImageDimension, unsigned int, OutputImageType::ImageDimension);

#ifdef ITK_USE_CONCEPT_CHECKING
        // Begin concept checking
        itkConceptMacro(ImageDimensionCheck, (Concept::SameDimensionOrMinusOne<
            itkGetStaticConstMacro(InputImageDimension),
            itkGetStaticConstMacro(OutputImageDimension) >));
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

        itkSetMacro(DeltaZ, unsigned int);
        itkSetMacro(MedianRadius, unsigned int);
        itkSetMacro(NumberOfLevels, unsigned int);
        itkSetMacro(ScalingSchedule, Array2D<unsigned int>);

        itkSetMacro(Tolerance, double);
        itkSetMacro(Detection, unsigned int);

        itkGetMacro(DeltaZ, unsigned int);
        itkGetMacro(MedianRadius, unsigned int);
        itkGetMacro(NumberOfLevels, unsigned int);
        itkGetMacro(ScalingSchedule, Array2D<unsigned int>);

        itkGetConstReferenceMacro(ProjectionDimension, unsigned int);

    protected:

        typedef MultiResolutionPyramidImageFilter< InputImageType, InputImageType > MultiResolutionPyramidImageFilterType;
        typedef ExpandImageFilter< OutputImageType, OutputImageType > ExpandImageFilterType;
        typedef MedianImageFilter< OutputImageType, OutputImageType > MedianImageFilterType;
        typedef VolumeDataToTopologicalDepthMapFilter< InputImageType, OutputImageType > VolumeDataToTopologicalDepthMapFilterType;

        MultiscaleVolumeToTopologicalDepthMapFilter();
        virtual ~MultiscaleVolumeToTopologicalDepthMapFilter(){}

        /** Manage the loss of dimension between input and output **/
        virtual void GenerateOutputInformation() ITK_OVERRIDE;
        virtual void GenerateInputRequestedRegion() ITK_OVERRIDE;

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;
        //virtual void ThreadedGenerateData(const OutputImageRegionType &, ThreadIdType ) ITK_OVERRIDE;

    private:

        ITK_DISALLOW_COPY_AND_ASSIGN(MultiscaleVolumeToTopologicalDepthMapFilter);

        typename MultiResolutionPyramidImageFilterType::Pointer m_MultiscalePyramideImageFilter;
        typename ExpandImageFilterType::Pointer m_ExpandImageFilter;
        typename MedianImageFilterType::Pointer m_MedianImageFilter;
        typename VolumeDataToTopologicalDepthMapFilterType::Pointer m_TopologicalDepthMapFilter;

        unsigned int m_Detection;
        double m_Tolerance;
        unsigned int m_DeltaZ;
        unsigned int m_MedianRadius;
        unsigned int m_NumberOfLevels;
        Array2D<unsigned int> m_ScalingSchedule;

        unsigned int m_ProjectionDimension;

    };

} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMultiscaleVolumeToTopologicalDepthMapFilter.hxx"
#endif


#endif // __itkVolumeDataToTopologicalDepthMapFilter_h
