#ifndef __itkImageProjectionFromTopologicalMapFilter_h
#define __itkImageProjectionFromTopologicalMapFilter_h

#include "itkImageToImageFilter.h"

#include <algorithm>    // std::min/max


namespace itk
{
    template< class InputImageType, class ZmapImageType, class OutputImageType >
    class ImageProjectionFromTopologicalMapFilter : public ImageToImageFilter< InputImageType, OutputImageType >
    {

    public:

        /** Standard class typedefs. */
        typedef ImageProjectionFromTopologicalMapFilter  Self;
        typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
        typedef SmartPointer< Self > Pointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(ImageProjectionFromTopologicalMapFilter, ImageToImageFilter);

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
        typedef typename ZmapImageType::Pointer     ZmapImagePointer;
        typedef typename ZmapImageType::ConstPointer     ZmapImageConstPointer;
        typedef typename ZmapImageType::RegionType  ZmapImageRegionType;
        typedef typename ZmapImageType::PixelType   ZmapImagePixelType;
        typedef typename ZmapImageType::SizeType    ZmapImageSizeType;
        typedef typename ZmapImageType::IndexType   ZmapImageIndexType;

        typedef FixedArray<int, 2>  ArrayType;

        /** The image to be inpainted in regions where the mask is white.*/
        void SetInputImage(const InputImageType * image);

        /** The mask to be inpainted. White pixels will be inpainted, black pixels will be passed through to the output.*/
        void SetInputZmap(const ZmapImageType * mask);

        itkGetConstReferenceMacro(ProjectionDimension, unsigned int);

        //itkGetMacro(TopologicalMap, typename OutputImageType::Pointer);
        //itkSetMacro(TopologicalMap, typename OutputImageType::Pointer);

        itkGetMacro(DeltaZ, ArrayType);
        itkSetMacro(DeltaZ, ArrayType);

        itkGetMacro(ProjectionType, std::string);
        itkSetMacro(ProjectionType, std::string);

        itkGetMacro(TranslationZ, int);
        itkSetMacro(TranslationZ, int);



    protected:

        ImageProjectionFromTopologicalMapFilter();
        virtual ~ImageProjectionFromTopologicalMapFilter(){}

        const InputImageType * GetInputImage()
        {
            return static_cast<const InputImageType *> (this->ProcessObject::GetInput(0));
        }
        typename ZmapImageType::ConstPointer GetInputZmap()
        {
            return static_cast<const ZmapImageType *> (this->ProcessObject::GetInput(1));
        }

        /** Manage the loss of dimension between input and output **/
        virtual void GenerateOutputInformation() ITK_OVERRIDE;
        virtual void GenerateInputRequestedRegion() ITK_OVERRIDE;

        /** Does the real work. */
        //virtual void GenerateData() ITK_OVERRIDE;
        virtual void ThreadedGenerateData(const OutputImageRegionType &, ThreadIdType ) ITK_OVERRIDE;

    private:

        ITK_DISALLOW_COPY_AND_ASSIGN(ImageProjectionFromTopologicalMapFilter);

        int m_TranslationZ;
        unsigned int m_ProjectionDimension;
        ArrayType m_DeltaZ;
        std::string m_ProjectionType;

        //typename OutputImageType::Pointer m_TopologicalMap;
    };
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageProjectionFromTopologicalMapFilter.hxx"
#endif


#endif // __itkImageProjectionFromTopologicalMapFilter_h
