#ifndef __localpeakdetection1D_hxx
#define __localpeakdetection1D_hxx


template< class T >
LocalPeakDetection1D<T>::LocalPeakDetection1D()
{}

template< class T >
LocalPeakDetection1D<T>:LocalPeakDetection1D( std::vector< T > & _ac)
{
    accumulator = _ac;
}

template< class T >
LocalPeakDetection1D<T>::~LocalPeakDetection1D()
{}

template< class T >
std::vector< T > 
LocalPeakDetection1D<T>::GetAccumulator()
{
    return accumulator;
}

template< class T >
void
LocalPeakDetection1D<T>::SetAccumulator(std::vector< T > & _ac)
{
    accumulator = _ac;
}

template< class T >
int
LocalPeakDetection1D<T>::DetectFirstLocalMaximum()
{
    //std::vector< std::pair< int, T > > PeakList;
    std::vector< T >::iterator it;
    for (it = accumulator.begin(); it != accumulator.end(); it++)
    {
        if (it != accumulator.begin() && it != accumulator.end())
        {
            // border management
            if (*it > *(it - 1) && *it < *(it + 1))
            {
                // peak!
                return static_cast<int>(it - accumulator.begin());
                //PeakList.push_back(std::make_pair<int, T>((int)it - accumulator.begin(), *it));
            }
        }
    }
    return 0;
}


#endif // __localpeakdetection1D_hxx
