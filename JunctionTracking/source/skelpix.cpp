/*** Class SkelPix.
 *
 * Represents a pixel of the segmentation skeleton.
 *
 * @author GOYA Yuki
 * @version 2012-08-01
 */

#include "skelpix.h"

#include <sstream>
#include <algorithm>

using namespace std;

unsigned int SkelPix::HEIGHT = 0;   //2011-10-25
unsigned int SkelPix::WIDTH = 0;    //2011-10-25

/*** constructor (default) */
SkelPix::SkelPix()
{
    idPixel = 0;
    neighbours[0] = 0;
    neighbours[1] = 0;
    neighbours[2] = 0;
    neighbours[3] = 0;
}

/*** constructor.
 * at least 2 neighbours are required ! 0 means no neighbour.
 * the neighbours are always sorted from lower to higher at initialization.
 * @param n0 a first neighbour (must be different from 0)
 * @param n1 a second neighbour (must be different from 0)
 * @param n2 a thrid neighbour
 * @param n3 a fourth neighbour
 * @param id the pixel id
 */
SkelPix::SkelPix(unsigned int n0, unsigned int n1, unsigned int n2, unsigned int n3, unsigned int id)
{
    idPixel = id;

    // sorting the neighbours
    if(n2 == 0) // 2 neighbours case
    {
        if(n0 < n1)
        {
            neighbours[0] = n0;
            neighbours[1] = n1;
        }
        else
        {
            neighbours[0] = n1;
            neighbours[1] = n0;
        }
        neighbours[2] = 0;
        neighbours[3] = 0;
    }
    else // 3 or 4 neighbours case
    {
        vector<unsigned int> temp;
        temp.push_back(n0);
        temp.push_back(n1);
        temp.push_back(n2);
        if(n3 != 0)
        {
            temp.push_back(n3);
        }
        sort(temp.begin(), temp.end()); //sort

        neighbours[0] = temp[0];
        neighbours[1] = temp[1];
        neighbours[2] = temp[2];
        if(n3 != 0)
        {
            neighbours[3] = temp[3];
        }
        else
        {
            neighbours[3] = 0;
        }
    }
}

/*** access to idPixel
 * @return the pixel id
 */
unsigned int SkelPix::GetIdPixel()
{
    return idPixel;
}

/*** access to neighbours
 * @param indix the index of the neighbour
 * @return the indix-th neighbour
 */
unsigned int SkelPix::GetNeighbour(unsigned int indix)
{
    if(indix < 4)
    {
        return neighbours[indix];
    }
    else
    {
        return 0;
    }
}

/*** convert skelpix to string
 * @return the string representation of the skelpix
 */
string SkelPix::ToString()
{
    ostringstream out;
    out << idPixel << " " << neighbours[0] << " " << neighbours[1] << " " << neighbours[2] << " " << neighbours[3];
    return out.str();
}

void SkelPix::FillString(string &s)
{
    ostringstream out;
    out << idPixel << " " << neighbours[0] << " " << neighbours[1] << " " << neighbours[2] << " " << neighbours[3];
    s = out.str();
    out.str("");
}

/*** access to the number of neighbours
 * @return the number of neighbours
 */
unsigned int SkelPix::GetNbNeighbours()
{
    if(neighbours[2] == 0)
    {
        return 2;
    }
    else
    {
        if(neighbours[3] != 0)
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }
}

/*** create the list of the neighbours couple possible with this skelpix
 * @return the list of all the neighbours couple possible
 */
vector< pair<unsigned int, unsigned int> > SkelPix::GetNeighboursCoupleList()
{
    vector< pair<unsigned int, unsigned int> > neighboursCouple;
    unsigned int i, j;
    unsigned int n = GetNbNeighbours();

    for(i = 0; i < n - 1; i++)
    {
        for(j = i + 1; j < n; j++)
        {
            neighboursCouple.push_back(make_pair(neighbours[i], neighbours[j]));
        }
    }
    return neighboursCouple;
}

/*** create the list of the neighbours couple possible with this skelpix (macro version)
 * @param the vector to fill
 */
void SkelPix::FillNeighboursCoupleList(vector<pair<unsigned int, unsigned int> > &neighboursCouple)
{
    unsigned int n = GetNbNeighbours();

    for(unsigned int i = 0; i < n - 1; i++)
        for(unsigned int j = i + 1; j < n; j++)
        {
            neighboursCouple.push_back(make_pair(neighbours[i], neighbours[j]));
        }
}


/*** determine if this skelpix is a vertex or not.
 * @return true if the skelpix is a vertex
 */
bool SkelPix::IsVertex()
{
    return (neighbours[2] != 0);
}

/*** set width
 * @param w the image width
 */
void SkelPix::SetWidth(unsigned int w)
{
    WIDTH = w;
}

/*** set height
* @param h the image height
*/
void SkelPix::SetHeight(unsigned int h)
{
    HEIGHT = h;
}

/*** access to height
 * @return the image height
 */
unsigned int SkelPix::GetHeight()
{
    return HEIGHT;
}

/*** access to width
 * @return the image width
 */
unsigned int SkelPix::GetWidth()
{
    return WIDTH;
}

/*** get (x,y) coordinates
 * @return the (x,y) coordinates of the pixel
 */
pair<unsigned int, unsigned int> SkelPix::GetXY()
{
    unsigned int x = idPixel % WIDTH;
    unsigned int y = (idPixel - x) / WIDTH;
    return make_pair(x, y);
}

/*** reset all attributes */
void SkelPix::Reset() //2012-06-07
{
    idPixel = neighbours[0] = neighbours[1] = neighbours[2] = neighbours[3] = 0;
}
/*** fill the attributes at once
 * @param n0 the first neighbour number
 * @param n1 the second neighbour number
 * @param n2 the thrid neighbour number
 * @param n3 the fourth neighbour number
 * @param id the pixel number
 */
void SkelPix::Fill(unsigned int n0, unsigned int n1, unsigned int n2, unsigned int n3, unsigned int id)
{
    idPixel = id;
    // set neighbours
    vector<unsigned int> temp;
    if(n0 != 0)
    {
        temp.push_back(n0);
    }
    if(n1 != 0)
    {
        temp.push_back(n1);
    }
    if(n2 != 0)
    {
        temp.push_back(n2);
    }
    if(n3 != 0)
    {
        temp.push_back(n3);
    }
    sort(temp.begin(), temp.end());
    for(unsigned int i = 0; i < temp.size(); i++)
    {
        neighbours[i] = temp[i];
    }
}

//end of file