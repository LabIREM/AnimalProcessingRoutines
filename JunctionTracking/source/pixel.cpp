/*** Class Pixel.
*
* Represents a pixel of the segmentation skeleton.
* A pixel will be define by an index, a list of neighbour cell, and a vertex indication
*
* @author Stephane Rigaud
* @version 2016-05-27
*/

#include "pixel.h"

#include <sstream>
#include <algorithm>


/***
 * Static values
 */


unsigned int Pixel::HEIGHT = 0; 
unsigned int Pixel::WIDTH  = 0; 


/***
 * Constructor
 */


/*** constructor
 * constructor by default
 */
Pixel::Pixel()
{
    m_Id = 0;
    m_NeighboursCellId[0] = 0;
    m_NeighboursCellId[1] = 0;
    m_NeighboursCellId[2] = 0;
    m_NeighboursCellId[3] = 0;
    m_Vertex = false;
}

/*** constructor
 * at least 2 neighbours are required ! 0 means no neighbour.
 * the neighbours are always sorted from lower to higher at initialization.
 * @param id the pixel id
 * @param n0 a first neighbour (must be different from 0)
 * @param n1 a second neighbour (must be different from 0)
 * @param n2 a thrid neighbour (default 0)
 * @param n3 a fourth neighbour (default 0)
 */
Pixel::Pixel(unsigned int _id, unsigned int _n0, unsigned int _n1, unsigned int _n2 = 0, unsigned int _n3 = 0 )
{
    m_Id = _id;

    unsigned int size = 2;
    m_NeighboursCellId[0] = _n0;
    m_NeighboursCellId[1] = _n1;
    m_NeighboursCellId[2] = _n2;
    m_NeighboursCellId[3] = _n3;
    if (_n2 != 0)
    {
        m_Vertex = true;
        size = 3;
        if (_n3 != 0)
        {
            size = 4;
        }
    }
    std::sort(m_NeighboursCellId, m_NeighboursCellId + size);
}

/*** constructor
 * constructor by copy
 * @param _p const Pixel reference
 */
Pixel::Pixel(const Pixel & _p)
{
    m_Id = _p.m_Id;
    m_Vertex = _p.m_Vertex;
    m_NeighboursCellId[0] = _p.m_NeighboursCellId[0];
    m_NeighboursCellId[1] = _p.m_NeighboursCellId[1];
    m_NeighboursCellId[2] = _p.m_NeighboursCellId[2];
    m_NeighboursCellId[3] = _p.m_NeighboursCellId[3];
}


/***
 * Operator
 */


/*** Operator ==
 * @param _p the pixel to compare with
 * @return true if same pixel id
 */
bool Pixel::operator== (const Pixel & _p) const
{
    return (this->m_Id == _p.m_Id);
}

/*** Operator ==
 * @param _id the pixel id to compare with
 * @return true if same pixel id
 */
bool Pixel::operator== (const unsigned int _id) const
{
    return (this->m_Id == _id);
}

/*** Operator >
* @param _p the pixel to compare with
* @return true if _p is smaller
*/
bool Pixel::operator>  (const Pixel & _p) const
{
    return (this->GetCoordinate().first  >= _p.GetCoordinate().first ) &&
           (this->GetCoordinate().second >  _p.GetCoordinate().second);
}

/*** Operator <
* @param _p the pixel to compare with
* @return true if _p is bigger
*/
bool Pixel::operator<  (const Pixel & _p) const
{
    return (this->GetCoordinate().first  <= _p.GetCoordinate().first ) &&
           (this->GetCoordinate().second <  _p.GetCoordinate().second);
}

/***
 * Get
 */


/*** access to id
 * @return the pixel id
 */
unsigned int Pixel::GetId() const
{
    return m_Id;
}

/*** access to HEIGHT
 * @return the image height
 */
unsigned int Pixel::GetHeight()
{
    return HEIGHT;
}

/*** access to WIDTH
 * @return the image width
 */
unsigned int Pixel::GetWidth()
{
    return WIDTH;
}

/*** access to a specific neighbour
 * @param _index the index of the neighbour
 * @return the index-th neighbour of the pixel
 */
unsigned int Pixel::GetNeighbour(unsigned int _index) const
{
    unsigned int index = 0;
    if (_index < 4)
    {
        index = m_NeighboursCellId[_index];
    }
    return index;
}

/*** access to the number of neighbours listed
 * @return the number of neighbours
 */
unsigned int Pixel::GetNbNeighbours() const
{
    unsigned int nbNeighbours = 2;
    if (m_NeighboursCellId[2] != 0)
    {
        nbNeighbours = 3;
        if (m_NeighboursCellId[3] != 0)
        {
            nbNeighbours = 4;
        }
    }
    return nbNeighbours;
}

/*** access (x,y) coordinates of the pixel
 * @return the (x,y) coordinates of the pixel
 * NB: coordinate calculated every time the function is called
 */
std::pair<unsigned int, unsigned int> Pixel::GetCoordinate() const
{
    unsigned int x = m_Id % WIDTH;
    unsigned int y = (m_Id - x) / WIDTH;
    return std::make_pair(x, y);
}

/*** create and access the list of the neighbours couple possible with this pixel
 * @return the list of all the neighbours couple possible
 */
