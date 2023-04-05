/*** Class Border
*
* Represents a border between 2 cells
* A border is composed of a list of pixel
*
* @author Stephane Rigaud
* @version 2016-05-27
*/

#include "border.h"

/***
 * Constructor
 */


/*** constructor
 * default
 */
Border::Border()
{
    m_Id = 0;
    m_Neighbours[0] = 0;
    m_Neighbours[1] = 0;
    m_AbsoluteTag = "";
}


/*** 
 * Operator
 */

bool Border::operator== (const Border & _b) const
{
    return (this->GetId() == _b.GetId());
}

bool Border::operator== (const unsigned int _id) const
{
    return (this->GetId() == _id);
}

/***
 * Get
 */


/*** Get id
 * @return the border's id
 */
unsigned int Border::GetId() const
{
    return m_Id;
}

/*** Get the pixels
 * @return the border's pixels
 */
Border::PixelListType Border::GetPixelList() const
{
    return m_PixelList;
}

/*** Get first neighbour
 * @return the first neighbour
 */
unsigned int Border::GetFirstNeighbour() const
{
    return m_Neighbours[0];
}

/*** Get the second neighbour
 * @return the second neighbour
 */
unsigned int Border::GetSecondNeighbour() const
{
    return m_Neighbours[1];
}

/*** Get neighbours
 * @return the neighbours
 */
unsigned int * Border::GetNeighbours()
{
    return m_Neighbours;
}

/*** Get number of vertex in the border
 */
unsigned int Border::GetNbVertex() 
{
    if (m_NbVertex == 0)
    {
        this->SetNbVertex();
    }
    return m_NbVertex;
}

/*** Get the tag
 * @return the border's tag
 */
std::string Border::GetAbsoluteTag() const
{
    return m_AbsoluteTag;
}

/*** Get the mean intensity
* @return the border's mean intensity
*/
double Border::GetMeanIntensity() const
{
    double meanIntensity = 0;
    if (m_NbDilatedPixels != 0)
    {
        meanIntensity = m_TotalIntensity / (double) m_NbDilatedPixels;
    }
    return meanIntensity;
}


/***
 * Set
 */


/*** initial Setter
 * @param idvalue the border id
 * @param firstvalue the first neighbour id
 * @param secondvalue the neighbour id
 */
void Border::Set(const unsigned int _id, const unsigned int _first, const unsigned int _second)
{
    m_Id = _id;
    m_Neighbours[0] = _first;
    m_Neighbours[1] = _second;
}

/*** set pixel list
 * @param _pixelList the new pixel list
 */
void Border::SetPixels(const PixelListType & _pixelList)
{
    m_PixelList = _pixelList;
    this->SetNbVertex();
}

/*** add a pixel to the border
 * @param val the pixel index
 * @param isv true if is a vertex
 */
void Border::AddPixel(const Pixel & _p)
{
    m_PixelList.push_back(_p);
    if (_p.IsVertex())
    {
        m_NbVertex += 1;
    }
}


/*** Set absolute tag
 * @param tag the tag
 */
void Border::SetAbsoluteTag(const std::string _tag)
{
    m_AbsoluteTag = _tag;
}

/*** Count and set number of vertex in the border
 */
void Border::SetNbVertex()
{
    unsigned int cpt = 0;
    PixelListConstIteratorType it = m_PixelList.begin();
    while (it != m_PixelList.end())
    {
        if (it->IsVertex())
        {
            cpt += 1;
        }
        it++;
    }
    m_NbVertex = cpt;
}

void Border::SetLength() {}

