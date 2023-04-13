/*** bordertracking.cxx
*
* Main Program to extract cell borders information.
* truncated version of cell specialized for quantification by Yuki.
* Modified by Stephane
*
* use ITK 4
*
* @author Stephane Rigaud
* @version 06-06-2016
*
*/

#include "bordertracking.h"

void ComputeBorderFeatures(Border & _b, const ImageType * _image, Cell & _cell1, Cell & _cell2)
{
    
    std::pair<float, float> intensityValues = _b.MeanIntensity(_image, 4);
    
    _b.SetLength();
    _b.SetTotalIntensity(intensityValues.first);
    _b.SetNbDilatedPixels(intensityValues.second);
    _b.SetIntensityRatio(_cell1.GetMeanBorderIntensity(), _cell2.GetMeanBorderIntensity());
}

void ComputeCellFeatures(Border & _b, std::map<unsigned int, Cell> * _cellList, const ImageType * _rawImage, const ImageType * _labeledImage, const ImageType * _dilatedImage, const unsigned int _areaRadius)
{
    // define the region of interest around the border
    ImageType::IndexType  startIndex;
    ImageType::RegionType region;
    ImageType::SizeType   regionSize;
    
    startIndex[0] = UIRound(std::max(_b.GetCentroid().first  - _areaRadius, 0.0));
    startIndex[1] = UIRound(std::max(_b.GetCentroid().second - _areaRadius, 0.0));
    if (startIndex[0] + 2 * _areaRadius + 1 < Pixel::GetWidth())
    {
        regionSize[0] = 2 * _areaRadius + 1;
    }
    else
    {
        regionSize[0] = Pixel::GetWidth() - startIndex[0];    // out of bound
    }
    if (startIndex[1] + 2 * _areaRadius + 1 < Pixel::GetHeight())
    {
        regionSize[1] = 2 * _areaRadius + 1;
    }
    else
    {
        regionSize[1] = Pixel::GetHeight() - startIndex[1];    // out of bound
    }
    region.SetSize(regionSize);
    region.SetIndex(startIndex);
    
    // cells in the region
    std::vector<unsigned int> cellIds;
    unsigned int localIntensity = 0;
    unsigned int localNPixel = 0;
    double backgroundIntensity = 0;
    unsigned int nBackPixel = 0;
    
    // iterate on the cell region
    itk::ImageRegionConstIterator<ImageType> iteLabelImage(_labeledImage, region);     // iterator on the labels in the region
    itk::ImageRegionConstIterator<ImageType> iteRawImage(_rawImage, region);           // iterator on the pixels in the region
    itk::ImageRegionConstIterator<ImageType> iteSegmentedImage(_dilatedImage, region); // iterator on the skeleton in the region
    for (iteLabelImage.GoToBegin(), iteSegmentedImage.GoToBegin(), iteRawImage.GoToBegin(); !iteLabelImage.IsAtEnd(); ++iteLabelImage, ++iteSegmentedImage, ++iteRawImage)
    {
        if (iteSegmentedImage.Get() == 0) // accumulate intensity if on the skeleton
        {
            localIntensity += iteRawImage.Get();
            localNPixel++;
        }
        else if (find(cellIds.begin(), cellIds.end(), iteLabelImage.Get()) == cellIds.end())// cell not seen yet ...
        {
            std::map<unsigned int, Cell>::iterator iteCell = _cellList->find(iteLabelImage.Get());  // get corresponding cellobject
            if (iteCell != _cellList->end())
            {
                cellIds.push_back(iteLabelImage.Get());  // add in the list
                                                         // accumulate background intensity
                backgroundIntensity += iteCell->second.GetIntegratedIntensity();
                nBackPixel += iteCell->second.GetErodeArea();
            }
        }
    }
    
    // set local border intensity and local background intensity
    if (nBackPixel != 0)
    {
        _b.SetLocalBackground(backgroundIntensity / (double) nBackPixel);
    }
    _b.SetLocalTotalIntensity(localIntensity);
    _b.SetLocalNbPixels(localNPixel);
}

