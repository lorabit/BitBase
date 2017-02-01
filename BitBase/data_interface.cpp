//
//  data_interface.cpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#include "data_interface.hpp"

void data_set(string key, int value){
    
}


versioned_value data_get(string key){
    return versioned_value(0, 0);
}


bool data_cas(string key, versioned_value value){
    return true;
}