void Border::SetAngle() 
{
    float dx = this->GetPixelList().front().GetCoordinate().first - this->GetPixelList().back().GetCoordinate().first;
    m_Angle = acos(dx / m_Cord);
}
void Border::SetCord() 
{
    float dx = this->GetPixelList().front().GetCoordinate().first  - this->GetPixelList().back().GetCoordinate().first;
    float dy = this->GetPixelList().front().GetCoordinate().second - this->GetPixelList().back().GetCoordinate().second;
    m_Cord = std::sqrt(dx * dx + dy * dy);
}
void Border::SetCurvature()
{
    m_Curvature = 0;
    CoordinatePixelType firstExt = this->GetPixelList().front().GetCoordinate();
    CoordinatePixelType secondExt = this->GetPixelList().back().GetCoordinate();
    CoordinatePixelType xy;

    float dx = firstExt.first  - secondExt.first;
    float dy = firstExt.second - secondExt.second;
    if (dy == 0.0) // cord perfectly horizontal
    {
        for (PixelListIteratorType it = ++m_PixelList.begin(); it != --m_PixelList.end(); it++)
        {
            xy = it->GetCoordinate();
            m_Curvature += (secondExt.second - xy.second) * (secondExt.second - xy.second);
        }
    }
    else if (dx == 0.0) // cord perfectly vertical
    {
        for (PixelListIteratorType it = ++m_PixelList.begin(); it != --m_PixelList.end(); it++)
        {
            xy = it->GetCoordinate();
            m_Curvature += (secondExt.first - xy.first) * (secondExt.first - xy.first);
        }
    }
    else // default case
    {
        // cord equation: ax+b
        float a = dy / dx;                                // 'coefficient directeur' of cord
        float b = secondExt.second - a * secondExt.first; // y-intercept of cord
        float c = -1.0 / a;                               // `arrow is orthogonal to cord
        for (PixelListIteratorType it = ++m_PixelList.begin(); it != --m_PixelList.end(); it++)
        {
            xy = it->GetCoordinate();
            float d = xy.second - c * xy.first; // y-intercept of `arrow
            float xp = (d - b) / (a - c);
            float yp = c * xp + d;
            m_Curvature += (xp - xy.first) * (xp - xy.first) + (yp - xy.second) * (yp - xy.second);
        }
    }
    m_Curvature /= (float) m_PixelList.size(); // average on border size
}
void Border::SetTotalIntensity(const float _value)
{
    m_TotalIntensity = _value;
}
void Border::SetNbDilatedPixels(const float _value)
{
    m_NbDilatedPixels = _value;
}
void Border::SetIntensityRatio(const float _cellInt1, const float _cellInt2)
{
    float intensity1 = 0;
    float intensity2 = 0;
    if (_cellInt1 != 0) 
    {
        intensity1 = (m_TotalIntensity / m_NbDilatedPixels) / _cellInt1;
    }
    if (_cellInt2 != 0)
    {
        intensity2 = (m_TotalIntensity / m_NbDilatedPixels) / _cellInt2;
    }
    m_IntensityRatio = (intensity1 + intensity2) / 2;
}
void Border::SetLocalBackground(const float _value)
{
    m_LocalBackground = _value;
}
void Border::SetLocalNbPixels(const float _value)
{
    m_LocalNbPixels = _value;
}
void Border::SetLocalTotalIntensity(const float _value)
{
    m_LocalTotalIntensity = _value;
}



/***
 * Misc
 */



/*** convert the border into a string
 * @return the string representation of the border
 */
std::string Border::ToString()
{
    std::ostringstream out;
    out << m_Id                  << " "  //  1
        << m_Angle               << " "  //  2
        << m_Length              << " "  //  3
        << m_Cord                << " "  //  4
        << m_Curvature           << " "  //  5
        << m_TotalIntensity      << " "  //  6
        << m_NbDilatedPixels     << " "  //  7
        << m_IntensityRatio      << " "  //  8
        << m_LocalBackground     << " "  //  9
        << m_LocalTotalIntensity << " "  // 10
        << m_LocalNbPixels       << " "; // 11
    out << PixelListToString(true);
    return out.str();
}

/*** convert pixel coordinate into string
 * @param toMatlab if true, will use the matlab references
 * @return a string containing the coordinate
 */
std::string Border::PixelListToString(const bool toMatlab) const
{
    std::ostringstream out;
    if (toMatlab)
    {
        out << indexToMatlabIndex(m_PixelList.front().GetId(), Pixel::GetHeight(), Pixel::GetWidth());
    }
    else
    {
        out << m_PixelList.front().GetId();
    }
    for (PixelListConstIteratorType it = ++m_PixelList.begin(); it != m_PixelList.end(); it++)
    {
        if (toMatlab)
        {
            out << " " << indexToMatlabIndex(it->GetId(), Pixel::GetHeight(), Pixel::GetWidth());
        }
        else
        {
            out << " " << it->GetId();
        }
    }
    return out.str();
}

/*** sort the pixel into continuous order
 * If, during sorting, we encounter a holes in the border, return the first sorted part
 * of the border. The rest of the border is not processed.
 * @return the sorted pixel list of the border.
 */
