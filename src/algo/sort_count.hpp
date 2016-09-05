#ifndef SORT_COUNT_H
#define SORT_COUNT_H

#include <vector>
#include <algorithm>
#include <numeric>

class sort_count {
public:
    static bool sort( std::vector< unsigned int > input, std::vector< unsigned int > & output ){
        unsigned int max = *std::max_element( input.begin(), input.end() );
        output.resize( input.size() );
        std::vector< unsigned int > temporary( max + 1, 0 );
        std::vector< unsigned int > temporary_partial_sum_index( max + 1, 0 );
        for( auto & i : input ){
            temporary[ i ]++;
        }
        std::partial_sum( temporary.begin(), temporary.end(), temporary_partial_sum_index.begin() );
        auto it_end = input.end();
        auto it_begin = input.begin();
        if( it_end == it_begin ) return true;
        do{
            --it_end;
            unsigned int val = *it_end;
            unsigned int index = --temporary_partial_sum_index[ val ];
            output[ index ] = val;
        }while( it_end != it_begin );
        return true;
    }
};

#endif