std::vector<Pixel> BuildSkeletonPixelList(ImageType * labelImage, CellMapType & cellMap, unsigned int width, unsigned int height, std::ofstream & file)
{
    
    // skeleton iteration setup
    ConstNeighborhoodIteratorType::RadiusType radius;
    radius.Fill(1);
    
    // Definition of the neighbor pixels on which the iteration will be done
    ConstNeighborhoodIteratorType::OffsetType offset1 = {{-1,-1}};
    ConstNeighborhoodIteratorType::OffsetType offset2 = {{-1, 0}};
    ConstNeighborhoodIteratorType::OffsetType offset3 = {{-1, 1}};
    ConstNeighborhoodIteratorType::OffsetType offset4 = {{ 0,-1}};
    ConstNeighborhoodIteratorType::OffsetType offset5 = {{ 0, 1}};
    ConstNeighborhoodIteratorType::OffsetType offset6 = {{ 1,-1}};
    ConstNeighborhoodIteratorType::OffsetType offset7 = {{ 1, 0}};
    ConstNeighborhoodIteratorType::OffsetType offset8 = {{ 1, 1}};
    
    Pixel                         skeletonPixel;
    std::string                   tmpString;
    std::vector<Pixel>            skeletonPixelList;
    ConstIteratorType             in(labelImage, labelImage->GetLargestPossibleRegion());         // iterator on the labeled image
    ConstNeighborhoodIteratorType it(radius, labelImage, labelImage->GetLargestPossibleRegion()); // iterate on the neighbor pixels of an iterator
    
    // Iteration on the skeleton of the labeled image
    unsigned int a, b, c, d, ipix;
    for(in.GoToBegin(), it.GoToBegin(), ipix = a = b = c = d = 0; !in.IsAtEnd(); ++in, ++it, ++ipix)
    {
        if (in.Get() == 0) // ensures that the iteration is made on the skeleton pixels
        {
            if (it.GetPixel(offset1) != 0) //check offset1
            {
                a = it.GetPixel(offset1);
            }
            if (it.GetPixel(offset2) != 0) //check offset2
            {
                if (a == 0)
                {
                    a = it.GetPixel(offset2);
                }
                else if (it.GetPixel(offset2) != a)
                {
                    b = it.GetPixel(offset2);
                }
            }
            if (it.GetPixel(offset3) != 0) //check offset3
            {
                if (a == 0)
                {
                    a = it.GetPixel(offset3);
                }
                else if (it.GetPixel(offset3) != a && b == 0)
                {
                    b = it.GetPixel(offset3);
                }
                else if (it.GetPixel(offset3) != a && it.GetPixel(offset3) != b)
                {
                    c = it.GetPixel(offset3);
                }
            }
            if (it.GetPixel(offset4) != 0) //check offset4
            {
                if (a == 0)
                {
                    a = it.GetPixel(offset4);
                }
                else if (it.GetPixel(offset4) != a && b == 0)
                {
                    b = it.GetPixel(offset4);
                }
                else if (it.GetPixel(offset4) != a && it.GetPixel(offset4) != b && c == 0)
                {
                    c = it.GetPixel(offset4);
                }
                else if (it.GetPixel(offset4) != a && it.GetPixel(offset4) != b && it.GetPixel(offset4) != c)
                {
                    d = it.GetPixel(offset4);
                }
            }
            if (it.GetPixel(offset5) != 0 && d == 0) //check offset5
            {
                if (a == 0)
                {
                    a = it.GetPixel(offset5);
                }
                else if (it.GetPixel(offset5) != a && b == 0)
                {
                    b = it.GetPixel(offset5);
                }
                else if (it.GetPixel(offset5) != a && it.GetPixel(offset5) != b && c == 0)
                {
                    c = it.GetPixel(offset5);
                }
                else if (it.GetPixel(offset5) != a && it.GetPixel(offset5) != b && it.GetPixel(offset5) != c)
                {
                    d = it.GetPixel(offset5);
                }
            }
            if (it.GetPixel(offset6) != 0 && d == 0) //check offset6
            {
                if (a == 0)
                {
                    a = it.GetPixel(offset6);
                }
                else if (it.GetPixel(offset6) != a && b == 0)
                {
                    b = it.GetPixel(offset6);
                }
                else if (it.GetPixel(offset6) != a && it.GetPixel(offset6) != b && c == 0)
                {
                    c = it.GetPixel(offset6);
                }
                else if (it.GetPixel(offset6) != a && it.GetPixel(offset6) != b && it.GetPixel(offset6) != c)
                {
                    d = it.GetPixel(offset6);
                }
            }
            if (it.GetPixel(offset7) != 0 && d == 0) //check offset7
            {
                if (a == 0)
                {
                    a = it.GetPixel(offset7);
                }
                else if (it.GetPixel(offset7) != a && b == 0)
                {
                    b = it.GetPixel(offset7);
                }
                else if (it.GetPixel(offset7) != a && it.GetPixel(offset7) != b && c == 0)
                {
                    c = it.GetPixel(offset7);
                }
                else if (it.GetPixel(offset7) != a && it.GetPixel(offset7) != b && it.GetPixel(offset7) != c)
                {
                    d = it.GetPixel(offset7);
                }
            }
            if (it.GetPixel(offset8) != 0 && d == 0) //check offset8
            {
                if (a == 0)
                {
                    a = it.GetPixel(offset8);
                }
                else if (it.GetPixel(offset8) != a && b == 0)
                {
                    b = it.GetPixel(offset8);
                }
                else if (it.GetPixel(offset8) != a && it.GetPixel(offset8) != b && c == 0)
                {
                    c = it.GetPixel(offset8);
                }
                else if (it.GetPixel(offset8) != a && it.GetPixel(offset8) != b && it.GetPixel(offset8) != c)
                {
                    d = it.GetPixel(offset8);
                }
            }
            if (b != 0) // to prevent 1-neighbour cases (a.k.a. bad skeletonization case)
            {
                skeletonPixel.Clear();
                skeletonPixel.Set(ipix, a, b, c, d);
                skeletonPixelList.push_back(skeletonPixel); // store in memory
                skeletonPixel.FillString(tmpString);          // write to file
                file << tmpString << std::endl;
            }
            else
            {
                std::pair<unsigned int, unsigned int> xy = indexToXY(ipix, width);
                std::cerr << "ERROR: Bad skeletonization ! at pixel x=" << xy.first << " y=" << xy.second << std::endl;
                throw -1;
            }
            a = b = c = d = 0; // reset
        }
        else // in.Get() !=0 (not on the border) -> check if border cell
        {
            int x = (int) in.GetIndex()[0];
            int y = (int) in.GetIndex()[1];
            unsigned int cellN = (unsigned int) in.Get();
            
            // look for cell number in the map
            CellMapIteratorType iteCellInFrame = cellMap.find(cellN);
            if (iteCellInFrame != cellMap.end()) // found
            {
                // if on the border & not set yet ...
                if ((iteCellInFrame->second.GetCategory() == -1) &&
                   ( x == 0 || x == width - 1 || y == 0 || y == height - 1 ))
                {
                    iteCellInFrame->second.SetCategory(Cell::BORDERCELL);
                }
            }
            else //not found
            {
                if ( x == 0 || x == width - 1 || y == 0 || y == height - 1 )
                {
                    cellMap.at(cellN).SetCategory(Cell::BORDERCELL);
                }
                else
                {
                    cellMap.at(cellN).SetCategory(Cell::DEFAULTCATEGORY);
                }
            }
        }
    } // end for each pixel of the image (skelpixellist)
    
    return skeletonPixelList;
}