Border::PixelListType Border::SortPixel()
{
    std::list<Pixel>  sortedElementList;  // list of pixel sorted 
    std::queue<Pixel> elementsFiFoQueue;  // Queue of pixel to be processed
    unsigned int cpt = 0;                 // cpt of number of pixel in the junction
    unsigned int reverseCpt = 0;          // cpt of number of pixel in the junction
    bool exitFlag = false;

    // fill the FIFO queue with elements in reverse order
    //std::copy(m_PixelList.begin(), m_PixelList.end(), std::back_inserter(elementsFiFoQueue));
    for (PixelListReverseIteratorType ite = m_PixelList.rbegin(); ite!= m_PixelList.rend(); ++ite)
    {
        elementsFiFoQueue.push(*ite);
    }
    

    // first elements of FIFO is the first element of the sorted list
    Pixel elast = elementsFiFoQueue.front(); 
    sortedElementList.push_back(elast);      
    elementsFiFoQueue.pop();                 

    // we loop until queue fifo is empty or we hit a inconsistency problem
    while (!elementsFiFoQueue.empty() && !exitFlag)
    {

        elast = elementsFiFoQueue.front();  
        elementsFiFoQueue.pop();            

        if (sortedElementList.back().Is4Connex(elast))
        {   
            // it is a 4-connex pixel, no brainer, we push back and loop
            sortedElementList.push_back(elast);
            cpt = 0;
        }
        else if (sortedElementList.back().Is8Connex(elast))
        {
            // it is a 8-connex pixel
            if (!elementsFiFoQueue.empty())
            {
                // we make sure that their is no 4-connex in the neighbourhood
                //   this manage the 3 pixel blocks case
                //   1 2 3        vs    1 2 3
                //       4 5 6                4 5 6
                //
                unsigned int nbelt = elementsFiFoQueue.size();
                Pixel tmp = elementsFiFoQueue.front();
                bool found = false;

                // temporary loop on the remaning pixel of the queue
                while (nbelt > 0 && !found)
                {
                    if (sortedElementList.back().Is4Connex(tmp))
                    {
                        sortedElementList.push_back(tmp);
                        elementsFiFoQueue.pop();
                        found = true;
                    }
                    else
                    {
                        nbelt--;
                        elementsFiFoQueue.push(tmp);
                        elementsFiFoQueue.pop();
                        tmp = elementsFiFoQueue.front();
                    }
                }
            }

            // we push back the 8-connex pixel and loop
            sortedElementList.push_back(elast);
            cpt = 0;
        }
        else
        {           
            // not a neighbour, we push it for later
            elementsFiFoQueue.push(elast);
            cpt++;
        }

        if (cpt == elementsFiFoQueue.size())
        {
            // we have looked at all the pixel in the queue
            // we reverse it and start again
            sortedElementList.reverse();
            cpt = 0;
            reverseCpt += 1;
        }

        if (reverseCpt == 3)
        {
            // we are hitting a hole in the border
            // we break free from the loop
            exitFlag = true;
        }
    }

    // 'smaller' extremity first
    if (sortedElementList.back() < sortedElementList.front())
    {
        sortedElementList.reverse();
    }
    
    // refill pixels
    PixelListType newPixels;
    std::copy(sortedElementList.begin(), sortedElementList.end(), std::back_inserter(newPixels));
    
    return newPixels;
}

/*** get the border's centroid coordinates (macro version)
 * @param xy the pair to store the coordinates
 */
void Border::GetCentroid(std::pair<double, double> & _centroids) const
{
    _centroids.first = 0;
    _centroids.second = 0;
    std::pair<unsigned int, unsigned int> pixelCoord;
    unsigned int size = m_PixelList.size();
    for (PixelListConstIteratorType it = m_PixelList.begin(); it != m_PixelList.end(); it++)
    {
        pixelCoord = it->GetCoordinate();
        _centroids.first += pixelCoord.first;
        _centroids.second += pixelCoord.second;
    }
    _centroids.first /= double(size);
    _centroids.second /= double(size);
}

/*** get the border's centroid coordinates
 * @return the coordinates of the border's centroid
 */
std::pair<double, double> Border::GetCentroid() const
{
    std::pair<unsigned int, unsigned int> centroids = std::make_pair(0, 0);
    std::pair<unsigned int, unsigned int> point;
    unsigned int size = m_PixelList.size();
    for (PixelListConstIteratorType it = m_PixelList.begin(); it != m_PixelList.end(); it++)
    {
        point = it->GetCoordinate();
        centroids.first += point.first;
        centroids.second += point.second;
    }
    centroids.first /= double(size);
    centroids.second /= double(size);
    return centroids;
}

