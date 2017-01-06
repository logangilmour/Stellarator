//
//  AllpassFilter.h
//  Stellarator
//
//  Created by Logan Gilmour on 2017-01-06.
//
//

#ifndef AllpassFilter_h
#define AllpassFilter_h

class AllpassFilter{
public:
    float process(float sample);
    void reset();
    void set(float k);
private:
    float buffer = 0;
    float c = 0.0f;
};
#endif /* AllpassFilter_h */