/*** MAIN PROGRAM
 * * * * * * * * * * * * *
 * extracts & tracks the borders from the segmentation skeleton based on cell tracking results.
 * * * * * * * * * * * * *
 * arguments (must be given in the following order):
 *  (+) first image full path
 *  (+) cell tracking folder path
 *  (+) number of currentFrames
 *  (+) output directory path
 *  (+) first raw image full path
 *  (+) area Estimation Parameters
 *  (+) options (-[w|p|b])
 * * * * * * * * * * * * *
 * generates
 *  (*) patchedBorderTracking.txt ~containing the results of border tracking with a patch for border lost due to a neighbour division
 *  (*) bordersInfo_*.txt ~containing several infos of each border for currentFrame *~
 *  (*) SkeletonPixelsList_*.txt ~containing the list of pixels forming the segmentation skeleton for currentFrame *~ [can be disabled by writeAll=false]
 *  (*) BordersNeighbours_*.txt ~containing the neighbours relative & absolute id for each border in currentFrame *~ [can be disabled by writeAll=false]
 *  (*) cellsList_*.txt ~containing the cell list in currentFrame *~ [can be disabled by writeAll=false]
 *  (*) cellnums_*.txt ~containing the cell relative numbers for each pixel in currentFrame *~ [can be disabled by writeAll=false]
 *  (*) cellsPix_*.txt ~containing the list of pixel for each cell in currentFrame *~ [can be disabled by writeAll=false]
 * * * * * * * * * * * * *
 */