std::vector< std::pair<unsigned int, unsigned int> > Pixel::GetNeighboursCoupleList() const
{
    std::vector< std::pair<unsigned int, unsigned int> > neighboursCouple;
    unsigned int size = GetNbNeighbours();
    for (unsigned int i = 0; i < size - 1; i++)
    {
        for (unsigned int j = i + 1; j < size; j++)
        {
            neighboursCouple.push_back(std::make_pair(m_NeighboursCellId[i], m_NeighboursCellId[j]));
        }
    }
    return neighboursCouple;
}


/***
* Set
*/


/*** Set all the attributes of a pixel
 * @param _id the pixel number
 * @param _n0 the first neighbour number (must be different from 0)
 * @param _n1 the second neighbour number (must be different from 0)
 * @param _n2 the thrid neighbour number (default 0)
 * @param _n3 the fourth neighbour number (default 0)
 */
void Pixel::Set(unsigned int _id, unsigned int _n0, unsigned int _n1, unsigned int _n2 = 0, unsigned int _n3 = 0)
{
    m_Id = _id;
    
    unsigned int size = 2;
    m_NeighboursCellId[0] = _n0;
    m_NeighboursCellId[1] = _n1;
    m_NeighboursCellId[2] = _n2;
    m_NeighboursCellId[3] = _n3;
    if (_n2 != 0)
    {
        m_Vertex = true;
        size = 3;
        if (_n3 != 0)
        {
            size = 4;
        }
    }
    std::sort(m_NeighboursCellId, m_NeighboursCellId + size);
}

/*** set width
 * @param _w the image width
 */
void Pixel::SetWidth(unsigned int _w)
{
    WIDTH = _w;
}

/*** set height
 * @param _h the image height
 */
void Pixel::SetHeight(unsigned int _h)
{
    HEIGHT = _h;
}


/***
 * Misc
 */


/*** clear all attributes 
 */
void Pixel::Clear()
{
    m_Id = 0;
    m_Vertex = false;
    m_NeighboursCellId[0] = 0;
    m_NeighboursCellId[1] = 0;
    m_NeighboursCellId[2] = 0;
    m_NeighboursCellId[3] = 0;
}

/*** determine if this skelpix is a vertex or not.
 * @return true if the skelpix is a vertex
 */
bool Pixel::IsVertex() const
{
    return m_Vertex;
}

/*** test if pixel is 8-conn with the pixel id
 * @param _id the pixel number to be test with 
 * @return true if 8-conn
 */
bool Pixel::Is8Connex(const unsigned int _id) const
{
    std::pair<unsigned int, unsigned int> currentPixel = this->GetCoordinate();
    std::pair<unsigned int, unsigned int> otherPixel = indexToXY(_id, this->GetWidth());
    return is8connex(currentPixel, otherPixel);
}

/*** test if pixel is 4-conn with the pixel id
* @param _p the pixel number to be test with
* @return true if 4-conn
*/
bool Pixel::Is4Connex(const unsigned int _id) const
{
    std::pair<unsigned int, unsigned int> currentPixel = this->GetCoordinate();
    std::pair<unsigned int, unsigned int> otherPixel = indexToXY(_id, this->GetWidth());
    return is4connex(currentPixel, otherPixel);
}

/*** test if pixel is 8-conn with the pixel 
* @param _p the pixel to be test with
* @return true if 8-conn
*/
bool Pixel::Is8Connex(const Pixel & _p) const
{
    std::pair<unsigned int, unsigned int> currentPixel = this->GetCoordinate();
    std::pair<unsigned int, unsigned int> otherPixel = _p.GetCoordinate();
    return is8connex(currentPixel, otherPixel);
}

/*** test if pixel is 4-conn with the pixel 
 * @param _p the pixel to be test with
 * @return true if 4-conn
 */
bool Pixel::Is4Connex(const Pixel & _p) const
{
    std::pair<unsigned int, unsigned int> currentPixel = this->GetCoordinate();
    std::pair<unsigned int, unsigned int> otherPixel = _p.GetCoordinate();
    return is4connex(currentPixel, otherPixel);
}

/*** create the list of the neighbours couple possible with this skelpix (macro version)
 * @param the vector to fill
 */
void Pixel::FillNeighboursCoupleList(std::vector< std::pair<unsigned int, unsigned int> > & neighboursCouple) const
{
    neighboursCouple.clear();
    unsigned int size = GetNbNeighbours();
    for (unsigned int i = 0; i < size - 1; i++)
    {
        for (unsigned int j = i + 1; j < size; j++)
        {
            neighboursCouple.push_back(std::make_pair(m_NeighboursCellId[i], m_NeighboursCellId[j]));
        }
    }
}

/*** convert pixel to string
 * @return the string representation of the skelpix
 */
std::string Pixel::ToString() const
{
    std::ostringstream out;
    out << m_Id << " " << m_NeighboursCellId[0] << " " << m_NeighboursCellId[1] << " " << m_NeighboursCellId[2] << " " << m_NeighboursCellId[3];
    return out.str();
}

/*** convert pixel into a reference string
 */
void Pixel::FillString(std::string & s) const
{
    std::ostringstream out;
    out << m_Id << " " << m_NeighboursCellId[0] << " " << m_NeighboursCellId[1] << " " << m_NeighboursCellId[2] << " " << m_NeighboursCellId[3];
    s = out.str();
    out.str("");
}

//end of file