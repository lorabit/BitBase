//
//  versioned_value.hpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#ifndef versioned_value_h
#define versioned_value_h


struct versioned_value {
    int version;
    int value;
    versioned_value(int _version, int _value):version(_version), value(_value){};
};

#endif /* versioned_value_h */