int main(int argc, char *argv[])
{
    std::cout << "version: " << version << " ::: compilation time: " << __DATE__ << " " << __TIME__ << std::endl;

    // first image, cell tracking files path, output directory, first raw image
    std::string firstImage;            // first image
    std::string correspondancePath;    // cell tracking files path
    std::string outputDirectory;       // output directory
    std::string firstrawImage;         // first raw image
    unsigned int nbCurrentFrames; // number of currentFrames
    int areaRadius;               // area of interest
    int seDilatationRadius;       // structuring element radius
    bool printAll = true;         // print in console
    bool writeAll = true;         // write in files (deprecated)
    bool borderIn = false;        // border cell included
    bool transposeFlag = false;

    itk::FixedArray<int, 2> order;
    if (transposeFlag)
    {
        order[0] = 1;
        order[1] = 0;
    }
    else
    {
        order[0] = 0;
        order[1] = 1;
    }

    /*
     *********************************************
     * READING ARGUMENTS
     *********************************************
     */

    // reading arguments
    if (argc < 5)
    {
        std::cerr << "Error: Missing arguments ... 5 arguments are required, " << argc - 1 << " provided !" << std::endl;
        return EXIT_FAILURE;
    }

    // argument index
    unsigned short iarg = 1;
    firstImage         = argv[iarg++];       // first image
    correspondancePath = argv[iarg++];       // folder containing the correspondance files
    nbCurrentFrames    = atoi(argv[iarg++]); // number of currentFrames   
    outputDirectory    = argv[iarg++];       // output directory (must exist)
    firstrawImage      = argv[iarg++];       // first raw image path
    std::string areaParam   = argv[iarg++];       // area estimator parameters (2012-03-30)
    size_t xloc = areaParam.find('x');
    if (xloc == std::string::npos)
    {
        std::cerr << "Invalid area Estimation Parameters ... format must be [area]x[dilation] !" << std::endl;
        return EXIT_FAILURE;
    }
    areaRadius = atoi(areaParam.substr(0, xloc).c_str());          // area of interest
    seDilatationRadius = atoi(areaParam.substr(xloc + 1).c_str()); // structuring element radius

    // additional options
    if (argc > iarg)
    {
        std::string options = argv[iarg++];
        if (options[0] == '-') // if not, options are ignored
        {
            if (options.find('p') != std::string::npos) // found
            {
                printAll = false;
            }
            if (options.find('w') != std::string::npos) // found
            {
                writeAll = false;
            }
            if (options.find('b') != std::string::npos) // found
            {
                borderIn = true;
            }
        }
        else
        {
            std::cerr << "Invalid option format ... format is -[p|w|b|m]" << std::endl;
        }
    }

    // raw and segmented image parsing
    // slicing segmentation image ...
    size_t found = firstImage.find_last_of(".");
    if (found == std::string::npos)
    {
        std::cerr << "Invalid segmentation file path ... extension not found!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string fileExtension = firstImage.substr(found); // file extension
    std::string fileRoot = firstImage.substr(0, found);
    found = fileRoot.find_last_of("_");
    if (found == std::string::npos)
    {
        std::cerr << "Invalid segmentation file path ... bad filename format! format must be [root]_[filenumber].[extension]" << std::endl;
        return EXIT_FAILURE;
    }
    std::string segFileRoot = fileRoot.substr(0, found + 1);        // file rootname
    std::string nFile_s = fileRoot.substr(found + 1);               // file number
    unsigned int nDigit = nFile_s.size();                      // number of digit
    int nFile = atoi(nFile_s.c_str());                         // first file number
    unsigned int lastCurrentFrame_u = nFile + nbCurrentFrames; // last currentFrame+1

    // slicing raw image ...
    found = firstrawImage.find_last_of(".");
    if (found == std::string::npos)
    {
        std::cerr << "Invalid raw file path ... extension not found!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string rawfileExtension = firstrawImage.substr(found);  // file extension
    std::string rawfileRoot = firstrawImage.substr(0, found);
    found = rawfileRoot.find_last_of("_");
    if (found == std::string::npos)
    {
        std::cerr << "Invalid raw file path ... bad filename format! format must be [root]_[filenumber].[extension]" << std::endl;
        return EXIT_FAILURE;
    }
    rawfileRoot = rawfileRoot.substr(0, found + 1);         // file rootname    

    /*
     ************************************************
        INITIALIZE DATA STRUCTURES
     ************************************************
    */

    // divided cells map (2011-11-02) 
    std::map<std::string, unsigned int>           dividedCells;
    std::map<std::string, unsigned int>::iterator iteDividedCells;

    // skeleton iteration setup (2012-05-24)
    ConstNeighborhoodIteratorType::RadiusType radius;
    radius.Fill(1);

    // Definition of the neighbor pixels on which the iteration will be done
    //ConstNeighborhoodIteratorType::OffsetType offset1 = {{-1,-1}};
    //ConstNeighborhoodIteratorType::OffsetType offset2 = {{-1, 0}};
    //ConstNeighborhoodIteratorType::OffsetType offset3 = {{-1, 1}};
    //ConstNeighborhoodIteratorType::OffsetType offset4 = {{ 0,-1}};
    //ConstNeighborhoodIteratorType::OffsetType offset5 = {{ 0, 1}};
    //ConstNeighborhoodIteratorType::OffsetType offset6 = {{ 1,-1}};
    //ConstNeighborhoodIteratorType::OffsetType offset7 = {{ 1, 0}};
    //ConstNeighborhoodIteratorType::OffsetType offset8 = {{ 1, 1}};

    unsigned int width  = 0; // image width (to be set later)
    unsigned int height = 0; // image height (to be set later)

    // for benchmark
    clock_t t_start, t_end;
    float time_time;
    // bench start
    t_start = clock();
    
    std::map<MapKeyType, Border> *test = new std::map<MapKeyType, Border>[nbCurrentFrames];     // array to store all the borders infos
    std::map<unsigned int, std::string> *tagMap = new std::map<unsigned int, std::string>[nbCurrentFrames]; // array to store the cell correspondance
    std::vector<std::string> *coalescedCells = new std::vector<std::string>[nbCurrentFrames];               // coalesced cells array (2012-10-08)

    // create structuring element
    StructuringElementType structuringElement;
    StructuringElementType::SizeType seRadius;
    seRadius.Fill(seDilatationRadius);
    structuringElement.SetRadius(seRadius);
    structuringElement.CreateStructuringElement();

    // ball of radius 1 for simple erosion
    StructuringElementType ball;
    StructuringElementType::SizeType radiusball;
    radiusball.Fill(1);
    ball.SetRadius(radiusball);
    ball.CreateStructuringElement();

    /*
     ***********************************************************************************************
     * PART 1 : Border Tracking
     * tracking of cells border using the cell tracking output and the segmentation skeleton.
     ***********************************************************************************************
     */

    for (unsigned int currentFrame = nFile; currentFrame < lastCurrentFrame_u; currentFrame++)
    {
        if (printAll)
        {
            std::cout << currentFrame << ": Image Reading ......\t....." << std::endl;
        }
        
        // Image name construction
        std::string segmentedImageName = (segFileRoot + numberToString(currentFrame, nDigit) + fileExtension).c_str();
        std::string rawImageName = (rawfileRoot + numberToString(currentFrame, nDigit) + rawfileExtension).c_str();
        
        // Load segmentation image
        ReaderType::Pointer segmentedImageReader = ReaderType::New();
        segmentedImageReader->SetFileName(segmentedImageName);
        TransposeImageFilterType::Pointer transposeSegmentedImage = TransposeImageFilterType::New();
        transposeSegmentedImage->SetInput(segmentedImageReader->GetOutput());
        transposeSegmentedImage->SetOrder(order);
        try
        {
            segmentedImageReader->Update();
        }
        catch( itk::ExceptionObject &excp )
        {
            std::cerr << "\tERROR!\n\tUnable to open " << segmentedImageName << std::endl;
            std::cerr << excp << std::endl;
            return EXIT_FAILURE;
        }

        // Load raw image
        ReaderType::Pointer rawImageReader = ReaderType::New();
        rawImageReader->SetFileName(rawImageName);
        TransposeImageFilterType::Pointer transposeRawImage = TransposeImageFilterType::New();
        transposeRawImage->SetInput(rawImageReader->GetOutput());
        transposeRawImage->SetOrder(order);
        try
        {
            transposeRawImage->Update();
        }
        catch( itk::ExceptionObject &excp )
        {
            std::cerr << "\tERROR!\n\tUnable to open " << rawImageName << std::endl;
            std::cerr << excp << std::endl;
            return EXIT_FAILURE;
        }

        // smart pointer to raw image
        ImageType::Pointer rawImage = transposeRawImage->GetOutput();
        
        // set width & height if first currentFrame
        if (currentFrame == nFile)
        {
            ImageType::SizeType size = transposeRawImage->GetOutput()->GetBufferedRegion().GetSize();
            width  = size.GetElement(0);
            height = size.GetElement(1);
            Pixel::SetWidth(width);
            Pixel::SetHeight(height);
        }
        
        
        
        if (printAll)
        {
            std::cout << currentFrame << ": Image Labelisation ......\t....." << std::endl;
        }
  
        // Apply Connected Component Filter
        labelFilterType::Pointer labelFilter = labelFilterType::New();  
        labelFilter->SetInput(transposeSegmentedImage->GetOutput());
        labelFilter->ReleaseDataFlagOn();
        labelFilter->Update();
        
        // Erode segimage (i.e. dilate the borders)
        BinaryErodeFilterType::Pointer erodeFilter = BinaryErodeFilterType::New();
        erodeFilter->SetInput(transposeSegmentedImage->GetOutput());
        erodeFilter->SetKernel(structuringElement);
        erodeFilter->SetForegroundValue(255);
        erodeFilter->SetBackgroundValue(0);
        erodeFilter->ReleaseDataFlagOn();
        erodeFilter->Update();

        // Apply Label Filter with the eroded image
        labelFilterType::Pointer erodeLabelFilter = labelFilterType::New();
        erodeLabelFilter->SetInput(erodeFilter->GetOutput());
        erodeLabelFilter->ReleaseDataFlagOn();
        erodeLabelFilter->Update();

        // Apply Labels Geometry Filter on the eroded label image
        GeometryFilterType::Pointer erodeGeometryFilter = GeometryFilterType::New();
        erodeGeometryFilter->SetInput(erodeLabelFilter->GetOutput());
        erodeGeometryFilter->SetIntensityInput(rawImage);
        erodeGeometryFilter->ReleaseDataFlagOn();
        erodeGeometryFilter->Update();

        // Dilate segimage
        BinaryErodeFilterType::Pointer simpleErosion = BinaryErodeFilterType::New();
        simpleErosion->SetInput(transposeSegmentedImage->GetOutput());
        simpleErosion->SetKernel(ball);
        simpleErosion->SetForegroundValue(255);
        simpleErosion->SetBackgroundValue(0);
        simpleErosion->ReleaseDataFlagOn();
        simpleErosion->Update();


        
        if (printAll)
        {
            std::cout << currentFrame << ": Exploring Skeleton ......\t....." << std::endl;
        }

        std::vector<Pixel> skeletonPixelList;                          // vector to store the data
        ImageType::Pointer labeledITKImage = labelFilter->GetOutput(); // abeled ITK image pointer
        CellMapType        cellInCurrentFrame;                         // map of cells

        // Build the list of pixel from the skeleton, and set cell categories
        std::ofstream borderPixel;
        borderPixel.open((outputDirectory + "/skeletonPixelsList_" + intToString(currentFrame) + ".txt").c_str());
        skeletonPixelList = BuildSkeletonPixelList(labeledITKImage, cellInCurrentFrame, width, height, borderPixel);
        borderPixel.close();
        
        // Set integrated intensity and eroded area
        GeometryFilterType::LabelsType cellLabels = erodeGeometryFilter->GetLabels();
        for(GeometryFilterType::LabelsType::iterator iteCellLabels = cellLabels.begin(); iteCellLabels != cellLabels.end(); iteCellLabels++)
        {
            CellMapIteratorType iteCellInFrame = cellInCurrentFrame.find(*iteCellLabels);
            if (iteCellInFrame != cellInCurrentFrame.end()) // found
            {
                iteCellInFrame->second.SetErodeArea(erodeGeometryFilter->GetVolume(*iteCellLabels));
                iteCellInFrame->second.SetIntegratedIntensity(erodeGeometryFilter->GetIntegratedIntensity(*iteCellLabels));
            }
        }

        /*
         *****************************************
         * BUILD BORDERS
         *****************************************
         */

        if (printAll)
        {
            std::cout << currentFrame << ": Extracting borders ......\t....." << std::endl;
        }

        // iterate on the skeleton pixels to build the borders
        unsigned int nbBorders = 0;
        std::vector<std::pair<unsigned int, unsigned int> > neighboursCouples;
        std::vector<Pixel>::iterator iteSkeletonPixel;
        for (iteSkeletonPixel = skeletonPixelList.begin(); iteSkeletonPixel != skeletonPixelList.end(); iteSkeletonPixel++)
        {
            neighboursCouples.clear();
            
            // list of all neighbour couple possible for pix
            iteSkeletonPixel->FillNeighboursCoupleList(neighboursCouples);
            
            // for every neighbour couple possible
            std::vector<std::pair<unsigned int, unsigned int> >::iterator iteNCouples;
            for (iteNCouples = neighboursCouples.begin(); iteNCouples != neighboursCouples.end(); iteNCouples++)
            {
                unsigned int key1 = iteNCouples->first;
                unsigned int key2 = iteNCouples->second;

                // exlude borders involving a border cell
                bool borderFlag1 = cellInCurrentFrame[key1].GetCategory() == Cell::BORDERCELL;
                bool borderFlag2 = cellInCurrentFrame[key2].GetCategory() == Cell::BORDERCELL;
                if (!borderIn && (borderFlag1 || borderFlag2))
                {
                    continue;
                }

                // add the pixel in the corresponding border, the lower key first
                MapKeyType keyM;
                if (key1 < key2)
                {
                    keyM = std::make_pair(key1, key2);
                }
                else
                {
                    keyM = std::make_pair(key2, key1);
                }

                // find corresponding border
                std::map<MapKeyType, Border>::iterator iteTest = test[currentFrame - nFile].find(keyM);
                if (iteTest != test[currentFrame - nFile].end()) // found
                {
                    iteTest->second.AddPixel(*iteSkeletonPixel);
                }
                else // not found -> create it !
                {
                    test[currentFrame - nFile][keyM].Set(++nbBorders, keyM.first, keyM.second);
                    test[currentFrame - nFile][keyM].AddPixel(*iteSkeletonPixel);
                }
            }
        }

        
        // verification of border integrity
        std::map<MapKeyType, Border>::iterator iteTest;
        for (iteTest = test[currentFrame - nFile].begin(); iteTest != test[currentFrame - nFile].end(); /*incrementation in loop*/)
        {
            if (!iteTest->second.IntegrityTest()) // the border is composed of two, we split
            {
                // save identification for the first and second segment
                MapKeyType currentKeyM = iteTest->first;
                unsigned int id = iteTest->second.GetId();
                MapKeyType additionalKeyM = std::make_pair(currentKeyM.second, currentKeyM.first);
                unsigned int newId = ++nbBorders;

                // get pixel list of both segment
                std::vector<Pixel> segment1 = iteTest->second.SortPixel();
                std::vector<Pixel> segment2 = iteTest->second.Difference(segment1);

                // erase current segment
                std::map<MapKeyType, Border>::iterator toBeDelete = iteTest;
                iteTest++;
                test[currentFrame - nFile].erase(toBeDelete);

                // save new segment
                test[currentFrame - nFile][currentKeyM].Set(id, currentKeyM.first, currentKeyM.second);
                test[currentFrame - nFile][currentKeyM].SetPixels(segment1);
                test[currentFrame - nFile][additionalKeyM].Set(newId, additionalKeyM.first, additionalKeyM.second);
                test[currentFrame - nFile][additionalKeyM].SetPixels(segment2);
            }
            else
            {
                iteTest++;
            }
        }
        
        
        
        // Extract border features
        std::pair<double, double> centroid;
        std::pair<unsigned int, unsigned int> intensityValues;
        CellMapIteratorType iteCellInFrame;
        for (iteTest = test[currentFrame - nFile].begin(); iteTest != test[currentFrame - nFile].end(); iteTest++)
        {
            // group borders per cell
            // first neighbour
            iteCellInFrame = cellInCurrentFrame.find(iteTest->first.first);
            if (iteCellInFrame != cellInCurrentFrame.end()) // found
            {
                iteCellInFrame->second.AddBorder(iteTest->second);
            }
            else // not found
            {
                // this code shoud not be reached !
                std::cerr << "ERROR: first neighbour not found!" << std::endl;
                return EXIT_FAILURE;
            }

            // second neighbour
            iteCellInFrame = cellInCurrentFrame.find(iteTest->first.second);
            if (iteCellInFrame != cellInCurrentFrame.end()) // found
            {
                iteCellInFrame->second.AddBorder(iteTest->second);
            }
            else // not found
            {
                // this code shoud not be reached !
                std::cerr << "ERROR: first neighbour not found!" << std::endl;
                return EXIT_FAILURE;
            }

            // compute feature here using the border methode of the same name
            Cell firstCellNeighbour  = cellInCurrentFrame[iteTest->first.first];
            Cell secondCellNeighbour = cellInCurrentFrame[iteTest->first.second];
            ComputeBorderFeatures(iteTest->second, rawImage, firstCellNeighbour, secondCellNeighbour);
            ComputeCellFeatures(iteTest->second, &cellInCurrentFrame, rawImage, labeledITKImage, simpleErosion->GetOutput(), areaRadius);
        }

        
        // print the borderMap
        std::ofstream borderPixel3((outputDirectory + "/bordersInfo2_" + intToString(currentFrame) + ".txt").c_str());
        for (iteTest = test[currentFrame - nFile].begin(); iteTest != test[currentFrame - nFile].end(); iteTest++)
        {
            borderPixel3 << iteTest->second.ToString() << std::endl;
        }
        borderPixel3.close();

        /*
         *****************************************
         * READING CELL TRACKING
         *****************************************
         */



        // map to store the correspondance file content
        CellTrackingMapType cellMap;

        if (nbCurrentFrames != 1) // skip if only one currentFrame
        {
            if (printAll)
            {
                std::cout << currentFrame << ": Reading Cell Tracking ...\t....." << std::endl;
            }

            // read the correspondance file
            std::ifstream corres((correspondancePath + "/correspondence_" + intToString(currentFrame) + ".txt").c_str());
            if (corres)
            {
                std::string ligne;
                size_t      found;
                CellTrackingMapIteratorType iteCorrespondanceMap;

                while (getline(corres, ligne))
                {
                    found = ligne.find_first_of(" ");
                    if (found == std::string::npos)
                    {
                        std::cerr << "ERROR: irregular correspondance file format: unable to read " << correspondancePath + "/correspondence_" + intToString(currentFrame) + ".txt" << std::endl;
                        return EXIT_FAILURE;
                    }
                    std::string relativeN = ligne.substr(0, found);
                    std::string absoluteN = ligne.substr(found + 1);
                    absoluteN = chomp(absoluteN); // remove white space and new line at the end of the std::string

                    // replace all the white spaces by an underscore (for cells born by division)
                    found = absoluteN.find_first_of(" ");
                    while (found != std::string::npos)
                    {
                        absoluteN[found] = '_';
                        found = absoluteN.find_first_of(" ");
                    }

                    //add to the divided cells map if divided cell
                    found = absoluteN.find_last_of('_');
                    if (found != std::string::npos)
                    {
                        std::string mother = absoluteN.substr(0, found);
                        iteDividedCells = dividedCells.find(mother);
                        // add to the map if not found
                        if (iteDividedCells == dividedCells.end())
                        {
                            dividedCells[mother] = currentFrame;
                        }
                    }
                    // relative cell id
                    int key = atoi(relativeN.c_str());
                    // fill the correspondance map
                    cellMap[key].push_back(absoluteN);
                }
            }
            else
            {
                std::cerr << "\tERROR!\n\tUnable to open " << correspondancePath + "/correspondence_" + intToString(currentFrame) + ".txt" << std::endl;
                return EXIT_FAILURE;
            }
            corres.close();

            /*
             * fill the tag map
             */
            for (CellTrackingMapIteratorType iteCellMap = cellMap.begin(); iteCellMap != cellMap.end(); iteCellMap++)
            {
                tagMap[currentFrame - nFile][iteCellMap->first] = tagVectorToString(iteCellMap->second, true);
                if (iteCellMap->second.size() > 1)
                {
                    coalescedCells[currentFrame - nFile].push_back(tagMap[currentFrame - nFile][iteCellMap->first]);
                }
            }
            cellMap.clear();


            /*
             * Set Borders Absolute Tag
             */
            if (printAll)
            {
                std::cout << currentFrame << ": Setting Absolute Tags ...\t....." << std::endl;
            }

            // iterate on all the borders
            for(iteTest = test[currentFrame - nFile].begin(); iteTest != test[currentFrame - nFile].end(); iteTest++)
            {
                std::string s1, s2;
                // look for first neighbour tag
                std::map<unsigned int, std::string>::iterator iteTagMap = tagMap[currentFrame - nFile].find(iteTest->first.first);
                if (iteTagMap != tagMap[currentFrame - nFile].end())
                {
                    s1 = iteTagMap->second;
                }
                else
                {
                    // this part of code should never be reached ..
                    std::cerr << "key1=" << iteTest->first.first << " not found ! (impossible case !? at currentFrame " << currentFrame << ")" << std::endl;
                    return EXIT_FAILURE;
                }
                // set first neighbour absolute tag
                iteTagMap = tagMap[currentFrame - nFile].find(iteTest->first.second);
                if (iteTagMap != tagMap[currentFrame - nFile].end())
                {
                    s2 = iteTagMap->second;
                }
                else
                {
                    // this part of code should never be reached ..
                    std::cerr << "key2=" << iteTest->first.second << " not found ! (impossible case !? at currentFrame " << currentFrame << ")" << std::endl;
                    return EXIT_FAILURE;
                }
                // set absolute tag
                if (cmpTAG(s1, s2))
                {
                    iteTest->second.SetAbsoluteTag("[" + s1 + " " + s2 + "]");
                }
                else
                {
                    iteTest->second.SetAbsoluteTag("[" + s2 + " " + s1 + "]");
                }
            }

            /*
             * print BordersNeighbbours
             */
            if (writeAll)
            {
                // print the border neighbours
                std::ofstream borderPixel4((outputDirectory + "/BordersNeighbours_" + intToString(currentFrame) + ".txt").c_str());
                for(iteTest = test[currentFrame - nFile].begin(); iteTest != test[currentFrame - nFile].end(); iteTest++)
                {
                    borderPixel4 << iteTest->second.GetId() << "," << iteTest->second.GetFirstNeighbour() << "," << iteTest->second.GetSecondNeighbour() << ",";
                    borderPixel4 << tagMap[currentFrame - nFile][iteTest->second.GetFirstNeighbour()] << "," << tagMap[currentFrame - nFile][iteTest->second.GetSecondNeighbour()] << std::endl;
                }
                borderPixel4.close();
            }
        }
        else if (writeAll) // print the file for the unique currentFrame
        {
            /*
             * print the border neighbours
             */
            std::ofstream borderPixel4((outputDirectory + "/BordersNeighbours_" + intToString(currentFrame) + ".txt").c_str());
            for(iteTest = test[currentFrame - nFile].begin(); iteTest != test[currentFrame - nFile].end(); iteTest++)
            {
                borderPixel4 << iteTest->second.GetId() << "," << iteTest->second.GetFirstNeighbour() << "," << iteTest->second.GetSecondNeighbour() << ",";
                borderPixel4 << iteTest->second.GetFirstNeighbour() << "," << iteTest->second.GetSecondNeighbour() << std::endl;
            }
            borderPixel4.close();
        }
        // end of nbCurrentFrames!=1
    } // end of for(currentFrame ...

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------ TRACKING --------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

    if (nbCurrentFrames != 1) //2012-05-11
    {
        /*
         *********************************************
         * TRACK THE BORDERS
         *********************************************
         */
        if (printAll)
        {
            std::cout << ".....................................\nTracking Cell Borders ..............." << std::endl;
        }

        // tracking map
        std::map<std::string, Border **> trackMap3;

        // iterate on all currentFrames
        for(unsigned int currentFrame = 0; currentFrame < nbCurrentFrames; currentFrame++)
        {
            // iterate on all borders in the currentFrame
            for (std::map<MapKeyType, Border>::iterator iteTest = test[currentFrame].begin(); iteTest != test[currentFrame].end(); iteTest++)
            {
                std::map<std::string, Border **>::iterator iteTrackMap3 = trackMap3.find(iteTest->second.GetAbsoluteTag());
                if (iteTrackMap3 != trackMap3.end())
                {
                    iteTrackMap3->second[currentFrame] = &(iteTest->second);    // set pointer to the border
                }
                else
                {
                    Border * *arr = new Border*[nbCurrentFrames]; //create a new array to store the new track
                    for(unsigned int i = 0; i < nbCurrentFrames; i++)
                    {
                        arr[i] = 0;
                    }
                    arr[currentFrame] = &(iteTest->second);
                    trackMap3[iteTest->second.GetAbsoluteTag()] = arr;
                }
            }
        }

        /*
         *********************************************
         * GENERATE & PRINT the patched tracking results.
         *********************************************
         */
        if (printAll)
        {
            std::cout << "Patching Cell Tracking .............." << std::endl;
        }

        std::ofstream stracks4((outputDirectory + "/patchedBorderTracking.txt").c_str());

        std::ofstream stracks5((outputDirectory + "/modifiedBorderTracking.txt").c_str());
        std::set<std::string> dejaVu;

        // iterate on all the tracks
        for (std::map<std::string, Border **>::iterator iteTrackMap3 = trackMap3.begin(); iteTrackMap3 != trackMap3.end(); iteTrackMap3++)
        {
            if (find(dejaVu.begin(), dejaVu.end(), iteTrackMap3->first) != dejaVu.end())
            {
                continue ;    //deja vu, go to next iteration
            }

            //tracking array
            Border * *trackElementt = iteTrackMap3->second;

            // not dejaVu case
            for(unsigned int i = 0; i < nbCurrentFrames; i++)
            {
                if (trackElementt[i] != 0)
                {
                    stracks4 << trackElementt[i]->GetId();
                    stracks5 << trackElementt[i]->GetId();
                }
                else // track lost ... trying to patch ...
                {
                    if (i != 0 && trackElementt[i - 1] != 0)
                    {
                        // first neighbour tag
                        std::string n1 = tagMap[i - 1][trackElementt[i - 1]->GetFirstNeighbour()];
                        // second neighbour tag
                        std::string n2 = tagMap[i - 1][trackElementt[i - 1]->GetSecondNeighbour()];

                        std::string n12[2] = {n1, n2};

                        // no patching for coalesced cells!
                        if ( n1.find('(') != std::string::npos || n2.find('(') != std::string::npos )
                        {
                            stracks4 << 0;
                            stracks5 << 'N';
                        }
                        else // let's look for division
                        {
                            // check for fusion (2012-10-08)
                            bool fused = false;
                            // look in for a coalescence ...
                            for(std::vector<std::string>::iterator iteCoalescedCell = coalescedCells[i].begin(); iteCoalescedCell != coalescedCells[i].end(); iteCoalescedCell++)
                                if ((iteCoalescedCell->find("(" + n1 + " ") != std::string::npos || iteCoalescedCell->find(" " + n1 + ")") != std::string::npos || iteCoalescedCell->find(" " + n1 + " ") != std::string::npos)
                                        && (iteCoalescedCell->find("(" + n2 + " ") != std::string::npos || iteCoalescedCell->find(" " + n2 + ")") != std::string::npos || iteCoalescedCell->find(" " + n2 + " ") != std::string::npos))
                                {
                                    fused = true;
                                    break;
                                }
                            if (fused)
                            {
                                stracks4 << 0;
                                stracks5 << 'F';
                            }
                            else
                            {
                                unsigned short ni = 0;
                                // tags array
                                std::string tags[8] = {"[" + n1 + "_1 " + n2 + "]", "[" + n1 + "_2 " + n2 + "]", "[" + n1 + " " + n2 + "_1]", "[" + n1 + " " + n2 + "_2]",
                                                  "[" + n2 + " " + n1 + "_1]", "[" + n2 + " " + n1 + "_2]", "[" + n2 + "_1 " + n1 + "]", "[" + n2 + "_2 " + n1 + "]"
                                                 };

                                bool divided = false;

                                // double division case
                                int niDivision[2] = { -1, -1};

                                while(!divided && ni < 2)
                                {
                                    std::map<std::string, unsigned int>::iterator iteDividedCell = dividedCells.find(n12[ni]);
                                    // ni division detected before or at current currentFrame
                                    if ( iteDividedCell != dividedCells.end() && (iteDividedCell->second <= i + nFile) )
                                    {
                                        // keep division time
                                        niDivision[ni] = iteDividedCell->second - nFile;

                                        std::map<std::string, Border **>::iterator itKeep = trackMap3.end();

                                        unsigned short nFound = 0;
                                        // look for tags
                                        for(unsigned int t = 0; t < 2; t++)
                                        {
                                            //cerr<<"looking for :"<<tags[t+2*ni]<<flush;

                                            // look for tag
                                            std::map<std::string, Border **>::iterator itTmp = trackMap3.find(tags[t + 2 * ni]);
                                            // if not found, look for the reversed version
                                            if (itTmp == trackMap3.end())
                                            {
                                                itTmp = trackMap3.find(tags[4 + t + 2 * ni]);
                                            }

                                            // tag found
                                            if (itTmp != trackMap3.end())
                                            {
                                                //cerr<<" found!"<<std::endl;
                                                // look for first appearence
                                                unsigned int fnb = 0;

                                                for(unsigned int j = i; j < nbCurrentFrames; j++)
                                                {
                                                    if (itTmp->second[j] != 0)
                                                    {
                                                        fnb = j;
                                                        //cerr<<"first appearence: "<<fnb<<std::endl;
                                                        break;
                                                    }
                                                }
                                                // 2011-12-08: ignore if in future
                                                if (fnb > i)
                                                {
                                                    continue;
                                                }
                                                // first tag found
                                                if (itKeep == trackMap3.end())
                                                {
                                                    itKeep = itTmp;
                                                    nFound++;
                                                    // division found
                                                    divided = true;
                                                    //cerr<<"division found !"<<std::endl;
                                                }
                                                else // another tag found
                                                {
                                                    nFound++;
                                                    stracks4 << 0; // cut by bridge
                                                    stracks5 << 'C';
                                                    break;
                                                }
                                            }
                                            //else cerr<<" not found!"<<std::endl;
                                        }
                                        // only one found
                                        if (nFound == 1)
                                        {
                                            //cerr<<"change of to track !"<<itKeep->first<<std::endl;
                                            // change track
                                            trackElementt = itKeep->second;
                                            // dejaVu
                                            dejaVu.insert(itKeep->first);
                                            // print ID
                                            if (trackElementt[i] != 0)
                                            {
                                                stracks4 << trackElementt[i]->GetId();
                                                stracks5 << trackElementt[i]->GetId();
                                            }
                                            else // divided but lost ...
                                            {
                                                //cerr<<"sorry i'm lost ..."<<std::endl;
                                                stracks4 << 0;
                                                stracks5 << '0';
                                            }
                                        }
                                    }
                                    ni++; // next
                                }
                                // dual division
                                if ((niDivision[0] == i) && (niDivision[1] == i))
                                {
                                    // re-use of tags
                                    tags[0] = "[" + n1 + "_1 " + n2 + "_1]";
                                    tags[1] = "[" + n1 + "_2 " + n2 + "_1]";
                                    tags[2] = "[" + n1 + "_1 " + n2 + "_2]";
                                    tags[3] = "[" + n1 + "_2 " + n2 + "_2]";
                                    std::map<std::string, Border **>::iterator iteTrackKeep = trackMap3.end();
                                    unsigned short nbfound = 0;
                                    for(unsigned int j = 0; j < 4 && nbfound < 2; j++)
                                    {
                                        std::map<std::string, Border **>::iterator iteTrackMapDual = trackMap3.find(tags[j]);
                                        if (iteTrackMapDual != trackMap3.end()) // found
                                        {
                                            nbfound++;
                                            if (iteTrackKeep == trackMap3.end())
                                            {
                                                iteTrackKeep = iteTrackMapDual;
                                            }
                                        }
                                    }
                                    if (nbfound == 1)
                                    {
                                        // change track
                                        trackElementt = iteTrackKeep->second;
                                        // dejaVu
                                        dejaVu.insert(iteTrackKeep->first);
                                        // print ID
                                        if (trackElementt[i] != 0)
                                        {
                                            stracks4 << trackElementt[i]->GetId();
                                            stracks5 << trackElementt[i]->GetId();
                                        }
                                        else
                                        {
                                            stracks4 << 0; // lost but divided ...
                                            stracks5 << '0';
                                        }
                                        // division found
                                        divided = true;
                                    }
                                }
                                // no division found -> simply lost track ...
                                if (!divided)
                                {
                                    stracks4 << 0;
                                    stracks5 << 0;
                                }
                            }
                        }// end of division check
                    }
                    else
                    {
                        stracks4 << 0; // first currentFrame or no track before case
                        stracks5 << 0;
                    }
                }
                // add space before next element
                stracks4 << " ";
                stracks5 << " ";
            } // end of for i
            // new line for next border
            stracks4 << std::endl;
            stracks5 << std::endl;
        } // end of for itTrackMap2
        stracks4.close();
        stracks5.close();
    } //end of nbCurrentFrames!=1

    /* ------------------------------------------------------------------------------ */
    /* ------------------------------------------------------------------------------ */
    /* ------------------------------------------------------------------------------ */

    //bench end
    t_end = clock();
    // time elapsed
    time_time = (t_end - t_start) / (double)CLOCKS_PER_SEC;
    if (printAll)
    {
        std::cout << "....................................." << std::endl;
        std::cout << "(*_*)\tJob Done in " << time_time << "sec." << "\t(*_*)" << std::endl;
        std::cout << "....................................." << std::endl;
    }
    return EXIT_SUCCESS;
}
// end of file