/*** tell if a pixel is connected to the border
 * @param _pixId the pixel id to test
 * @return true if the pixel is neighbour to one of the pixel composing the border
 */
bool Border::IsLinked(unsigned int _pixId) const
{
    bool flag = false;
    unsigned int id;
    std::pair<unsigned int, unsigned int> borderPixel;
    PixelListConstIteratorType it = m_PixelList.begin();
    while (!flag && it != m_PixelList.end())
    {
        borderPixel = it->GetCoordinate();
        id = it->GetId();
        if (_pixId != id)
        {
            if (it->Is8Connex(_pixId))
            {
                flag = true;
            }
        }
        it++;
    }
    return flag;
}

/*** test if the border contain a hole
 * @return true if the border is not continuous
 */
bool Border::IntegrityTest() const
{
    bool flag = true;
    if (m_PixelList.size() > 1)
    {
        PixelListConstIteratorType it = m_PixelList.begin();
        while (flag && it != m_PixelList.end())
        {
            flag = this->IsLinked(it->GetId());
            it++;
        }
    }
    return flag;
}

/*** difference between the pixel list of the border and a pixel list
 * @param _pixelList the pattern to compare with
 * @return the difference between the pixel list and the pattern
 */
Border::PixelListType Border::Difference(const PixelListType & _pixelList) const
{
    PixelListType difference;
    std::set<Pixel> s_model(m_PixelList.begin(), m_PixelList.end());
    std::set<Pixel> s_pattern(_pixelList.begin(), _pixelList.end());
    std::set_difference(s_model.begin(), s_model.end(), s_pattern.begin(), s_pattern.end(), std::back_inserter(difference));
    return difference;
}

std::pair<float, float> Border::MeanIntensity(const ImageType * _image, const unsigned int _conn) const
{
    std::pair<float, float> result = std::make_pair(0.0, 0.0);

    // Get image size
    unsigned int width = Pixel::GetWidth();   // image width
    unsigned int height = Pixel::GetHeight(); // image height

    // Dilate border pixels
    int x, y;
    std::vector<unsigned int> dilatedPixels;
    for (PixelListIteratorType itePixel = this->GetPixelList().begin(); itePixel != this->GetPixelList().end(); itePixel++)
    {
        // add to dilated pixel list
        dilatedPixels.push_back(itePixel->GetId());

        y = itePixel->GetCoordinate().first;
        x = itePixel->GetCoordinate().second;
        unsigned int index = itePixel->GetId();
        // 4-connexity
        if (x - 1 >= 0) // left
        {
            dilatedPixels.push_back(index - 1);
        }
        if (x + 1 < width) // right
        {
            dilatedPixels.push_back(index + 1);
        }
        if (y - 1 >= 0) // up
        {
            dilatedPixels.push_back(index - width);
        }
        if (y + 1 < height) // down
        {
            dilatedPixels.push_back(index + width);
        }
        // if 8-connexity
        if (_conn == 8)
        {
            if (y - 1 >= 0) // upper corners
            {
                if (x - 1 >= 0) // upper left
                {
                    dilatedPixels.push_back(index - width - 1);
                }
                if (x + 1 < width) //upper right
                {
                    dilatedPixels.push_back(index - width + 1);
                }
            }
            if (y + 1 < height) // lower corners
            {
                if (x - 1 >= 0) // lower left
                {
                    dilatedPixels.push_back(index + width - 1);
                }
                if (x + 1 < width) // lower right
                {
                    dilatedPixels.push_back(index + width + 1);
                }
            }
        }
    } // end for loop on pixel list

    // sort the pixels and remove duplicated
    sort(dilatedPixels.begin(), dilatedPixels.end());
    dilatedPixels.erase(unique(dilatedPixels.begin(), dilatedPixels.end()), dilatedPixels.end());

    // set number of pixels and accumulate intensity
    result.second = dilatedPixels.size();
    itk::Index<2> index;
    std::pair<unsigned int, unsigned int> xy;
    for (std::vector<unsigned int>::iterator itePixel = dilatedPixels.begin(); itePixel != dilatedPixels.end(); itePixel++)
    {
        xy = indexToXY(*itePixel, width);
        index[0] = xy.first;
        index[1] = xy.second;
        result.first += _image->GetPixel(index);
    }
    return result;
}




//end of file
